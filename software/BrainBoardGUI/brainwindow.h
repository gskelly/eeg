#ifndef BRAINWINDOW_H
#define BRAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "brainplot.h"
#include <qwt_plot.h>

#define DEFAULT_CHANNELS 8
#define DEFAULT_LENGTH 1000
#define DEFAULT_AMP 200

/**
 * @brief Open an window that will contain all our GUI.
 */
class BrainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Creates a window and insert graphical elements.
     * For now, the window has a TabWidget in the Central Widget. This
     * Tab component has a plot chart and an empty tab.
     * @param parent QObject that contains this instance
     * @param nChannels number of channels in file
     * @param Length number of samples collected from each channel
     * @param Amplitude expected maximum value of samples
     */
    explicit BrainWindow(QWidget *parent = 0, int nChannels = DEFAULT_CHANNELS,
                         int Length = DEFAULT_LENGTH, int Amplitude = DEFAULT_AMP);
    ~BrainWindow();

    /**
     * @brief Adds one sample on each channel being displayed.
     */
    void addSample(std::vector<float>);

protected:

signals:
    
public slots:

private:
    int nChannels;
    std::vector<float> InstantData;

    QTabWidget *TabCanvas;
    BrainPlot *DataCanvas;
};

#endif // BRAINWINDOW_H
