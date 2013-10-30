#ifndef BRAINUART_H
#define BRAINUART_H

#include "brainsource.h"
#include "bbuart.h"

/**
 * @brief Reads UART port to get data and returns it throught getFrame().
 * Uses the bbEngine implemented in bbuart.* to get data from the UART
 * port and shape it in a suitable way to be displayed by BrainBoardGUI.
 * @see BrainSource
 */
class BrainUART : public BrainSource
{
public:
    /**
     * @brief Sets up a bbStream that will manage
     * @param number of channels in file
     * @param number of samples collected from each channel
     * @param overlap between frames. must be in [0, 1) interval
     * @see bbuart_init()
     * @see bbframe_create()
     * @see bbbuffer_create()
     * @see startStream();
     */
    BrainUART(int, int, float);
    ~BrainUART();

    /**
     * @brief Start reading the stream of data.
     * @note If you decide to send the 'r' character from MATLAB, you have
     * to set it in the function.
     */
    void startStream();

    /**
     * @brief Get a Frame from the engine
     * @param Data pointer to memory where data will be written
     */
    void getFrame(std::vector<float> &Data);

private:
    int nChannels;
    int SamplesPerChannel;

    bbStream stream;
    bbBuffer b;
    bbFrame eeg;
};

#endif // BRAINUART_H
