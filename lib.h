#ifndef lib_h
#define lib_h
#include <sys/shm.h>
#include <thread>
#include <sys/ipc.h>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 1024
#define SHM_KEY 0x1234
#define NAME "kienhoang"
struct shmSegment
{
    bool strComplete;
    bool numComplete;
    int number;
    char buf[BUF_SIZE];
};

class SlddSender
{
    public:

    SlddSender();
    ~SlddSender();
    void sendString(std::string data);
    void sendNumber(int num);
    private:

    int shmId;
    shmSegment *pShm;
};

class SlddReceiver
{
    public:

    SlddReceiver();
    ~SlddReceiver();
    void printString();
    void printNumber();
    private:

    int shmId;
    shmSegment *pShm;
};

#endif
