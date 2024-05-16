#include "lib.h"
using namespace std;
#define SHMGET_ERROR -1
SlddReceiver::SlddReceiver()
{
    cout<<"SlddReceiver create shared memory"<<endl;

    shmId = shmget(SHM_KEY, sizeof(shmSegment), 0644 | IPC_CREAT);
    if (shmId == SHMGET_ERROR)
    {
        cout<<"shmget error\n"<<endl;
    }

    pShm = (shmSegment *)shmat(shmId, NULL, 0);
    if (pShm == (void *)-1)
    {
        cout<<"shmat error\n"<<endl;
    }

    /* Reset value for shared memory */
    char *pBuf = pShm->buf;
    memset(pBuf, 0, BUF_SIZE);
    int *num = &(pShm->number);
    *num = 0;
    bool* strComp = &(pShm->strComplete);
    *strComp = false;
    bool* numComp = &(pShm->numComplete);
    *numComp = false;

}

SlddReceiver::~SlddReceiver()
{
    if (shmdt(pShm) == -1)
    {
        cout<<"shmdt fail !"<<endl;
    }
    // Mark the segment to be destroyed. The segment willactually be destroyed only after the last process detaches it
    if (shmctl(shmId, IPC_RMID, 0) == -1)
    {
        cout<<"shmctl fail !"<<endl;
    }
}

void SlddReceiver:: printString()
{
    while(pShm->strComplete == false);
    cout<<"print string: "<<endl;
    cout<<pShm->buf<<endl;
    pShm->strComplete =false;
}

void SlddReceiver::printNumber()
{
    while(pShm->numComplete == false);
    cout<<"print number: "<<endl;
    cout<<pShm->number<<endl;
}


SlddSender::SlddSender()
{
    cout<<"shmget start running..."<<endl;
    shmId = shmget(SHM_KEY, sizeof(shmSegment), 0644 | IPC_CREAT);
    cout<<"shm_key = 1234; shmId = "<<shmId<<endl;
    if (shmId == -1)
    {
        cout<<"shmget fail !"<<endl;
    }

    // Attach to the segment to get a pointer to it.
    cout<<"shmat start running..."<<endl;
    pShm = (shmSegment *)shmat(shmId, NULL, 0);
    if (pShm == (void *)-1)
    {
        cout<<"shmat fail !"<<endl;
    }
}

SlddSender::~SlddSender()
{
    // Write done. Detach the share memory
    cout<<"Write done. Detach the share memory"<<endl;

    if (shmdt(pShm) == -1)
    {
        cout << "shmdt" << endl;
    }
}

void SlddSender::sendString(string data)
{
    cout<<"Write string data"<<endl;

    pShm->strComplete = false;
    char *pBuf = pShm->buf;
    memcpy(pBuf, data.c_str(), data.size());
    pShm->strComplete = true;

}

void SlddSender::sendNumber(int num)
{
    cout<<"Write number data"<<endl;

    pShm->numComplete = false;
    int* tem = &(pShm->number);
    *tem = num;
    pShm->numComplete = true;
}