#include "braincollector.h"
#include "brainuart.h"
#include <iostream>

/**
 * @brief A class to speed up debugging process.
 * @details BrainFile opens a file with the data, so we don't need to
 * set up any USART device to test features not related to the commu-
 * nication port.
 * @see BrainSource
 */
class BrainFile : public BrainSource
{
public:
    /**
     * @brief Open file and initialize values
     * @param nChannels number of channels in file
     * @param SamplesPerChannel number of samples collected from each channel
     */
    BrainFile(int nChannels, int SamplesPerChannel){
        this->nChannels = nChannels;
        this->SamplesPerChannel = SamplesPerChannel;
        DataSource = fopen("nine_channels.txt", "r");
    }

    /**
     * Close file
     */
    ~BrainFile(){
        fclose(DataSource);
    }

    /**
     * @see BrainSource
     */
    void getFrame(std::vector<float> &Data){
        for(int s = 0; s < SamplesPerChannel; s++)
            for(int c = 0; c < nChannels; c++)
                fscanf(DataSource, " %f", &Data[c*SamplesPerChannel+s]);
    }

private:
    int nChannels;
    int SamplesPerChannel;

    FILE *DataSource;
    std::vector<float> Data;
};

BrainCollector::BrainCollector(QObject *parent,int nChannels, int FrameLength, float Overlap, BrainWindow *window):
    QObject(parent),
    FrameData(nChannels*FrameLength),
    InstantData(nChannels)
{
    /*Save data in object*/
    this->nChannels = nChannels;
    this->FrameLength = FrameLength;
    this->Overlap = Overlap;
    this->Window = window;

    /*Initialize control variables*/
    this->sample = 0;
    this->frameReady = false;

    /*Timer*/
    ( void )startTimer( 10 ); // 10usec. It goes as fast as the board allows

    /*Open stream*/
    DataSource = new BrainUART(nChannels, FrameLength, Overlap);//new BrainFile(nChannels, FrameLength);//
}

/*Export the frame information to user-specified variables*/
void BrainCollector::getInfo(int *nChannels, int *FrameLength, float *Overlap){
    *nChannels = this->nChannels;
    *FrameLength = this->FrameLength;
    *Overlap = this->Overlap;
}

bool BrainCollector::dataReady(){
    return frameReady;
}

/*Export data to a user-specified matrix*/
void BrainCollector::getData(float *Data){
    for(int c = 0; c < nChannels; c++)
        for(int s = 0; s < FrameLength; s++)
            Data[c*FrameLength+s] = FrameData[c*FrameLength+s];
}

void BrainCollector::timerEvent( QTimerEvent * )
{
    /*If we reach the end of a frame, requests a new one and
    offsets the counter to skip the overlapped region*/
    if(sample%FrameLength == 0){
        DataSource->getFrame(FrameData);
        if(sample != 0){
            sample = Overlap*FrameLength;
            frameReady = true;
        }
    }

    /*Display a frame line*/
    for(int c = 0; c < nChannels; c++)
        InstantData[c] = FrameData[c*FrameLength+sample];

    sample++;
    Window->addSample(InstantData);
}
