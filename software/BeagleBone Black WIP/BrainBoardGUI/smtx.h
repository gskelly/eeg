#ifndef SMTX_H
#define SMTX_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;
#include <unistd.h>


class SmTx
{
public:
    /**
     * @brief Initializes the control variables
     * @param key key to data transfer
     * @param keyHandshake key to handshaking variables
     * @param size size of memory segment for data
     * @param idle period of wait when unsuccessfull try is done
     */
    SmTx(key_t key, key_t keyHandshake, int size, int idle);

    /**
     * @brief Creates a memory segment to be shared
     * @param permission permissions grated to owner, group and world
     * @return zero on success
     */
    int create(int permission);

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
    inline void handShake(void *data){ cout<<"A\n\r";write(data); usleep(500);cout<<"B\n\r";waitRead(); usleep(500); cout<<"C\n\r"; }

    /**
     * @brief Error code for create().
     * @see create()
     */
    static const int SHMGETERROR = -1;
    /**
     * @brief Error code for create().
     * @see create()
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

    void write(void *data);
    void waitRead();
};

#endif // SMTX_H
