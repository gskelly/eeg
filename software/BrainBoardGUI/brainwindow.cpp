#include "brainwindow.h"
#include "brainplot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

BrainWindow::BrainWindow(QWidget *parent, int nChannels,
                         int Length, int Amplitude) :
    QMainWindow(parent),
    InstantData(nChannels)
{
    this->nChannels = nChannels;

    /*Creates the canvas for all tabs*/
    TabCanvas = new QTabWidget;

    /*Data plot*/
    DataCanvas = new BrainPlot(this, nChannels, Length, Amplitude);
    TabCanvas->addTab(DataCanvas, "Raw data");
    TabCanvas->addTab(new QWidget, "Future stuff");

    setCentralWidget(TabCanvas);
}

BrainWindow::~BrainWindow(){
}

void BrainWindow::addSample(std::vector<float> data)
{
    DataCanvas->timeStep(data);
}
