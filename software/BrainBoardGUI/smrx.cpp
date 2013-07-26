#include "smrx.h"
#include <cstring>
#include <unistd.h>

SmRx::SmRx(key_t key, key_t keyHandshake, int size, int idle)
{
    Key = key;
    KeyHandshake = keyHandshake;
    Size = size;
    Idle = idle;
}

/*Find a shared memory array and attach to it*/
int SmRx::join(int permission){
    if( (ID = shmget(Key, Size, permission)) < 0)
        return SHMGETERROR;

    if( (Buffer = shmat(ID, NULL, 0)) < (void *)0)
        return SHMATERROR;

    if( (Handshake = shmget(KeyHandshake, 2*sizeof(bool), permission)) < 0)
        return SHMGETERROR;

    if( (Memo = (bool *)shmat(Handshake, NULL, 0)) < (bool *)0)
        return SHMATERROR;

    return 0;
}

int SmRx::detach(){
    if( (shmdt(Buffer)) < 0)
        return -1;
    if( (shmdt(Memo)) < 0)
        return -1;
    return 0;
}

void SmRx::read(void *data){
    /*Blocks transmitter until Buffer is filled*/
    while(!Memo[READY])
        usleep(Idle);

    /*Copies data from buffer*/
    memcpy(data, Buffer, Size);

    /*Warns Transmitter we got data*/
    Memo[ACQUIRED] = true;

}

void SmRx::waitResponse(){
    /*Wait until Transmitter knows we got data*/
    while(Memo[READY])
        usleep(Idle);

    /*Finish handshaking and reinitialize flag*/
    Memo[ACQUIRED] = false;
}
