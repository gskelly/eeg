#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>

#define READY 0
#define ACQUIRED 1

class SharedMemory
{
public:
    SharedMemory(key_t key, key_t keyHandshake, int size);
    /*Initialization/termination*/
    int create(int permission);
    int join(int permission);
    int attach();
    int detach();

    /*Memory manipulation*/
    inline void *data(){ return &Data; }
    void *Data;

    /*Processes communication*/
    bool dataReady() { return Memo[0]; }
    bool dataAcquired(){ return Memo[1]; }
    inline void putData()        { Memo[0] = 1; }
    inline void gotData()        { Memo[1] = 1; }
    inline void ackDataGot()     { Memo[0] = 0; }
    inline void resetHandshake() { Memo[1] = 0; }

private:
    int Size;

    key_t Key;
    int ID;

    key_t KeyHandshake;
    int Handshake;
    bool *Memo;
};

#endif // SHAREDMEMORY_H
