#ifndef BRAINPLOT_H
#define BRAINPLOT_H

#include <qwt_plot.h>

class QwtPlotCurve;
class QwtPlotDirectPainter;

/**
 * @brief Manages the curves and legends which display the data acquired.
 */
class BrainPlot: public QwtPlot
{
    Q_OBJECT

public:
    /**
     * @brief Creates a QwtPlot that will display the EEG data
     * @param parent QObject that contains this instance
     * @param nChannels number of channels in file
     * @param Length number of samples collected from each channel
     * @param Amplitude expected maximum value of samples
     */
    BrainPlot( QWidget *parent = NULL, int nChannels = 1,
               int Length = 1000, int Amplitude = 200);
    virtual ~BrainPlot();

    /**
     * @brief Adds a point to each channel in the plot
     * @param an array containing a sample from each channel, in
     * the order it should appear
     */
    void timeStep(std::vector<float>);

    /**
     * @brief Leftover of code that inspired this class.
     * No actual importance.
     */
    void clearPoints(int);

public Q_SLOTS:

private Q_SLOTS:

private:
    int nChannels;
    int plotLength;

    int minusExpectedAmplitude;

    int sample;

    QwtPlotDirectPainter *d_directPainter;
    std::vector<QwtPlotCurve *> d_curves;
    std::vector<QwtPlotCurve *> d_zeros;
    QwtPlotCurve *d_sweep;

    void setClipRegion(QwtPlotCurve *, QPointF);
    void appendPoint(QPointF, int );
};

#endif // BRAINPLOT_H
