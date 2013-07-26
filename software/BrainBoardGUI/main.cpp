#include "brainwindow.h"
#include <QtGui>
#include "braincollector.h"
#include <unistd.h>

#include <iostream>
#include "smtx.h"

#include <pthread.h>

#define BKEY 6678
#define HSKEY 6679

#define CHANNELS 9
#define SAMPLES 50
#define OVERLAP 0

using namespace std;
void qianalg(BrainCollector *collector);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BrainWindow *window = new BrainWindow(0, CHANNELS, 500, 20);
    window->showMaximized();

    BrainCollector *collector = new BrainCollector(0, CHANNELS, SAMPLES, OVERLAP, window);

    //Not working yet =/
    //pthread_t QianAlg;
    //pthread_create(&QianAlg, NULL, (void * (*)(void *))qianalg, (void *)collector);

    return app.exec();
}

void qianalg(BrainCollector *collector){
    int nChannels,
        FrameLength;
    float Overlap;

    /*Get some important information*/
    collector->getInfo(&nChannels, &FrameLength, &Overlap);
    int infosize = sizeof(nChannels)+sizeof(FrameLength)+sizeof(Overlap);
    int datasize = nChannels*FrameLength*sizeof(float);

    /*Prepare the array that will be transferred*/
    char *infopacket = (char*)malloc(infosize);
    memcpy(infopacket, &nChannels, sizeof(nChannels));
    memcpy(infopacket+sizeof(nChannels), &FrameLength, sizeof(FrameLength));
    memcpy(infopacket+sizeof(nChannels)+sizeof(FrameLength), &Overlap, sizeof(Overlap));

    float *datapacket = (float *)malloc(datasize);
    while(!collector->dataReady())
        usleep(1000);
    collector->getData(datapacket);
    for(int i=0; i < FrameLength; i++){
        for(int j=0; j < nChannels; j++)
            printf(" %.1f", datapacket[j*FrameLength+i]);
        printf("\n\r");
    }
    /*Sends it*/
    SmTx *info = new SmTx(BKEY, HSKEY, infosize, 100);
    info->create(0666);
    cout << "infostart\n\r";
    info->handShake(infopacket);
    cout << "infoend\n\r";

    cout << datasize << "\n\r";
    SmTx *data = new SmTx(BKEY+10, HSKEY+10, datasize, 100);
    cout << data->create(0666) << "\n\r";
    cout << "datastart\n\r";
    data->handShake(datapacket);
    cout << "dataend\n\r";

}
