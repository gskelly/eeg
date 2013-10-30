#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>
#include <qwt_painter.h>
#include <qpaintengine.h>
#include "brainplot.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_plot_item.h>
#include <QVector>

/*Class to manage the arrays of data*/
class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    CurveData(int Length, int offset)
    {
        d_samples.resize(Length);
        /*Offsets all points.
        Without this procedure, all points are set to (0,0) and will
        generate an extra line in the end of plot during first screening*/
        for(int i = 0; i < Length; i++)
            d_samples.replace(i, QPointF(i, offset));
    }

    virtual QRectF boundingRect() const
    {
        if ( d_boundingRect.width() < 0.0 )
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    inline void replace(int index, const QPointF &point )
    {
        d_samples.replace(index, point);
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
};

BrainPlot::BrainPlot(QWidget *parent, int nChannels, int Length, int Amplitude):
    QwtPlot( parent ),
    d_curves( nChannels ),
    d_zeros( nChannels )
{
    /*Set some attributes*/
    minusExpectedAmplitude = -Amplitude;
    plotLength = Length;
    this->nChannels = nChannels;

    /*Start sample counter*/
    sample = 0;

    /*Painter*/
    d_directPainter = new QwtPlotDirectPainter( this );
    if ( QwtPainter::isX11GraphicsSystem() )
    {
#if QT_VERSION < 0x050000
        canvas()->setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif
        canvas()->setAttribute( Qt::WA_PaintOnScreen, true );
    }

    /*Activates the legend for channel tracking*/
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::ReadOnly);
    insertLegend( legend, QwtPlot::LeftLegend );

    /*Instantiate the reference lines and the data curves for each channel*/
    int Hfactor = 360/(nChannels+1);//different colors for each curve
    char name[15];

    for(int c = 0; c < nChannels; c++){
        /*Curve*/
        sprintf(name, "Channel %d", c+1);
        d_curves[c] = new QwtPlotCurve( name);
        d_curves[c]->setData( new CurveData(plotLength,
                                            minusExpectedAmplitude*(2*c+1)) );
        d_curves[c]->setPen( QColor::fromHsv ( Hfactor*c, 255, 255));
        d_curves[c]->setStyle( QwtPlotCurve::Lines );
        d_curves[c]->setSymbol( new QwtSymbol( QwtSymbol::NoSymbol));
        d_curves[c]->attach( this );

        /*Reference line*/
        d_zeros[c] = new QwtPlotCurve;
        d_zeros[c]->setData( new CurveData() );
        d_zeros[c]->setPen( QColor::fromHsv ( Hfactor*c, 255, 140));
        d_zeros[c]->setStyle( QwtPlotCurve::Lines );
        d_zeros[c]->setSymbol( new QwtSymbol( QwtSymbol::NoSymbol));
        d_zeros[c]->setItemAttribute(QwtPlotItem::Legend, false);
        d_zeros[c]->attach( this );

        /*Draw reference lines*/
        CurveData *data = static_cast<CurveData *>( d_zeros[c]->data() );
        data->append(QPointF(0, minusExpectedAmplitude*(2*c+1)));
        data->append(QPointF(plotLength, minusExpectedAmplitude*(2*c+1)));
        d_directPainter->drawSeries( d_zeros[c], 0, 1);
    }

    /*Sweeping line*/
    d_sweep = new QwtPlotCurve;
    d_sweep->setData( new CurveData() );
    d_sweep->setPen( Qt::black, plotLength/100, Qt::SolidLine);
    d_sweep->setStyle( QwtPlotCurve::Lines );
    d_sweep->setSymbol( new QwtSymbol( QwtSymbol::NoSymbol));
    d_sweep->setItemAttribute(QwtPlotItem::Legend, false);
    d_sweep->attach( this );

    CurveData *data = static_cast<CurveData *>( d_sweep->data() );
    data->append(QPointF(0, plotLength));
    data->append(QPointF(0, 2*nChannels*minusExpectedAmplitude));

    /*Axis*/
    setAxisScale( xBottom, 0, plotLength );
    setAxisScale( yLeft, 2*nChannels*minusExpectedAmplitude, 0 );

    enableAxis(xBottom, false);
    enableAxis(yLeft, false);

    /*Frame*/
    setFrameStyle( QFrame::NoFrame );
    setLineWidth( 0 );

    /*Canvas*/
    plotLayout()->setAlignCanvasToScales( true );
    plotLayout()->setCanvasMargin(100, QwtPlot::xBottom);
    plotLayout()->setCanvasMargin(100, QwtPlot::xTop);
    setCanvasBackground( Qt::black );

    /*Grid*/
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    grid->setMinorPen( Qt::gray, 0, Qt::DotLine );
    grid->enableYMin(true);
    grid->attach( this );

    /*Optimizaion for real-time data collecting*/
    setAutoReplot( false );

    /*Show*/
    replot();
}

BrainPlot::~BrainPlot()
{
    d_curves.clear();
    d_zeros.clear();
}

void BrainPlot::timeStep(std::vector<float> data){
    /*Curves*/
    for(int c = 0; c < nChannels; c++)
        appendPoint(QPointF(sample%plotLength, data[c]), c);

    /*Sweep line*/
    CurveData *sweep = static_cast<CurveData *>( d_sweep->data() );
    sweep->replace(0, QPointF((sample)%plotLength, plotLength));
    sweep->replace(1, QPointF((sample)%plotLength, 2*nChannels*minusExpectedAmplitude));
    setClipRegion(d_sweep, QPointF(0,0));
    d_directPainter->drawSeries( d_sweep, 0, 1);

    /*Time step*/
    sample++;

    /*Refreshes plot*/
    replot();
}

void BrainPlot::appendPoint( QPointF point, int Channel )
{
    /*Adds new point - with proper index and offset - to the data series*/
    CurveData *data = static_cast<CurveData *>( d_curves[Channel]->data() );
    point.setY(minusExpectedAmplitude*(2*Channel+1)+point.y());
    data->replace(sample%plotLength, point);
    setClipRegion(d_curves[Channel], point);

    d_directPainter->drawSeries( d_curves[Channel],
        data->size() - 1, data->size() - 1 );
}

/*Refreshes only the area that matters*/
void BrainPlot::setClipRegion(QwtPlotCurve *curve, QPointF point){
    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {
        /*
           Depending on the platform setting a clip might be an important
           performance issue. F.e. for Qt Embedded this reduces the
           part of the backing store that has to be copied out - maybe
           to an unaccelerated frame buffer device.
         */
        const QwtScaleMap xMap = canvasMap( curve->xAxis() );
        const QwtScaleMap yMap = canvasMap( curve->yAxis() );

        QRegion clipRegion;

        const QSize symbolSize = curve->symbol()->size();
        QRect r( 0, 0, symbolSize.width() + 2, symbolSize.height() + 2 );

        const QPointF center =
            QwtScaleMap::transform( xMap, yMap, point );
        r.moveCenter( center.toPoint() );
        clipRegion += r;

        d_directPainter->setClipRegion( clipRegion );
    }
}

void BrainPlot::clearPoints(int Channel)
{
    CurveData *data = static_cast<CurveData *>( d_curves[Channel]->data() );
    data->clear();

    replot();
}
