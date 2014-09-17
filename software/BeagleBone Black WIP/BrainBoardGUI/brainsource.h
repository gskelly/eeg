#ifndef BRAINSOURCE_H
#define BRAINSOURCE_H

#include <vector>

/**
 * @brief Abstract class that defines a method compatile with the BBGUI engine.
 * Every class that immplements this abstract class must have a getFrame() method.
 * @see getFrame()
 */
class BrainSource
{
public:
    /**
     * @brief Return a frame from data engine.
     * Regardless of the source, the user will get an array which implements a li-
     * nearized matrix ad contains one frame of data.
     * @param Data address to memory region where data will be written
     */
    virtual void getFrame(std::vector<float> &Data) = 0;
};

#endif // BRAINSOURCE_H
