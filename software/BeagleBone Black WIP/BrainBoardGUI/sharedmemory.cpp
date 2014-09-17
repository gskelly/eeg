#include "sharedmemory.h"
#include <stdlib.h>
#include <iostream>

SharedMemory::SharedMemory(key_t key, key_t keyHandshake, int size)
{
    Key = key;
    KeyHandshake = keyHandshake;
    Size = size;
}

/*Returns zero if success*/
int SharedMemory::create(int permission){
    ID = shmget(Key, Size, IPC_CREAT | permission);
    Data = shmat(ID, NULL, 0);

    Handshake = shmget(KeyHandshake, 2*sizeof(bool), IPC_CREAT | permission); // do the handshake!
    Memo = (bool *)shmat(Handshake, NULL, 0);
    std::cout << "ha" <<std::endl;
    //Memo[0] = false;
    //Memo[1] = false;

    return (ID < 0 || Handshake < 0);
}

int SharedMemory::join(int permission){
    ID = shmget(Key, Size, permission);
    Handshake = shmget(KeyHandshake, 2*sizeof(bool), permission);


    Data = shmat(ID, NULL, 0);
    Memo = (bool *)shmat(Handshake, NULL, 0);
}

int SharedMemory::attach(){
    Data = shmat(ID, NULL, 0);
    Memo = (bool *)shmat(Handshake, NULL, 0);

    return (Data < 0 || Memo < 0);
}

int SharedMemory::detach(){
    return shmdt(Data);
    return shmdt(Memo);
}
