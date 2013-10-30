#ifndef _BB_UART_H_
#define _BB_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/*Headers*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <pthread.h>

/*Functions and Macros*/

                                /***UART***/

/*Closes file desscriptor*/
#define bbuart_term(fd) close(fd)

/*Return a file descriptor that corresponds to the selected serial port connec-
ted with the selected baudrate*/
int bbuart_init(char *serialport, speed_t baud);

/*Send the content pointed by data to TX*/
int bbuart_write(int fd, void *data, uint8_t size);

/*Read the RX buffer and return an array with the data*/
void bbuart_read(int fd, void *data, uint8_t size);

/*Return 0 if there is any data in RX buffer and a non-zero number otherwise*/
uint8_t bbuart_isempty(int fd);

/*Return the number of bytes waiting to read in RX buffer*/
uint8_t bbuart_nbytes(int fd);

/*Read one sample of data - defined as 32bits*/
int bbuart_read32(int fd, float *data, pthread_mutex_t *pmutex);

                          /***Buffer***/
/*Buffer*/
typedef struct bbBuffer{
    float **Data;
    int Channels;
    int Samples;
    int DataOverwritten;
    int NewSamples;
    int NextFrameIndex;
    pthread_mutex_t Mutex;
} bbBuffer;

/*Returns the index for next position in buffer*/
#define bbbuffer_next(pBuffer, px) (*px = (*px+1)%pBuffer->Samples)

/*Returns number of samples on Buffer*/
#define bbbuffer_nsamples(pBuffer) (pBuffer->NewSamples)

/*Let user know if data is overwritten*/
#define bbbuffer_dataOverwritten(pBuffer) (pBuffer->DataOverwritten)

/*Let user clear data overwritten flag*/
#define bbbuffer_clearDataOverwritten(pBuffer) (pBuffer->DataOverwritten = 0)

/*Allocate buffer*/
int bbbuffer_create(bbBuffer *b, int Channel, int Samples);

/*Clear buffer*/
int bbbuffer_destroy(bbBuffer *b);

/*Returns the index for desired relative position in buffer*/
void bbbuffer_step(bbBuffer *b, int *reference, int step);

/*Returns data at a specific position*/
float bbbuffer_getAt(bbBuffer *b, int channel, int sample);

                          /***Frame***/
/*Frame*/
typedef struct bbFrame{
    float **MatrixData;
    int Channels;
    int Samples;
    int Overlap;
} bbFrame;

/*Initialize a frame*/
int bbframe_create(bbFrame *f, int chan, int len, float overlap);

/*Release memory allocated for a frame*/
void bbframe_destroy(bbFrame *f);

/*Allocate a frame, fill with data and prepare for next call*/
int bbframe_get(bbFrame *data, bbBuffer *b);

                          /***Stream***/
/*Stream*/
typedef struct bbStream{
    int fd;
    bbBuffer *b;
    pthread_t read;//, write;
    bbFrame *f;
} bbStream;

/*Creates a new thread that will poll for data in the USART.
This data is accessible by bbstream_getFrame() function*/
#define bbstream_read(s) \
    pthread_create(&(s.read), NULL, (void * (*)(void *))bbstream_readstream, (void *)&s)

/*Read a stream of data - should be called through bbstream_read()*/
void *bbstream_readstream(bbStream *stream);

/*Close files and clear up memory*/
int bbstream_term(bbStream *s);

#ifdef __cplusplus
}
#endif

#endif
