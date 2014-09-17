#include "brainuart.h"

#define BUFFER_SIZE 1500
#define FRAME_LENGTH 1

BrainUART::BrainUART(int nChannels, int frameLength, float overlap){
    this->nChannels = nChannels;
    this->SamplesPerChannel = frameLength;

    /*Open serial connection*/
    stream.fd = bbuart_init((char *)"/dev/ttyO4", B115200);
    /*Initializes data*/
    bbbuffer_create(&b, nChannels, BUFFER_SIZE);
    bbframe_create(&eeg, nChannels, frameLength, overlap);
    stream.b = &b;
    stream.f = &eeg;

    startStream();
}

BrainUART::~BrainUART(){
    /*Finish stream*/
    bbstream_term(&stream);
}

void BrainUART::startStream(){
    char r;

    /*Read r*/
    while(bbuart_isempty(stream.fd));
    //bbuart_read(stream.fd, &r, 1);
    printf("%c\n", r);

    /*Read data*/
    bbstream_read(stream);
}

void BrainUART::getFrame(std::vector<float> &Data){
    bbframe_get(&eeg, &b);
    for(int c = 0; c < nChannels; c++)
        for(int s = 0; s < SamplesPerChannel; s++)
            Data[c*SamplesPerChannel+s] = eeg.MatrixData[c][s];
}
