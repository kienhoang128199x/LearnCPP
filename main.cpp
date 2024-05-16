#include <algorithm>
#include <mutex>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include "lib.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <fstream>
#include "lib.h"


#define PARENT_PROCESS 0


const char *PATH = "/home/kienhoang/WORKSPACE/LearnCPPGitHub/ecallApp.exe";
const char *CMD = "c";
const char *COMMENT = "run chat application";

using namespace std;

int main()
{
    unsigned int mPid{0};
    mPid = fork();
    if (PARENT_PROCESS == mPid) // parent process
    {
        cout << "This is main process" << endl;
    }
    else // child process
    {
        execl(PATH, CMD, COMMENT, NULL);

        
    }

    return 0;
}


#if 0
//------------------------#if0---------------------------------------------------------

#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include "lib.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <fstream>
#include "lib.h"

using namespace std;
unsigned int volumn(0);
#define BUF_SIZE 1024
#define SHM_KEY 0x1234
const char *path = "/data001/kien4.hoang/Delete/Cpp/ecallApp.exe";
const char *cmd = "c";
const char *comment = "on monday";
int main()
{
    unsigned int mPid{0};
    mPid = fork();
    if (mPid == 0) // parent process
    {
        cout << "This is sldd process" << endl;

        SlddReceiver slr;
        slr.printString();
        slr.printNumber();
      
    }
    else // child process
    {
        
        execl(path, path, cmd, NULL);
    }

    return 0;
}

#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

class Circle
{
    public:
    virtual void display();
};

class Rectangle
{
    public:
    virtual void display();
};

class Shape : public Circle, public Rectangle
{
    public:
    void Circle::display()
    {
        cout<<"shape"<<endl;
    }
};

int main()
{
    Shape s;

    return 0;
}



// define base class
class Point
{
protected:
	int m_x, m_y;
public:
	Point(int x, int y) :m_x{ x }, m_y{ y }{};
	Point(Point& p)
	{
		m_x = p.m_x;
		m_y = p.m_y;
	}
	virtual void m_display()
	{
		cout<<"m_display of base class running..."<<endl;
		cout << "m_x: " << m_x << " and m_y: " << m_y << endl;
	}
    virtual void increase() = 0;
};

// define derived class
class ColorPoint :public Point
{
private:
	int m_color;
public:
	ColorPoint(int x, int y, int c) :Point(x, y), m_color{ c }{};
	void m_display()
	{
		cout<<"m_display of derived class running..."<<endl;
		cout << "m_x: " << m_x << " and m_y: " << m_y << " and m_color: " << m_color << endl;
	}
    void increase()
    {
        cout<<"increase"<<endl;
    }
};

int main()
{
    ColorPoint* cp1 = new ColorPoint(1,2,3);
    cp1->increase();

	Declare derived object
	ColorPoint cp1(1, 2, 3);
	
	
	// base pointer points to derived object
	Point* ptr=&cp1;
	
	// base pointer call a method which co-exist in base and derived class
	
	ptr->m_display();
	
	return 0;
}
unsigned int volumn{0};
unsigned int brightness{0};
mutex volumnMutex;
void increaseVolumn()
{
    for(unsigned int c=0;c<200000;c++)
    {
        
        {
            
            volumn++;
        }
        std::lock_guard<mutex> lgd(volumnMutex);
        brightness++;

    }
}
                            
struct FileManager
{
    int x;
    string name;
    FileManager(): x{11},name{"nam"}{}
};

char s[] = {'a','a','a','a'};
int x = 2147483647;
int main()
{
    thread th1(increaseVolumn);
    thread th2(increaseVolumn);
    if(true==th1.joinable())
    {
        th1.join();
    }
    if(true==th2.joinable())
    {
        th2.join();
    }
    cout<<volumn<<" "<<brightness<<endl;
    return 0;
}

int main()
{
    thread th1(increaseVolumn);
    thread th2(increaseVolumn);
    th1.join();
    th2.join();
    cout<<"volumn = "<<volumn<<endl;
    
    return 0;
}
void countFunc(int count)
{
    cout<<" child's threadID = "<<this_thread::get_id()<<endl;
    for(int j{0};j<count;j++)
    {
        cout<<j<<endl;
    }
}
cout<<" main's threadID = "<<this_thread::get_id()<<endl;
usleep(5000000);
cout<<"__FILENAME__ "<<__FILE__<<" __LINE__ "<<__LINE__<<" __FUNCTION__ "<<__FUNCTION__<<endl;
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include "lib.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <fstream>
#include "lib.h"

using namespace std;
unsigned int volumn(0);
#define BUF_SIZE 1024
#define SHM_KEY 0x1234
const char *path = "/data001/kien4.hoang/Delete/Cpp/ecallApp.exe";
const char *cmd = "c";

int main()
{
    unsigned int mPid{0};
    mPid = fork();
    if (mPid == 0) // parent process
    {
        cout << "This is sldd process" << endl;

        SlddReceiver slr;
        slr.printString();
        slr.printNumber();
      
    }
    else // child process
    {
        execl(path, path, cmd, NULL);
    }

    return 0;
}


int main()
{
    thread th1(increaseVolumn);
    thread th2(increaseVolumn);
    if(th1.joinable()==true)
    {
        cout<<"th1 join"<<endl;
        th1.join();
    }
    if(th2.joinable()==true)
    {
        cout<<"th2 join"<<endl;
        th2.join();
    }
    cout<<"value of volumn "<<volumn<<endl;
    return 0;
}



int main()
{
    ofstream ofs;
    ifstream ifs;
    ofs.open("./logFile.txe",std::ios_base::out);
    if(ofs.is_open()==true)
    {
        ofs<<"Time is 3.00pm"<<endl;
        ofs.close();
    }

    ofs.open("./logFile.txe",std::ios_base::out|std::ios_base::ate);
    if(ofs.is_open()==true)
    {
        ofs<<"Time is 3.36pm"<<endl;
        ofs.close();
    }
    string line;
    ifs.open("./logFile.txe",std::ios_base::in);
    if(ifs.is_open()==true)
    {
        while(getline(ifs,line))
        {
            cout<<line<<endl;
        }
        ifs.close();
    }
    return 0;
}


// share memory code
int shmId, numtimes;
        struct shmSegment *pShm;
        char *pBuf;
        int spaceAvailable;

        shmId = shmget(SHM_KEY, sizeof(shmSegment), 0644 | IPC_CREAT);
        cout<<"shmId = "<<shmId<<endl;
        if (shmId == -1)
        {
            cout << "shmget error!" << endl;
            return 1;
        }

        // Attach to the segment to get a pointer to it.
        pShm = (shmSegment *)shmat(shmId, NULL, 0);
        if (pShm == (void *)-1)
        {
            cout << "shmat error" << endl;
            return 1;
        }

        //Transfer blocks of data from buffer to shared memory
        pBuf = pShm->buf;
        spaceAvailable = BUF_SIZE; //1024 bytes
        pShm->complete = 0;
        for (numtimes = 0; numtimes < 5; numtimes++)
        {
            pShm->cnt = fill_buffer(pBuf, spaceAvailable);
            printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", pShm->cnt);
            pBuf = pShm->buf;
            sleep(3);
        }
        printf("Writing Process: Wrote %d times\n", numtimes);
        pShm->complete = 1;

        if (shmdt(pShm) == -1)
        {
            cout << "shmdt" << endl;
            return 1;
        }

        if (shmctl(shmId, IPC_RMID, 0) == -1)
        {
            cout << "shmctl" << endl;
            return 1;
        }
        printf("Writing Process: Complete\n");

#endif
