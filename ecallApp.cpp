#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "lib.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout<<"This is ecall application"<<endl;
    cout<<"argc = "<<argc<<endl;

    for(int i{0};i<argc;i++)
    {
        cout<<"argv["<<i<<"] = "<<argv[i]<<endl;
    }


    return 0;
}