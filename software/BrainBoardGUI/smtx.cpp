#include "smtx.h"
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
/*Initializes fields*/
SmTx::SmTx(key_t key, key_t keyHandshake, int size, int idle)
{
    Key = key;
    KeyHandshake = keyHandshake;
    Size = size;
    Idle = idle;
}

int SmTx::create(int permission){
    /*Instantiate the shared memory array*/
    if( (ID = shmget(Key, Size, IPC_CREAT | permission)) < 0)
        return SHMGETERROR;

    if( (Buffer = shmat(ID, NULL, 0)) < (void *)0)
        return SHMATERROR;

    /*Attach*/
    if( (Handshake = shmget(KeyHandshake, 2*sizeof(bool), IPC_CREAT | permission)) < 0)
        return SHMGETERROR;

    if( (Memo = (bool *)shmat(Handshake, NULL, 0)) < (bool *)0)
        return SHMATERROR;

    /*Initialize flags*/
    Memo[0] = false;
    Memo[1] = false;

    return 0;
}

int SmTx::detach(){
    /*Detach from memory buffer*/
    if( (shmdt(Buffer)) < 0)
        return -1;
    if( (shmdt(Memo)) < 0)
        return -1;
    return 0;
}

#include <iostream>
using namespace std;
void SmTx::write(void *data){
    /*Blocks transmitter until Buffer is free*/
    while(Memo[ACQUIRED])
        usleep(Idle);

    /*Copies data to buffer*/
    memcpy(Buffer, data, Size);

    /*Warns Receiver we have data waiting*/
    Memo[READY] = true;
}

void SmTx::waitRead(){
    /*Wait until Buffer is read by the Receiver*/
    while(!Memo[ACQUIRED])
        usleep(Idle);

    /*Tells Receiver we know the data was got
    and reinitialize flag*/
    Memo[READY] = false;
}
