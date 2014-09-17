#ifdef __cplusplus
extern "C" {
#endif

#include "bbuart.h"

/*Return a file descriptor that corresponds to the selected serial port connec-
ted with the selected baudrate*/
int bbuart_init(char *serialport, speed_t baud){
    struct termios options;
    int fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);

    if(fd == -1)
        fprintf(stderr, "bbuart_init: Unable to open %s.\n", serialport);
    else
        fcntl(fd, F_SETFL, O_NONBLOCK);

    /*Get the current options for the port*/
    tcgetattr(fd, &options);

    /*Set baudrates*/
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    /*Set the port in raw mode*/
    cfmakeraw(&options);

    /*Set port*/
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}


/*Send the content pointed by data to TX*/
int bbuart_write(int fd, void *data, uint8_t size){
    int n = write(fd, data, size);

    if(n < 0)
        fprintf(stderr, "bbuart_write: Data not sent.\n");

    return n;
}

/*Read the RX buffer and return an array with the data
Make sure that 'data' has more than 'size' bytes*/
void bbuart_read(int fd, void *data, uint8_t size){
    int n = read(fd, data, size);

    if(n < 0){
        if(errno == EAGAIN)
            fprintf(stderr, "bbuart_read: SERIAL EAGAIN ERROR\n");
        else 
            fprintf(stderr, "bbuart_read: SERIAL read error %d %s\n", errno, strerror(errno));
    }                    
}

/*Return 0 if there is any data in RX buffer and a non-zero number otherwise*/
uint8_t bbuart_isempty(int fd){
    int n;
    if(ioctl(fd, FIONREAD, &n) < 0)
        fprintf(stderr, "bbuart_isempty: SERIAL FIONREAD ERROR\n");
    return !n;
}


/*Return the number of bytes waiting to read in RX buffer*/
uint8_t bbuart_nbytes(int fd){
    int n;
    if(ioctl(fd, FIONREAD, &n) < 0)
        fprintf(stderr, "bbuart_nbytes: SERIAL FIONREAD ERROR\n");
    return n;
}

/*Read one sample of data*/
int bbuart_read32(int fd, float *data, pthread_mutex_t *pmutex){
    int dataIn;

    /*Check if there's enough data available*/
    if(bbuart_nbytes(fd) < 4)
        return -1;

    /*Read the requested number of bytes*/
    bbuart_read(fd, &dataIn, 4);
    
    /*Process and store input*/
    if(pmutex != NULL) pthread_mutex_lock(pmutex);
    *data = (float)dataIn/10.0;
    if(pmutex != NULL) pthread_mutex_unlock(pmutex);
    
    return 0;
}

/*--------------------------------------Buffer--------------------------------------*/

/*Allocate buffer*/
int bbbuffer_create(bbBuffer *b, int Channels, int Samples){
    register int i;

    /*Create data pointer*/
    b->Data = (float **)malloc(Channels*sizeof(float*));
    if(b->Data == NULL){
        fprintf(stderr, "bbbuffer_create: Unable to allocate memory for Data\n");
        return -1;
    }
    /*Create matrix*/
    for(i = 0; i < Channels; i++){
        b->Data[i] = (float *)malloc(Samples*sizeof(float));
        if(b->Data[i] == NULL){
            fprintf(stderr, "bbbuffer_create: Unable to allocate memory for Data[%d]\n", i);
            return -1;
        }
    }
        
    /*Set dimensions*/
    b->Channels = Channels;
    b->Samples = Samples;
    
    /*Set control flags*/
    b->DataOverwritten = 0;
    b->NewSamples = 0;
    b->NextFrameIndex = 0;
    if(pthread_mutex_init(&(b->Mutex), NULL)){
        fprintf(stderr, "bbbuffer_create: Unable to initialize Mutex\n");
        return -1;
    }
    
    return 0;
}

/*Releasa memory allocated to buffer*/
int bbbuffer_destroy(bbBuffer *b){
    register int i;
    
    /*Uninitialize mutex*/
    if(pthread_mutex_destroy(&(b->Mutex))){
        fprintf(stderr, "bbbuffer_destroy: Unable to uninitialize Mutex\n");
        return -1;
    }

    /*Deallocate memory*/
    for(i = 0; i < b->Channels; i++)
        if(b->Data[i] != NULL)
            free(b->Data[i]);

    if(b->Data != NULL)
        free(b->Data);
    
    return 0;
}

/*Returns the index for desired relative position in buffer*/
void bbbuffer_step(bbBuffer *b, int *reference, int step){
    *reference = (*reference+step)%(b->Samples);
    /*If index is negative, bring it to 'b' domain, as if the buffer is circular*/
    if(*reference < 0)
        *reference += (b->Samples);
}

/*Return data at a specific position*/
float bbbuffer_getAt(bbBuffer *b, int channel, int sample){
    sample %= (b->Samples);
    /*If index is negative, bring it to 'b' domain, as if the buffer is circular*/
    if(sample < 0)
        sample += (b->Samples);
    return b->Data[channel][sample];
}

/*--------------------------------------Stream--------------------------------------*/

/*Read a stream of data - should be called through bbstream_read()*/
void *bbstream_readstream(bbStream *stream){
    int sample, channel, index;
    int fd = stream->fd;
    bbBuffer *b = stream->b;
    
    //Thread is canceable, but only on 'cancel points' (in this case, usleep(1))
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    //Read a sample of data.
    for(sample = 0, index = 0; 1;sample++){
        //Read sample from USART and save in the buffer
        for(channel = 0; channel < b->Channels; channel++){
            while(bbuart_read32(fd, &(b->Data[channel][index]), &(b->Mutex)) < 0);
        }

        //Raise samples number
        pthread_mutex_lock(&(b->Mutex));
        b->NewSamples++;
        pthread_mutex_unlock(&(b->Mutex));

        //Warns user that data was overwritten
        bbbuffer_next(b, &index);
        if(b->NextFrameIndex == index){
            b->DataOverwritten = 1;
        }

        usleep(1); //Cancelation point
    }
    return 0;
}

/*Close files and clear up memory*/
int bbstream_term(bbStream *s){
    /*Finish reading thread*/
    if(pthread_cancel(s->read)){
        fprintf(stderr, "bbstream_term: Unable to cancel reading thread\n");
        return -1;
    }
    /*Close UART file descriptor*/
    bbuart_term(s->fd);
    /*Deallocate uffer*/
    bbbuffer_destroy(s->b);
    /*Deallocate frame*/
    bbframe_destroy(s->f);
    
    return 0;
}


/*These are functions to test the sending data functionality
void *bbstream_writestream(bbStream *stream);
#define bbstream_write(s) \
	pthread_create(&(s.write), NULL, (void *)bbstream_writestream, (void *)&s)
*/

//struct element
//pthread_cancel(s->write); in bbstream_term

/*Sends a stream of a single number - for now
void *bbstream_writestream(bbStream *stream){
    int n = 17;
    
    //Thread is canceable, but only on 'cancel points' (in this case, usleep(1))
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    //Streams a number through UART
    while(1){
        bbuart_write(stream->fd, (void *)&n, 4);
        usleep(1000000/FREQ);
    }
    
    return 0;
}*/

/*--------------------------------------Frame--------------------------------------*/

/*Initialize a frame*/
int bbframe_create(bbFrame *f, int chan, int len, float overlap){
    int i;

    f->Channels = chan;
    f->Samples = len;
    f->Overlap = (int)(overlap*f->Samples);
    f->MatrixData = (float **)malloc(f->Channels*sizeof(float *));
    if(f->MatrixData == NULL){
        fprintf(stderr, "bbstream_createFrame: Unable to allocate memory for MatrixData\n");
        return -1;
    }
    for(i = 0; i < f->Channels; i++){
        f->MatrixData[i] = (float *)malloc((f->Samples)*sizeof(float));
        if(f->MatrixData == NULL){
            fprintf(stderr, "bbstream_createFrame: Unable to allocate memory for MatrixData[%d]\n", i);
            return -1;
        }
    }
    
    return 0;
}

/*Release memory allocated for a frame*/
void bbframe_destroy(bbFrame *f){
    int j;
    for(j=0; j<f->Channels;j++)
        if(f->MatrixData[j] != NULL)
            free(f->MatrixData[j]);

    if(f->MatrixData != NULL)
        free(f->MatrixData);
}

/*Allocate a frame, fill with data and prepare for next call*/
int bbframe_get(bbFrame *data, bbBuffer *b){
    int samp, chan;
    static int overlap = 0; //First iteraction consumes more samples. This
                            //variable will be changed at end of this function

    /*If the buffer is not ready, warns the user*/
    if(bbbuffer_nsamples(b) < (data->Samples - overlap))
        return -1;
        
    /*Fill frame*/
    for(samp = 0; samp < data->Samples; samp++){
        for(chan = 0; chan < data->Channels; chan++)
            data->MatrixData[chan][samp] = b->Data[chan][b->NextFrameIndex];
        /*Get next sample*/
        bbbuffer_next(b, &(b->NextFrameIndex));
    }
    
    /*After copying data, brings the index back to guarantee the overlap*/
    b->NextFrameIndex -= data->Overlap;

    /*Clear NewSamples*/
    pthread_mutex_lock(&(b->Mutex));
    b->NewSamples -= data->Samples - overlap;
    pthread_mutex_unlock(&(b->Mutex));

    //Next iteraction won't be the first anymore
    overlap = data->Overlap;
    
    return 0;
}

#ifdef __cplusplus
}
#endif
