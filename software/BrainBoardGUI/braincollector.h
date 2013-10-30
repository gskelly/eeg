#ifndef BRAINCOLLECTOR_H
#define BRAINCOLLECTOR_H

#include <QObject>
#include "brainwindow.h"
#include "brainsource.h"

/**
 * @brief Links a data engine and the GUI.
 * @details Gets a BrainWindow as a parameter and links to a BrainSource. The
 * BrainCollector object calls the getFrame() method from BrainSource to col-
 * lect data and calls addSample() from BrainWindow to display it.This is do-
 * ne inside a timer callback funtion.
 * @todo Add BrainSource as a constructor parameter.
 */
class BrainCollector : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Saves data in class attributes.
     * @param parent QObject that contains this instance
     * @param nChannels number of channels in file
     * @param FrameLength number of samples collected from each channel
     * @param Overlap overlap between frames. must be in [0, 1) interval
     * @param window BrainWindow object
     * @see BrainWindow
     * @see BrainSource
     */
    explicit BrainCollector(QObject *parent = 0, int nChannels = 0,
                            int FrameLength = 1, float Overlap = 0, BrainWindow *window = 0);

    /**
     * @brief Get the dimensions and overlap of gathered data using pointers
     * @param nChannels
     * @param FrameLength
     * @param Overlap
     */
    void getInfo(int *nChannels, int *FrameLength, float *Overlap);

    /**
     * @brief Copy the current frame to the address pointed by Data.
     * Should be called only when dataReady() returns true.
     * @param Data
     * @see dataReady()
     */
    void getData(float *Data);

    /**
     * @brief Allow us to know if the first frame was already built.
     * @return true or false
     */
    bool dataReady();

signals:
    
public slots:
private:
    /*Info*/
    int nChannels;
    int FrameLength;
    float Overlap;
    /*Control*/
    int sample;
    bool frameReady;

    /*Storage*/
    std::vector<float> FrameData;
    std::vector<float> InstantData;
    BrainWindow *Window;
    BrainSource *DataSource;

    void timerEvent( QTimerEvent * );
};

#endif // BRAINCOLLECTOR_H


