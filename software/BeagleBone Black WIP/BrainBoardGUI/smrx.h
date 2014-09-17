#ifndef SMRX_H
#define SMRX_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>

/**
 * @brief Reads data from a shared memory piece of memory.
 * Should be used with SmTx. They will share the same keys and the
 * handShaking() function will handle the data transfer. Both app-
 * lications are blocked until data gets transferred.
 */
class SmRx
{
public:public:
    /**
     * @brief Initializes the control variables
     * @param key key to data transfer
     * @param keyHandshake key to handshaking variables
     * @param size size of memory segment for data
     * @param idle period of wait when unsuccessfull try is done
     */
    SmRx(key_t key, key_t keyHandshake, int size, int idle);

    /**
     * @brief Joins a created memory segment
     * @param permission permissions grated to owner, group and world
     * @return zero on success
     */
    int join(int permission);

    /**
     * @brief Detach from memory segment.
     * @return zero on success
     */
    int detach();

    /**
     * @brief The 4-state handshaking protocol.
     * The Rx waits for data and for the response of transmitter.
     * @param data pointer to memory where data will be stored
     */
    inline void handShake(void *data){ read(data); waitResponse(); }

    /**
     * @brief Error code for join().
     * @see join()
     */
    static const int SHMGETERROR = -1;
    /**
     * @brief Error code for join().
     * @see join()
     */
    static const int SHMATERROR = -2;
private:
    int Size;
    int Idle;

    key_t Key;
    int ID;
    void *Buffer;

    key_t KeyHandshake;
    int Handshake;
    bool *Memo;

    static const int READY = 0;
    static const int ACQUIRED = 1;

    /*Handshaking steps*/
    void read(void *data);
    void waitResponse();
};

#endif // SMRX_H
