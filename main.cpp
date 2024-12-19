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
#include <vector>
using namespace std;

int main()
{
    cout<<"now is 21.06"<<endl;
    return 0;
}


#if 0
//------------------------#if0---------------------------------------------------------

int main()
{
    cout << "main run, pid == " << getpid() << "  thid == " << this_thread::get_id << endl;
    int x{0};
    string s{""};
    while (true)
    {
        for (int i{0}; i < 100000000; i++)
        {
            for (int i{0}; i < 10000000; i++)
            {
                cout << x++ << endl;
                cout << s << endl;
            }
        }
    }
    return 0;
}



/**
 * Proxy design pattern code example
*/

// The abstract base class for pure virtual method
class IImage
{
public:
    virtual void show() = 0;
};

// The real class that will run actual method .Ex show()
class Image : public IImage
{
private:
    string fileName;

public:
    Image(string s) : fileName{s}
    {
        cout << "Load image " << s << endl;
    }
    void show()
    {
        cout << "show image " << fileName << endl;
    }
    ~Image()
    {
        cout<<"Image's destructor"<<endl;
    }
};

// The proxy class that will create real object when needed and request method
class ProxyImage : public IImage
{
private:
    Image *pImage;
    string fileName;

public:
    ProxyImage(string s) : fileName{fileName}, pImage{nullptr}
    {
        cout<<"ProxyImage's constructor"<<endl;
    }
    void show()
    {
        if(nullptr == pImage)
        {
            pImage = new Image(fileName); // only create object when needed
        }

        pImage->show();
    }
    ~ProxyImage()
    {
        delete pImage;
    }
};

int main()
{
    ProxyImage pImg("dog image");
    pImg.show();
}


// The following code implements two kinds of proxy: the virtual proxy described
// in the Motivation section, and a proxy implemented with doesNotUnderstand:.7
// 1. A virtual proxy. The Graphic class defines the interface for graphical
// objects:
class Graphic
{
public:
    virtual ~Graphic();
    virtual void Draw(const Point &at) = 0;
    virtual void HandleMouse(Event &event) = 0;
    virtual const Point &GetExtent() = 0;
    virtual void Load(istream &from) = 0;
    virtual void Save(ostream &to) = 0;

protected:
    Graphic();
};
// The Image class implements the Graphic interface to display image files.
// Image overrides HandleMouse to let users resize the image interactively.
// real class
class Image : public Graphic
{
public:
    Image(const char *file); // loads image from a file
    virtual ~Image();
    virtual void Draw(const Point &at);
    virtual void HandleMouse(Event &event);
    virtual const Point &GetExtent();
    virtual void Load(istream &from);
    virtual void Save(ostream &to);

private:
    // ...
};
// Design Patterns: Elements of Reusable Object-Oriented Software
// ImageProxy has the same interface as Image:
class ImageProxy : public Graphic
{
public:
    ImageProxy(const char *imageFile);
    virtual ~ImageProxy();
    virtual void Draw(const Point &at);
    virtual void HandleMouse(Event &event);
    virtual const Point &GetExtent();
    virtual void Load(istream &from);
    virtual void Save(ostream &to);

protected:
    Image *GetImage();

private:
    Image *_image;
    Point _extent;
    char *_fileName;
};
// The constructor saves a local copy of the name of the file that stores the
// image, and it initializes _extent and _image:
ImageProxy::ImageProxy(const char *fileName)
{
    _extent = Point::Zero; // don't know extent yet
    _image = 0;
}
Image *ImageProxy::GetImage()
{
    if (_image == 0)
    {
        _image = new Image(_fileName);
    }
    return _image;
}
// The implementation of GetExtent returns the cached extent if possible;
// otherwise the image is loaded from the file. Draw loads the image, and
// HandleMouse forwards the event to the real image.
// Design Patterns: Elements of Reusable Object-Oriented Software
// 242
const Point &ImageProxy::GetExtent()
{
    if (_extent == Point::Zero)
    {
        _extent = GetImage()->GetExtent();
    }
    return _extent;
}
void ImageProxy::Draw(const Point &at)
{
    GetImage()->Draw(at);
}
void ImageProxy::HandleMouse(Event &event)
{
    GetImage()->HandleMouse(event);
}
// The Save operation saves the cached image extent and the image file name
// to a stream. Load retrieves this information and initializes the
// corresponding members.
void ImageProxy::Save(ostream &to)
{
    to << _extent << _fileName;
}
void ImageProxy::Load(istream &from)
{
    from >> _extent >> _fileName;
}
// Finally, suppose we have a class TextDocument that can contain Graphic
// objects:
class TextDocument
{
public:
    TextDocument();
    void Insert(Graphic *);
    // ...
};
// We can insert an ImageProxy into a text document like this:
// TextDocument* text = new TextDocument;
// Design Patterns: Elements of Reusable Object-Oriented Software
// 243
// ...
text->Insert(new ImageProxy("anImageFileName"));


/////////////////////////////////////////////////////////
// Class that represents a simple thread pool
class ThreadPool
{

private:
    // Vector to store worker threads
    vector<thread> threads_;

    // Queue of tasks
    queue<function<void()>> tasks_;

    // Mutex to synchronize access to shared data
    mutex queue_mutex_;

    // Condition variable to signal changes in the state of
    // the tasks queue
    condition_variable cv_;

    // Flag to indicate whether the thread pool should stop
    // or not
    bool stop_ = false;

public:
    // // Constructor to creates a thread pool with given number of threads
    ThreadPool(size_t num_threads = thread::hardware_concurrency()) // Returns the number of concurrent threads supported by the implementation.
    // The value should be considered only a hint.
    {
        // Creating worker threads
        for (size_t i = 0; i < num_threads; ++i)
        {
            threads_.emplace_back([this]
            { 
				while (true) 
                { 
					function<void()> task; // same as void (*task)();
					// The reason for putting the below code here is to unlock the queue before executing the task so that other threads can perform enqueue tasks 
					{ 
						// Locking the queue so that data can be shared safely 
						unique_lock<mutex> lock(queue_mutex_); 

						// Waiting until there is a task to execute (message is enqueued then cv_.notify_one()) or the pool is stopped 
						cv_.wait(lock, [this] 
                        { 
							return (!tasks_.empty() || stop_); 
						}); 

						// exit the thread in case the pool is stopped and there are no tasks
                        if (stop_ && tasks_.empty())
                        {
                            return;
                        }

                        // Get the next task from the queue 
						task = move(tasks_.front()); 
						tasks_.pop(); 
					} 

					task(); 
				} 
            });
        }
    }

    // Destructor to stop the thread pool
    ~ThreadPool()
    {
        {
            // Lock the queue to update the stop flag safely
            unique_lock<mutex> lock(queue_mutex_);
            stop_ = true;
        }

        // Notify all threads
        cv_.notify_all();

        // Joining all worker threads to ensure they have completed their tasks
        for (auto &thread : threads_)
        {
            thread.join();
        }
    }

    // Enqueue task for execution by the thread pool
    void enqueue(function<void()> task)
    {
        {
            unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace(move(task));
        }
        cv_.notify_one();
    }
};

int main()
{
    // Create a thread pool with 4 threads
    ThreadPool pool(4);

    // Enqueue tasks for execution
    for (int i = 0; i < 15; ++i)
    {
        pool.enqueue([i]
                     { 
			cout << "Task " << i << " is running on thread "
				<< this_thread::get_id() << endl; 
			// Simulate some work 
			this_thread::sleep_for( 
				chrono::milliseconds(100)); });
    }

    return 0;
}




int volume{0};
std::mutex mt;



/*
    Name: notify_one
    Copyright: LG
    Author: kien4.hoang
    Date: 22/11/21 11:04
    Description: Demonstrate how to use notify_one method of condition variable
*/

#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable consume;

void eatCake(int id)
{
    std::unique_lock<std::mutex> unl(mtx); // lock for running cout in order
    cout << "Consumer number " << id << " waiting for a cake..." << endl;

    consume.wait(unl);
    cout << "Consumer number " << id << " has already eaten a cake" << endl;
}

void makeCake()
{
    std::unique_lock<std::mutex> unl(mtx);
    cout << "Producer makes cakes" << endl;
    consume.notify_one();
}

int main()
{
    // create 5 consumer threads for eating cake
    thread consumers[5];
    thread producer[5];
    for (int i = 0; i < 5; ++i)
    {
        consumers[i] = thread(eatCake, i);
    }

    // delay time for 5 consumer threads done
    this_thread::sleep_for(chrono::milliseconds(500));

    // run thread producer, notify_one to one of 5 consumer which are waiting
    // for (int i = 0; i < 5; ++i)
    // {
    //     int x{0};
    //     cout << "input number" << endl;
    //     cin >> x;
    //     producer[i] = thread(makeCake);
    // }
    consume.notify_all();
    // main thread waiting for other thread
    for (int i = 0; i < 5; ++i)
    {
        if(producer[i].joinable())
        {
            producer[i].join();
        }
        else
        {
            // cout<<"producer["<<i<<"] not joinable"<<endl;
        }
    }
    for (int i = 0; i < 5; ++i)
    {
        if(consumers[i].joinable())
        {
            consumers[i].join();
        }
        else
        {
            // cout<<"consumers["<<i<<"] not joinable"<<endl;
        }
    }

    // NOTE THAT PROGRAM WILL STUCK BECAUSE 4 CONSUMER STILL WAITING
    return 0;
}



void increaseVolume()
{

    cout << "this_thread::get_id() == " << this_thread::get_id() << endl;
    for (int i{0}; i < 100000; i++)
    {
        mt.lock();
        volume++;
        mt.unlock();
    }
}

int main()
{
    cout << "run main.exe, pid == " << getpid() << " ; this_thread::thread_id() == " << this_thread::get_id() << endl;
    std::thread th1(increaseVolume), th2(increaseVolume);
    if (th1.joinable())
    {
        th1.join();
    }
    if (th2.joinable())
    {
        th2.join();
    }
    cout << "volume == " << volume << endl;
    return 0;
}




/////////////////////////////////////////////////////////////////
/**
 * Observer design pattern implementation start
*/

class IECMReceiver
{
public:
    virtual void update(string s) = 0;
};

class ECMService
{
private:
    vector<IECMReceiver *> vIEcm;
    ECMService()
    {
        cout << "create object ECMService " << this << endl;
    }

public:
    static ECMService *getInstance()
    {
        static ECMService ecm;
        return &ecm;
    }
    void regis(IECMReceiver *pRev)
    {
        cout << "Add receiver " << pRev << " to vector" << endl;
        vIEcm.push_back(pRev);
    }
    void remove()
    {
        cout << "remove fun" << endl;
    }
    void notify(string str)
    {
        cout << "notify function" << endl;
        for (auto rev : vIEcm)
        {
            rev->update(str);
        }
    }
};

class EcallApp : public IECMReceiver
{
public:
    EcallApp()
    {
        cout << "Create ecall app " << this << endl;
    }
    void update(string s)
    {
        cout << this << " update data " << s << endl;
    }
    void registerReceiver()
    {
        ECMService::getInstance()->regis(this);
    }
};

int main()
{
    cout << "main fun" << endl;
    EcallApp a1, a2, a3;
    ECMService::getInstance()->regis(&a1);
    ECMService::getInstance()->regis(&a3);
    ECMService::getInstance()->notify("hoang trung kien");
    return 0;
}

/////////////Observer design pattern implementation end///////////////////////////////////////////////////

#include <bits/stdc++.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <utility>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

// class Point
// {
// private:
//     int x;

// public:
//     Point(int tx) : x{tx}
//     {
//         cout << "Point's constructor" << endl;
//     }
//     void show(Point p)
//     {
//         cout << "p.x == " << p.x << endl;
//     }
//     ~Point()
//     {
//         cout << "Point destructor" << endl;
//     }
// };

typedef int (*fptr)(int, int);

void show()
{
    cout << "global show" << endl;
}
int sum(int x, int y)
{
    cout << "global's sum function" << endl;
    return (x + y);
}
namespace myNamespace
{

    void show()
    {
        cout << "myNamespace's show" << endl;
    }

    int sum(int x, int y)
    {
        cout << "myNamespace's sum function" << endl;
        return (x + y);
    }

}

using namespace myNamespace;
using myUint = unsigned int;
int main()
{
    // sum(1, 2);
    show();

    return 0;
}

#if 0
////////////////////////////////////////

class Point
{
private:
    int x;

public:
    Point()
    {
        cout << "Point constructor" << endl;
    }
    int show()
    {
        cout << "show" << endl;
        // x++;
        return 1;
    }
};
int main()
{
    Point *p = nullptr;
    if (p->show() == 1)
    {
        cout << "oke" << endl;
    }
    return 0;
}

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
/**
 *     The following program prints part of the file specified in its
       first command-line argument to standard output.  The range of
       bytes to be printed is specified via offset and length values in
       the second and third command-line arguments.  The program creates
       a memory mapping of the required pages of the file and then uses
       write(2) to output the desired bytes.
*/
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    int fd;
    char *addr;
    off_t offset, pa_offset;
    size_t length;
    ssize_t s;
    struct stat sb;

    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "%s file offset [length]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");

    if (fstat(fd, &sb) == -1) /* To obtain file size */
        handle_error("fstat");

    offset = atoi(argv[2]);
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
    /* offset for mmap() must be page aligned */

    if (offset >= sb.st_size)
    {
        fprintf(stderr, "offset is past end of file\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 4)
    {
        length = atoi(argv[3]);
        if (offset + length > sb.st_size)
            length = sb.st_size - offset;
        /* Can't display bytes past end of file */
    }
    else
    { /* No length arg ==> display to end of file */
        length = sb.st_size - offset;
    }

    addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
                MAP_PRIVATE, fd, pa_offset);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
    if (s != length)
    {
        if (s == -1)
            handle_error("write");

        fprintf(stderr, "partial write");
        exit(EXIT_FAILURE);
    }

    munmap(addr, length + offset - pa_offset);
    close(fd);

    exit(EXIT_SUCCESS);
}



void show(int x = 0)
{
    int y __attribute__((unused));
    cout<<"show"<<endl;
}
int main()
{
    int x __attribute__((unused));
    show(1);
    return 0;
}


const string FILE_NAME = "poem2024.cpp";

int main()
{
    string s;
    char my_template[] = "filenameXXXXXX";
    int id = mkstemp(my_template);
    if(-1==id)
    {
        cout<<"mkstemp fail"<<endl;
    }
    /**
     * create and write to file
    */
    ofstream ofs;
    ofs.open(FILE_NAME, std::ios_base::out|std::ios_base::app);
    while(ofs)
    {
        getline(cin,s);
        if(s=="-1") break;
        ofs<<s<<endl;
    }
    ofs.close();

    // /**
    //  * read from file
    // */
    ifstream ifs;
    ifs.open(FILE_NAME,ios_base::in);
    while(getline(ifs,s))
    {
        cout<<s<<endl;
    }
    ifs.close();
    return 0;
}




// C++ Program to demonstrate thread pooling

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <cstdarg>
using namespace std;
void show(int num, ...)
{
    va_list vl;
    va_start(vl,num);
    for(int i{0};i<num;i++)
    cout<<va_arg(vl, int)<<endl;
    va_end(vl);
}
int main()
{
    show(3,6,7,8);
    return 0;
}

#include <iostream>
using namespace std;
void printLog()
{
	cout<<endl;
}
template <typename T,typename...Types> void printLog(T var1,Types...varn)
{
	cout<<var1<<" ";
	printLog(varn...);
}



/*
    Name: notify_one
    Copyright: LG
    Author: kien4.hoang
    Date: 22/11/21 11:04
    Description: Demonstrate how to use notify_one method of condition variable
*/

#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable consume;

void eatCake(int id)
{
    std::unique_lock<std::mutex> unl(mtx); // lock for running cout in order
    cout << "Consumer number " << id << " waiting for a cake..." << endl;

    consume.wait(unl);
    cout << "Consumer number " << id << " has already eaten a cake" << endl;
}

void makeCake()
{
    std::unique_lock<std::mutex> unl(mtx);
    cout << "Producer makes cakes" << endl;
    consume.notify_one();
}

int main()
{
    // create 5 consumer threads for eating cake
    thread consumers[5];
    thread producer[5];
    for (int i = 0; i < 5; ++i)
    {
        consumers[i] = thread(eatCake, i);
    }

    // delay time for 5 consumer threads done
    this_thread::sleep_for(chrono::milliseconds(500));

    // run thread producer, notify_one to one of 5 consumer which are waiting
    for (int i = 0; i < 5; ++i)
    {
        int x{0};
        cout << "input number" << endl;
        cin >> x;
        producer[i] = thread(makeCake);
    }

    // main thread waiting for other thread
    for (int i = 0; i < 5; ++i)
    {
        producer[i].join();
    }
    for (int i = 0; i < 5; ++i)
    {
        consumers[i].join();
    }

    // NOTE THAT PROGRAM WILL STUCK BECAUSE 4 CONSUMER STILL WAITING
    return 0;
}


class Point
{
public:
    int x;

public:
    Point(int tem) : x{tem}
    {
        cout << "Point's constructor 1 par" << endl;
    }
    Point()
    {
        cout << "Point's constructor non par" << endl;
    }
    void show()
    {
        cout << "x == " << x << endl;
    }
    virtual ~Point()
    {
        cout << "Point's destructor" << endl;
    }
};

class A : public virtual Point
{
public:
    A(int tem) : Point(tem)
    {
        x = tem;
        cout << "A's constructor" << endl;
    }
    ~A()
    {
        cout << "A's destructor" << endl;
    }
};

class B : public virtual Point
{
public:
    B(int tem) : Point(tem)
    {
        x = tem;
        cout << "B's constructor" << endl;
    }
    ~B()
    {
        cout << "B's destructor" << endl;
    }
};

class X : public B, public A
{
public:
    X(int tem) : Point(7), B(1), A(2) 
    {
        cout << "X's constructor" << endl;
    }
    ~X()
    {
        cout << "X's destructor" << endl;
    }
};

int main()
{
    X x(9);
    x.show();
    return 0;
}


using namespace std;
void show(int n)
{
    while (true)
    {
        n++;
        cout << n << endl;
        usleep(1000000);
    }
}

int main()
{
    cout<<"main thread, id == "<<this_thread::get_id()<<endl;
    thread th(show, 3);
    th.detach();
    cout<<"main finish"<<endl;
    return 0;
}

#define MAX_SPACE 5
int n = 1;
void showSpace(int n)
{
    for(int i{0};i<n;i++)
    {
        cout<<"   ";
    }
}
void showNumber(int& num, int l)
{
    for(int i{0};i<l;i++)
    {
        cout<<num<<" ";
        num++;
    }
    cout<<endl;
}
int main()
{
    for(int i{1};i<=11;i+=2)
    {
        showSpace(MAX_SPACE - i/2);
        showNumber(n, i);
    }
    for(int i{9};i>=1;i-=2)
    {
        showSpace(MAX_SPACE - i/2);
        showNumber(n, i);
    }
    return 0;
}


int sum(int a, int b)
{
    return (a+b);
}
int main()
{
    int (*fp)(int,int);
    fp = sum;
    cout<<fp<<endl; --> 1
    cout<<reinterpret_cast<void*>(fp)<<endl; --> 0x55cfebe00d7a
    return 0;
}


int main()
{
    long unsigned int n{0}, s{0}, b{0};
    vector<long unsigned int> iv;
    cin >> n >> s;
    for (long unsigned int i{0}; i < n; i++)
    {
        long unsigned int tem;
        cin >> tem;
        iv.push_back(tem);
    }
    long unsigned int p = iv[0];
    set<long unsigned int> dif;
    vector<long unsigned int> tem;

    for (long unsigned int i{1}; i < n; i++)
    {
        if (p >= iv[i])
        {
            // cout<<"set p = "<<iv[i]<<endl;
            
        }
        else // p<iv[i]
        {
            tem.push_back(p - iv[i]);
        }
        p = iv[i];
    }
    sort(tem.begin(), tem.end());
    for (long unsigned int i = s; i < tem.size(); i++)
    {
        b -= tem[i];
    }
    cout << b << endl;

    return 0;
}


class Solution
{
public:
    vector<int> mostCompetitive(vector<int> &nums, int k)
    {
        stack<int> s;
        int n = nums.size();
        int pos;
        for (int i{0}; i < n; i++)
        {
            if ((n - 1 - i) >= (k - s.size() - 1))
            {
                if(!s.empty())
                {
                    cout << "compare " << nums[i] << " " << s.top() << endl;
                    while (s.top() > nums[i])
                    {
                        s.pop();
                    }
            
                }

                cout<<"push "<<nums[i]<<endl;
                s.push(nums[i]);
                // if (s.size() == k)
                // {
                //     break;
                // }
            }
            else
            {
                for (int j{i}; j < n; j++)
                {
                    s.push(nums[j]);
                }
                break;
            }
        }

        vector<int> ov;
        while (!s.empty())
        {
            ov.push_back(s.top());
            s.pop();
        }

        reverse(ov.begin(), ov.end());

        return ov;
    }
};
int main()
{
    vector<int> iv{1, 4, 5, 3, 12, 10};
    int k{3};
    Solution s;

    vector<int> ov = s.mostCompetitive(iv, k);
    for (auto i : ov)
    {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}



/**
 * https://leetcode.com/problems/find-the-most-competitive-subsequence/
 * OK but timelimit exceed
*/
class Solution
{
public:
    vector<int> mostCompetitive(vector<int> &nums, int k)
    {
        vector<int> res; // result vector
        set<int> s; // 
        for(auto i:nums)
        {
            s.insert(i);
        }
        int start{0};
        auto it = s.begin();
        while (res.size() != k)
        {
            auto pos = find(nums.begin() + start, nums.end(), *it);
            if (pos != nums.end())
            {
                if ((pos - nums.begin() + k - (res.size()+1)) <= (nums.size() - 1))// enough element in s
                {
                    start = pos - nums.begin() + 1;
                    res.push_back(*it);
                    it = s.begin();
                }
                else // not enough --> check next element in s
                {
                    it++;
                }
            }
            else
            {
                it++;
            }
        }
        return res;
    }
};

int main()
{
    vector<long unsigned int> iv, crr;
    vector<vector<long unsigned int>> ov;
    int cnt{0};
    long unsigned int n{0};

    cin >> n;
    for (long unsigned int i{0}; i < n; i++)
    {
        long unsigned int tem{0};
        cin >> tem;
        iv.push_back(tem);
    }
    for (long unsigned int i{0}; i < n; i++)
    {
        for (long unsigned int j{i+1}; j < n; j++)
        {
            if((iv[i]&iv[j])>=(iv[i]^iv[j]))
            {
                cnt++;
            }
        }
    }
    cout<<cnt<<endl;
    return 0;
}

/**
 * generate all subset of an array (element in array may be not unique)
*/

long unsigned int funA(vector<long unsigned int> iv)
{
    long unsigned int n = iv.size();
    long unsigned int res = iv[0];
    for (long unsigned int i{1}; i < n; i++)
    {
        res ^= iv[i];
    }
    res ^= n;
    return res;
}
void backtracking(long unsigned int idx, vector<long unsigned int> &iv, vector<vector<long unsigned int>> &ov, vector<long unsigned int> &crr)
{
    for (long unsigned int i{idx}; i < iv.size(); i++)
    {
        crr.push_back(iv[i]);
        bool coincide{false};
        for (auto k : ov)
        {
            long unsigned int v = 0;
            if (k.size() == crr.size())
            {
                for (long unsigned int m{0}; m < k.size(); m++)
                {
                    if (k[m] == crr[m])
                    {
                        v++;
                    }
                }
                if (v == k.size()) // coincide
                {
                    coincide = true;
                    break;
                }
            }
        }
        if (coincide == false)
        {
            ov.push_back(crr);
        }
        backtracking(i + 1, iv, ov, crr);
        crr.pop_back();
    }
}
int main()
{
    vector<long unsigned int> iv, crr;
    vector<vector<long unsigned int>> ov;
    long unsigned int n{0};

    cin >> n;
    for (long unsigned int i{0}; i < n; i++)
    {
        long unsigned int tem{0};
        cin >> tem;
        iv.push_back(tem);
    }
    ov.push_back({iv[0]});
    backtracking(0, iv, ov, crr);

    long unsigned int sum = 0;
    for (auto i : ov)
    {
        sum += funA(i);
    }
    cout << sum << endl;
    return 0;
}


int main()
{
    vector<int> iv;
    int n{0}, k{0};
    cin >> n >> k;
    for (int i{0}; i < n; i++)
    {
        int tem;
        cin >> tem;
        iv.push_back(tem);
    }

    int tem = 0;
    while (tem != k)
    {
        if (iv[0] < iv[1])
        {
            tem=0;
            iv.push_back(iv[0]);
            iv.erase(iv.begin());
        }
        else
        {
            tem++;
            iv.push_back(iv[1]);
            iv.erase(iv.begin()+1);
        }
    }
    cout<<iv[0]<<endl;
    return 0;
}

void backtracking(vector<int> &iv, vector<vector<int>> &ov, vector<int> &crr)
{
    if (crr.size() == 3)
    {
        ov.push_back(crr);
        return;
    }
    for (int i{0}; i < iv.size(); i++)
    {
        if (find(crr.begin(), crr.end(), iv[i]) == crr.end())
        {
            crr.push_back(iv[i]);
            backtracking(iv, ov, crr);
            crr.pop_back();
        }
        
    }
}

int main()
{
    vector<int> iv{1,2,3}, crr;
    vector<vector<int>> ov;
    backtracking(iv, ov, crr);
    for (auto i : ov)
    {
        for (auto x : i)
        {
            cout << x << " ";
        }
        cout << endl;
    }
    return 0;
}



/**
 * https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/
*/
class Solution
{
public:

    vector<int> twoSum(vector<int> &numbers, int target)
    {
        vector<int> res;
        int n= numbers.size();
                res.push_back(0);
                res.push_back(0);
        for(int i{0};i<n;i++)
        {
            auto it = lower_bound(numbers.begin() +i +1, numbers.end(), target - numbers[i]);
            if(it == numbers.end())
            {
                continue;
            }
            else
            {
                if(numbers[i]+numbers[it-numbers.begin()] == target)
                {
                res[0]=i+1;
                res[1]=it-numbers.begin() +1;
                break;
                }
                else
                {
                    continue;
                }
            }
        }
        return res;
    }
};



/**
 * https://leetcode.com/problems/maximum-number-of-coins-you-can-get
*/
class Solution
{
public:
    int maxCoins(vector<int> &piles)
    {
        int sum{0};
        int n = piles.size();
        sort(piles.begin(),piles.end());
        for(int i{n/3};i<n;i+=2)
        {
            sum+=piles[i];
        }
        return sum;
    }
};

/**
 * https://leetcode.com/problems/lemonade-change/
*/
class Solution
{
public:
    bool lemonadeChange(vector<int> &bills)
    {
        int m5{0},m10{0};
        for(auto x:bills)
        {
            if(5==x)
            {
                m5++;
            }
            else if(10==x)
            {
                if(m5 >=1)
                {
                    m10++;
                    m5--;
                }
                else
                {
                    return false;
                }
            }
            else // 20
            {
                if(m10>=1)
                {
                    m10--;
                    if(m5>=1)
                    {
                        m5--;
                    }
                    else
                    {
                        return false;
                    }

                }
                else
                {
                    if(m5>=3)
                    {
                        m5-=3;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};




/**
 * https://leetcode.com/problems/array-partition/
*/
class Solution
{
public:
    int arrayPairSum(vector<int> &nums)
    {
        int res{0};
        sort(nums.begin(), nums.end());
        for (int i{0}; i < nums.size(); i+=2)
        {
            res+=nums[i];
        }
        return res;
    }
};


/**
 * https://leetcode.com/problems/minimum-number-of-days-to-make-m-bouquets/description/
*/
class Solution
{
public:
    bool canMake(int d, int m, int k, vector<int> &bloomDay)
    {
        int cnt_k{0}, cnt_m{0};
        for (auto i : bloomDay)
        {
            if (i <= d) // the flower bloomed
            {
                cnt_k++;
            }
            else
            {
                cnt_k = 0;
            }
            if (k == cnt_k)
            {
                cnt_m++;
                cnt_k = 0;
            }
        }
        if (cnt_m >= m)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int minDays(vector<int> &bloomDay, int m, int k)
    {
        int low, hig, mid, max{bloomDay[0]};
        for (auto i : bloomDay)
        {
            if (i > max)
            {
                max = i;
            }
        }
        low = 1;
        hig = max;
        while (low <= hig)
        {
            mid = low + (hig - low) / 2;
            // traverse through bloomDay to check whether we can make m bouquet k flower or not
            if (canMake(mid, m, k, bloomDay))
            {
                if (!canMake(mid - 1, m, k, bloomDay))
                {
                    return mid;
                }
                else
                {
                    hig = mid - 1;
                }
            }
            else
            {
                low = mid + 1;
            }
        }
        return -1;
    }
};
int main()
{
    Solution s;

    return 0;
}

/**
 * https://leetcode.com/problems/first-bad-version/
 */

// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

class Solution
{
public:
    int firstBadVersion(int n)
    {
        if (1 == n)
        {
            return 1;
        }
        int low{1}, high{n}, mid, ans;
        while (low <= high)
        {
            mid = low + (high - low) / 2;
            if (isBadVersion(mid))
            {
                if (!isBadVersion(mid - 1))
                {
                    break;
                }
                else
                {
                    high = mid - 1;
                }
            }
            else
            {
                low = mid + 1;
            }
        }
        return mid;
    }
};


int n{0};
bool check(vector<string> &crr)
{
    int arr[][]

}
void backtracking(vector<string> &crr, vector<vector<string>> &res)
{
    if (crr.size() == n) // enough row for check
    {
        if(check() == true)
        {
            res.push_back(crr);
        }
    }
    else // create next row
    {
        for (int col{0}; col < n; col++)
        {
            string s(n, '.');
            s[col] = 'Q';
            crr.push_back(s);
            backtracking(crr, res);
            crr.pop_back();
        }
    }
}
int main()
{
    vector<vector<string>> res;
    vector<string> crr;
    cout << "input n" << endl;
    cin >> n;
    backtracking(crr, res);
    for (auto i : res)
    {
        int t{0};
        for (auto x : i)
        {
            t++;
            cout << x << endl;
            if (t == 4)
            {
                cout << "-------------------" << endl;
            }
        }
    }
    return 0;
}


/**
 * https://www.codechef.com/problems/TRICOIN
*/

int N{0};
vector<int> vCoin;
int main()
{
    // cout<<"input N: "<<endl;
    cin>>N;
    // cout<<"input coin"<<endl;
    int tem{0};
    for(int i{0};i<N;i++)
    {
        cin>>tem;
        vCoin.push_back(tem);
    }
    for(int i{0};i<N;i++)
    {
        int sum{0};
        int x{0};
        while(sum <= vCoin[i])
        {
            x++;
            sum +=x;
        }
        
    
        cout<<x-1<<endl;
    }
    return 0;
}



class Solution
{
public:
    int orangesRotting(vector<vector<int>> &grid)
    {

        int count{0};
        queue<pair<int, int>> q;
        const unsigned long nc{grid[0].size()};
        const unsigned long nr{grid.size()};
        int dr[] = {1, -1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        int queuePushed[nr][nc];
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                queuePushed[i][j] = -1;
            }
        }
        bool change{true};
        int minute{0};
        while (change == true)
        {
            for (int i{0}; i < nr; i++)
            {
                for (int j{0}; j < nc; j++)
                {
                    queuePushed[i][j] = -1;
                }
            }
            change = false;
            for (int m{0}; m < nr; m++)
            {
                for (int j{0}; j < nc; j++)
                {
                    if ((queuePushed[m][j] == -1) && (grid[m][j] == 2))
                    {
                        queuePushed[m][j] = 0;
                        for (int i{0}; i < 4; i++)
                        {
                            int tr = m + dr[i];
                            int tc = j + dc[i];
                            if ((tr >= 0) && (tr <= (nr - 1)) && (tc >= 0) && (tc <= (nc - 1)) &&
                                (queuePushed[tr][tc] == -1) &&
                                (grid[tr][tc] == 1))
                            {
                                grid[tr][tc] = 2;
                                queuePushed[tr][tc] = 0;
                                change = true;
                            }
                        }
                    }
                }
            }
            minute++;
        }
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                if (grid[i][j] == 1)
                {
                    return -1;
                }
            }
        }
        return minute-1;
    }
};
int main()
{
    Solution s;
    vector<vector<int>> v{{2, 1, 1}, {1, 1, 0}, {0, 1, 1}};

    cout << s.orangesRotting(v) << endl;
    return 0;
}




/**
 * https://leetcode.com/problems/nearest-exit-from-entrance-in-maze/
*/
class Solution
{
public:
    int nearestExit(vector<vector<char>> &maze, vector<int> &entrance)
    {
        {
            int count{0};
            queue<pair<int, int>> q;
            const unsigned long nc{maze[0].size()};
            const unsigned long nr{maze.size()};
            // cout<<"nr nc: "<<nr<<" "<<nc<<" "<<endl;
            int dr[] = {1, -1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            int queuePushed[nr][nc];
            for (int i{0}; i < nr; i++)
            {
                for (int j{0}; j < nc; j++)
                {
                    queuePushed[i][j] = -1;
                }
            }
            // cout<<"BFS from root: "<<i<<" "<<j<<endl;
            q.push({entrance[0], entrance[1]});
            queuePushed[entrance[0]][entrance[1]] = 0;
            while (!q.empty())
            {
                pair<int, int> s{q.front()};
                q.pop();
                for (int i{0}; i < 4; i++)
                {
                    int tr = s.first + dr[i];
                    int tc = s.second + dc[i];
                    if ((tr >= 0) && (tr <= (nr - 1)) && (tc >= 0) && (tc <= (nc - 1)) &&
                        (queuePushed[tr][tc] == -1) &&
                        (maze[tr][tc] == '.'))
                    {
                        // cout<<"push: "<<tr<<" "<<tc<<" to queue"<<endl;
                        q.push({tr, tc});
                        queuePushed[tr][tc] = queuePushed[s.first][s.second] + 1;
                        if ((tr == 0) || (tr == (nr - 1)) || (tc == 0) || (tc == nc - 1))
                        {
                            return queuePushed[tr][tc];
                        }
                    }
                }
            }
            return -1;
        }
    }
};
int main()
{
    Solution s;
    vector<vector<char>> v{{'+','+','.','+'},{'.','.','.','+'},{'+','+','+','.'}};
    vector<int> ec{1,2};
    cout<<s.nearestExit(v,ec)<<endl;
    return 0;
}


/**
 * https://leetcode.com/problems/detect-cycles-in-2d-grid/
*/
class Solution
{
public:
    bool containsCycle(vector<vector<char>> &grid)
    {
        int count{0};
        stack<pair<int, int>> q;
        
        const unsigned long nc{grid[0].size()};
        const unsigned long nr{grid.size()};
        bool stackPushed[nr][nc];
        // cout<<"nr nc: "<<nr<<" "<<nc<<" "<<endl;
        int dr[] = {1, -1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                stackPushed[i][j] = false;
            }
        }
        
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                char rootVal{grid[i][j]};
                // reset stackPushed when check new root

                // dfs from root ij
                // cout<<"DFS from root: "<<i<<" "<<j<<endl;
                q.push({i, j});
                stackPushed[i][j] = true;
                while (!q.empty())
                {
                    pair<int, int> s{q.top()};
                    q.pop();
                    for (int i{0}; i < 4; i++)
                    {
                        int tr = s.first + dr[i];
                        int tc = s.second + dc[i];
                        if ((tr >= 0) && (tr <= (nr - 1)) && (tc >= 0) && (tc <= (nc - 1)) &&
                            (stackPushed[tr][tc] == false) &&
                            (grid[tr][tc] == rootVal))
                        {
                            // check 4 arround point, if oke --> push to queue
                            q.push({tr, tc});
                            stackPushed[tr][tc] = true;
                            // check 4 arround just pushed point, if already pushed to stack --> loop
                            for (int i{0}; i < 4; i++)
                            {
                                int checkR = tr + dr[i];
                                int checkC = tc + dc[i];
                                if ((checkR >= 0) && (checkR <= (nr - 1)) && (checkC >= 0) && (checkC <= (nc - 1)) &&
                                    (stackPushed[checkR][checkC] == true) &&
                                    (grid[checkR][checkC] == rootVal) &&
                                    (checkR != s.first) && (checkC != s.second))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }
};
int main()
{
    Solution s;
    vector<vector<char>> v{{'a', 'b', 'b'}, {'b', 'z', 'b'}, {'b', 'b', 'a'}};
    cout << s.containsCycle(v) << endl;
    return 0;
}



/**
 * https://leetcode.com/problems/number-of-islands/
*/
class Solution
{
public:
    int numIslands(vector<vector<char>> &grid)
    {
        int count{0};
        stack<pair<int, int>> q;
        const unsigned long nc{grid[0].size()};
        const unsigned long nr{grid.size()};
        // cout<<"nr nc: "<<nr<<" "<<nc<<" "<<endl;
        int dr[] = {1, -1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        bool visited[nr][nc];
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                visited[i][j] = false;
            }
        }
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                if ((grid[i][j] == '1') && (visited[i][j] == false))
                {
                    // cout<<"DFS from root: "<<i<<" "<<j<<endl;
                    q.push({i, j});
                    visited[i][j] = true;
                    while (!q.empty())
                    {
                        pair<int, int> s{q.top()};
                        q.pop();
                        for (int i{0}; i < 4; i++)
                        {
                            int tr = s.first + dr[i];
                            int tc = s.second + dc[i];
                            if ((tr >= 0) && (tr <= (nr - 1)) && (tc >= 0) && (tc <= (nc - 1)) &&
                                (visited[tr][tc] == false) &&
                                (grid[tr][tc] == '1'))
                            {
                                // cout<<"push: "<<tr<<" "<<tc<<" to queue"<<endl;
                                q.push({tr, tc});
                                visited[tr][tc] = true;
                            }
                        }
                    }
                    count++;
                }
                else
                {
                    // cout<<"visited or water"<<endl;
                    continue;
                }
            }
        }

        return count;
    }
};
int main()
{
    Solution s;
    vector<vector<char>> v{{'1', '1', '0', '0', '0'}, {'1', '1', '0', '0', '0'}, {'0', '0', '1', '0', '0'}, {'0', '0', '0', '1', '1'}};
    cout << s.numIslands(v) << endl;
    return 0;
}



/*
https://leetcode.com/problems/flood-fill/description/
*/

class Solution
{
public:
    vector<vector<int>> floodFill(vector<vector<int>> &image, int sr, int sc, int color)
    {

        queue<pair<int, int>> q;
        int orColor{image[sr][sc]};
        const unsigned long nc{image[0].size()};
        const unsigned long nr{image.size()};
        // cout<<"nr "<<nr<<"; nc "<<nc<<endl;
        int dr[] = {1, -1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        bool visited[nr][nc];
        for (int i{0}; i < nr; i++)
        {
            for (int j{0}; j < nc; j++)
            {
                visited[i][j] = false;
            }
        }
        q.push({sr, sc});
        visited[sr][sc] = true;
        while (!q.empty())
        {
            pair<int,int> s{q.front()};
            image[s.first][s.second]=color;
            q.pop();
            for (int i{0}; i < 4; i++)
            {
                int tr = s.first + dr[i];
                int tc = s.second + dc[i];
                if ((tr >= 0) && (tr <= (nr - 1)) && (tc >= 0) && (tc <= (nc - 1)) && 
                (visited[tr][tc] == false)&&
                (image[tr][tc] == orColor))
                {
                    q.push({tr,tc});
                    visited[tr][tc]=true;
                }
                  
            }
        }
        return image;
    }
};

int main()
{
    Solution s;
    vector<vector<int>> v{{1,1,1},{1,1,0},{1,0,1}};
    s.floodFill(v,1,1,2);
    for(auto i:v)
    {
        for(auto x:i)
        {
            cout<<x<<" ";
        }
        cout<<endl;
    }
    return 0;
}

/*
DFS implementation - stack
*/

const int N{7};
vector<vector<int>> adj{{1, 2}, {0, 3, 4, 6}, {0, 4, 6, 5}, {1}, {1, 2}, {2}, {1, 2}};
int main()
{
    stack<int> s;
    bool visited[N];
    for (int i{0}; i < N; i++)
    {
        visited[i] = false;
    }
    s.push(0);
    visited[0] = true;
    while (!s.empty())
    {
        int tem(s.top());
        s.pop();
        cout << "checked " << tem << endl;
        for (auto i : adj[tem])
        {
            if (!visited[i])
            {
                s.push(i);
                visited[i] = true;
            }
        }
    }

    return 0;
}




/*
BFS implementation - queue
*/
const int N{7};
vector<vector<int>> adj{{1, 2}, {3, 4,6}, {4, 6, 5}, {1}, {1,2}, {2}, {1,2}};
int main()
{
    queue<int> q;
    bool visited[N];
    for(int i{0};i<N;i++)
    {
        visited[i] = false;
    }
    visited[0] = true;
    q.push(0);
    while(!q.empty())
    {
        int s{q.front()};
        cout<<"checked node "<<s<<endl;
        q.pop();
        for(auto i:adj[s])
        {
            if(visited[i] == false)
            {
                q.push(i);
                visited[i] = true;
            }
        }

    }
    cout << "main" << endl;
    return 0;
}


class Solution
{
public:
    void topKFrequent(vector<string> &words, int k)
    {
        map<string, int> m;
        // vector<bool> v;
        // for(int j{0};j<k;j++)
        // {
        //     v.push_back(false);
        // }
        while (words.size() != 0)
        {
            // int count{0};
            string s{words[0]};
            for (int i{0};i<words.size();i++)
            {
                if (s == words[i])
                {
                    words.erase(words.begin()+i);
                    // count++;
                    m[s]++;
                }
            }
            
        }
        auto it = m.begin();

        // Iterate through the map and print the elements
        while (it != m.end())
        {
            cout << "Key: " << it->first
                 << ", Value: " << it->second << endl;
            ++it;
        }
    }
};
int main()
{
    Solution s;
    vector<string> v{"the","day","is","sunny","the","the","the","sunny","is","is"};
    s.topKFrequent(v,2);
    return 0;
}



void show(const vector<int> &v)
{
    for (auto i : v)

    {
        cout << i << " ";
    }
    cout<<endl;
}
int main()
{
    vector<int> v;
    int n{5};
    for(int i{1};i<=n;i++)
    {
        v.push_back(n);
    }
    // cout<<"v is "<<endl;
    // show(v);
    int k{2};
    auto it = v.begin();
    int dif{0};
    while (v.size() != 1)
    {
        dif = (it - v.begin() + k - 1) % (v.size());
        it = v.begin() + dif;
        // cout<<"done diff"<<endl;
        v.erase(it);
        show(v);
    }
    cout<<v[0]<<endl;
    return 0;
}


class Solution
{
public:
    vector<int> mostCompetitive(vector<int> &nums, int k)
    {
        vector<int> res; // result vector
        set<int> s;
        for(auto i:nums)
        {
            s.insert(i);
        }
        int start{0};
        auto it = s.begin();
        while (res.size() != k)
        {
            auto pos = find(nums.begin() + start, nums.end(), *it);
            if (pos != nums.end())
            {
                // cout << "found v[n] == " << v[n] << endl;
                if ((pos - nums.begin() + k - (res.size()+1)) <= (nums.size() - 1))
                {
                    // cout<<"enough space"<<endl;
                    start = pos - nums.begin() + 1;
                    res.push_back(*it);
                    it = s.begin();
                }
                else
                {
                    // cout<<"NOT enough space"<<endl;
                    it++;
                }
            }
            else
            {
                it++;
            }
        }
        return res;
    }
};
int main()
{
    cout << "main start" << endl;
    Solution s;
    vector<int> v{71,18,52,29,55,73,24,42,66,8,80,2};
    vector<int> res;
    res = s.mostCompetitive(v, 3);
    for (auto x : res)
    {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
int main(int argc, char *argv[])
{
    string s;
    int k;
    cin >> s >> k;

    int len = s.size();
    for (int i = 0; i < (len - (len % k)); i++)
    {
        if ((i != 0) && ((i % k) == 0))
        {
            k = -k;
        }
        char c = s[i] - 'a';
        c += (26 + k);
        c %= 26;
        s[i] = 'a' + c;
    }
    cout << s <<endl;
    return 0;
}

#include <iostream>
using namespace std;

int N;             // Number of executable code data
int M{4};          // Number of virus data
int A[20000 + 10]; // Executable code data
int B[10 + 10];    // Virus data

int sol; // Correct answer

void InputData(void)
{
    int i;

    cin >> N >> M;

    for (i = 0; i < N; i++)
    {
        cin >> A[i];
    }

    for (i = 0; i < M; i++)
    {
        cin >> B[i];
    }
}

void OutputData(void)
{
    cout << sol << endl;
}

void sortArr(int *arr)
{
    for (int i{M-2}; i >=0; i--)
    {
        for (int j{0}; j <= i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int tem = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tem;
            }
        }
    }
}
int Find(int start)
{
    int ret{1};
    int arr[M];
    for (int i{0}; i < M; i++)
    {
        arr[i] = A[start + i];
    }
    sortArr(arr);
    for (int i = 1; i < M; i++)
    {
        if ((arr[i] - B[i]) != (arr[0] - B[0]))
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

void Solve(void)
{
    for (int i = 0; i <= N - M; i++)
    {
        sol += Find(i);
    }
}

int main(void)
{
    InputData(); // Input
    sortArr(B);
    Solve(); // Problem solving
    OutputData(); // Output

    return 0;
}


int N;//Number of sponsored goods
int D[100000 + 10];//Preference
int sol = -30001;//Maximum preference of the first method

void InputData(){
	cin >> N;
	for(int i=0;i<N;i++){
		cin >> D[i];
	}
}

void Solve()
{
    int sum{0};
    sol = D[0];
	for(int i{0};i<N;i++)
    {
        sum+=D[i];
        if(sum > sol)
        {
            sol = sum;
        }
        if(sum<0)
        {
            sum = 0;
        }
    }
}


int main(){
	InputData();//Input function
	Solve();
	cout << sol << endl;//Output
	return 0;
}

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
#include <mqueue.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <memory>
// #define NDEBUG
#include <assert.h>
#include <stdio.h>
#include <time.h>
using namespace std;

int N;              // Number of sponsored goods
int D[100000 + 10]; // Preference
int sol = -30001;   // Maximum preference of the first method

void InputData()
{
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        cin >> D[i];
    }
}

void Solve()
{
    cout << "init vector" << endl;
    vector<vector<int>> arr;
    vector<int> tem;
    for (int i{0}; i < N; i++)
    {
        {
            tem.push_back(0);
        }
    }
    for (int i{0}; i < N; i++)
    {
        arr.push_back(tem);
    }

    int sum{0};
    cout << "Calculate first column" << endl;
    for (int j{0}; j < N; j++)
    {
        sum += D[j];
        arr[0][j] = sum;
    }
    cout << "Done calculate first column" << endl;
    for (int i{1}; i < N; i++)
    {
        for (int j{0}; j < N; j++)
        {
            arr[i][j] = arr[i - 1][j] - D[i - 1];
        }
    }
    cout << "Done calculate all column" << endl;
    sol = arr[0][0];
    cout << "Find  max" << endl;
    for (int i{0}; i < N; i++)
    {
        for (int j{0}; j < N; j++)
        {
            if (arr[i][j] > sol)
            {
                sol = arr[i][j];
            }
        }
    }
}

int main()
{
    InputData(); // Input function
    Solve();
    cout << sol << endl; // Output
    return 0;
}

int fibo(int n)
{
    int a=0,b=1, sum = 0;
    if(n<0)
    {
        cout<<"wrong n"<<endl;
    }
    else if( 0==n )
    {
        return 0;
    }
    else if(1 == n)
    {
        return 1;
    }
    else
    {
        for(int j{2};j<=n;j++)
        {
            sum = a + b;
            a = b;
            b = sum;
        }
    }
    return sum;
}
int main()
{
    cout<<fibo(9)<<endl;
    return 0;
}



int arr[] = {7, 5, 3, 1};
int s{17};
int myCnt{0};
int main()
{
    int sum{0};
    for (int j{0}; j < 4; j++)
    {
        while ((sum+arr[j]) <= s)
        {
            sum+=arr[j];
            myCnt++;
        }
    }
    cout<<myCnt<<endl;
    return 0;
}


int arr0 = 1, arr1 = 2;

int fun(int n)
{
    if(n==0) return arr0;
    if(n==1) return arr1;
    return (fun(n-1)+fun(n-2));
}
int main()
{
    cout<<"fun9== "<<fun(9)<<endl;
    return 0;
}




int N;              // Number of sponsored goods
int D[100000 + 10]; // Preference
int sol = -30001;   // Maximum preference of the first method

void InputData()
{
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        cin >> D[i];
    }
}

void Solve()
{
    for (int i{0}; i < N; i++)
    {
        int lSum = D[i];
        int tSum = D[i];
        for (int k{i}; k < (N - 1); k++)
        {
            tSum += D[k + 1];
            if (tSum > lSum)
            {
                lSum = tSum;
            }
        }
        if (0 == i)
        {
            sol = lSum;
        }
        if (lSum > sol)
        {
            sol = lSum;
        }
    }
}

int main()
{
    InputData(); // Input function
    Solve();
    cout << sol << endl; // Output
    return 0;
}

// define base class
class Point
{
protected:
    int m_x, m_y;

public:
    Point(int x, int y) : m_x{x}, m_y{y}
    {
        cout << "Point constructor ,address == " << this << endl;
    };
    Point(Point &p)
    {
        cout << "Point copy constructor ,address == " << this << endl;
        m_x = p.m_x;
        m_y = p.m_y;
    }
    virtual void mDisplay()
    {
        cout << "mDisplay of base class running..." << endl;
        cout << "m_x: " << m_x << " and m_y: " << m_y << endl;
    }
    // virtual void increase() = 0;
    virtual ~Point()
    {
        cout << "Point destructor ,address == " << this << endl;
    }
};

// define derived class
class ColorPoint : public Point
{
private:
    int m_color;

public:
    ColorPoint(int x, int y, int c) : Point(x, y), m_color{c}
    {
        cout << "ColorPoint constructor ,address == " << this << endl;
    };

    void mDisplay()
    {
        cout << "mDisplay of derived class running..." << endl;
        cout << "m_x: " << m_x << " and m_y: " << m_y << " and m_color: " << m_color << endl;
    }
    void increase()
    {
        cout << "increase" << endl;
    }
    ~ColorPoint()
    {
        m_color = 0;
        cout << "ColorPoint destructor ,address == " << this << endl;
    }
};

void show(Point p)
{
    p.mDisplay();
}

void throwException()
{
    cout << "throwException" << endl;
    ColorPoint p(1,2,3);
    throw p;
    cout << "expect not show" << endl;
}
int main()
{
    cout<<"main fun start"<<endl;
    try
    {
        throwException();
    }
    catch (Point& x)
    {
        cout << "catch exception type Point" << endl;
        x.mDisplay();
    }
    cout << "end of main" << endl;

    return 0;
}
/*
throwException
constructor 1
constructor 2
catch exception
display
destructor 2
end of main
des 1
*/


int arr[] = {4,1,2,1,3};
int N{5};
int main()
{
    for(int n{N - 1}; n > 0; n--)
    {
        for(int j{0}; j < n; j++)
        {
            if(arr[j] > arr[j+1])
            {
                int tem = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tem;
            }
        }
    }
    for(int i{0};i<N;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    return 0;
}




int arr[3][2] = {{5,0}, {2,0}, {1,0}};
int acc{18};
int tem{0};
int main()
{
    for(int i =0; i<3;i++)
    {
        cout<<"tem == "<<tem<<endl;
        while(acc >=(tem+arr[i][0]))
        {
            arr[i][1]++;
            tem+=arr[i][0];
        }
    }
    for(int i{0};i<3;i++)
    {
        cout<<arr[i][0]<<" "<<arr[i][1]<<endl;
    }
    return 0;
}


class Point
{
    private:
    int x;
    public:
    Point(int tem) : x{tem}
    {
        cout<<"Point's constructor"<<endl;
    }
    ~Point()
    {
        cout<<"Point's destructor"<<endl;
    }
    int show()
    {
        cout<<"x =="<<x<<endl;
        return x;
    }
};
int main()
{
    cout<<"start main"<<endl;
    Point x(1);
    assert(x.show() != 1);
    cout<<"end main"<<endl;
    return 0;
}


// string crashSe{"2|2|2|2|2|1705311287873"};
string crashSe{"01|2345678|"}; //crashSe.length() == 10, from 0 to 9 , 10 is nullTerminated character

char c{'|'};
int main()
{    
    string res = "";
    if(crashSe.find_last_of(c) != string::npos)
    {
        // 0 <= crashSe.find_last_of(c) <= (crashSe.length() - 1)
        res = crashSe.substr(crashSe.find_last_of(c) + 1); // --> nullTerminated character
    }
    else
    {
        cout<<"not found"<<endl;
    }
    cout<<"res == "<<res<<endl;
    return 0;
}





// define base class
class Point
{
protected:
    int m_x, m_y;

public:
    Point(int x, int y) : m_x{x}, m_y{y} 
    {
        cout<<"Point constructor"<<endl;
    };
    Point(Point &p)
    {
        m_x = p.m_x;
        m_y = p.m_y;
    }
    virtual void mDisplay()
    {
        cout << "mDisplay of base class running..." << endl;
        cout << "m_x: " << m_x << " and m_y: " << m_y << endl;
    }
    virtual void increase() = 0;
    virtual ~Point()
    {
        m_x = 0;
        m_y = 0;
        cout<<"Point destructor"<<endl;
    }
};

// define derived class
class ColorPoint : public Point
{
private:
    int m_color;

public:
    ColorPoint(int x, int y, int c) : Point(x, y), m_color{c} 
    {
        cout<<"ColorPoint constructor"<<endl;
    };
    void mDisplay()
    {
        cout << "mDisplay of derived class running..." << endl;
        cout << "m_x: " << m_x << " and m_y: " << m_y << " and m_color: " << m_color << endl;
    }
    void increase()
    {
        cout << "increase" << endl;
    }
    ~ColorPoint()
    {
        m_color = 0;
        cout<<"ColorPoint destructor"<<endl;
    }
};
int main()
{
    Point* p = new ColorPoint(1,2,3);
    p->mDisplay();
    delete p;
    p = nullptr;
    return 0;
}



-------------------------------------------------------------------

const char *path = "/home/worker/delete/Cpp/ecallApp.exe";
const char *appId = "ecallApp";
int mainSocFD{-1}, subSocFD{-1}, portNum{0};
sockaddr_in mainSocAddr, subSocAddr;
socklen_t clientLength{0};
char buffer[256];
int main()
{
    // int socket(int domain, int type, int protocol);
    mainSocFD = socket(AF_INET, SOCK_STREAM, 0);
    if (mainSocFD < 0)
    {
        cout << "socket fail" << endl;
    }
    else
    {
        cout<<"mainSocFD == "<<mainSocFD<<endl;
    }
    // struct sockaddr_in
    // {
    //     sa_family_t sin_family;  /* AF_INET */
    //     in_port_t sin_port;      /* Port number */
    //     struct in_addr sin_addr; /* IPv4 address */
    // };
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    bzero((void *)&mainSocAddr, sizeof(mainSocAddr));
    portNum = 22222;
    mainSocAddr.sin_family = AF_INET;
    mainSocAddr.sin_addr.s_addr = INADDR_ANY;
    mainSocAddr.sin_port = htons(portNum);
    if (bind(mainSocFD, (struct sockaddr *)&mainSocAddr, sizeof(mainSocAddr)) < 0)
    {
        cout << "ERROR on binding, errno == "<<errno<< endl;
    }
    listen(mainSocFD, 5);
    clientLength = sizeof(subSocAddr);
    subSocFD = accept(mainSocFD, (struct sockaddr *)&subSocAddr, &clientLength);
    if (subSocFD < 0)
    {
        cout << "ERROR on accept" << endl;
    }

    cout << "get connection from IP == " << inet_ntoa(subSocAddr.sin_addr) << " and port == " << ntohs(subSocAddr.sin_port) << endl;

    send(subSocFD, "Today is Thurday", 17, 0);
    bzero(buffer, 256);
    int n = read(subSocFD, buffer, 255);
    if (n < 0)
    {
        cout << "error when read" << endl;
    }
    cout << buffer << endl;

    close(subSocFD);
    close(mainSocFD);
    return 0;
}


void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     // setup the host_addr structure for use in bind call
     // server byte order
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

     // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
     // bind() passes file descriptor, the address structure, 
     // and the length of the address structure
     // This bind() call will bind  the socket to the current IP address on port, portno
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
     listen(sockfd,5);

     // The accept() call actually accepts an incoming connection
     clilen = sizeof(cli_addr);

     // This accept() function will write the connecting client's address info 
     // into the the address structure and the size of that structure is clilen.
     // The accept() returns a new socket file descriptor for the accepted connection.
     // So, the original socket file descriptor can continue to be used 
     // for accepting new connections while the new socker file descriptor is used for
     // communicating with the connected client.
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     printf("server: got connection from %s port %d\n",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));


     // This send() function sends the 13 bytes of the string to the new socket
     send(newsockfd, "Hello, world!\n", 13, 0);

     bzero(buffer,256);

     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

     close(newsockfd);
     close(sockfd);
     return 0; 
}



const char *path = "/home/worker/delete/Cpp/ecallApp.exe";
const char *appId = "ecallApp";
using namespace std;
uint32_t key = 77U;
int msgQueueID = 0;
std::string data{""};
int msgType{0};


int main()
{
    MyMsgbuf* mb = new MyMsgbuf();
    unsigned int mPID{0};
    mPID = fork();
    if (mPID == 0)
    {
        cout << "this is sldd process" << endl;
        cout<<"fill your msgType"<<endl;
        cin>>msgType;
        cout<<"Fill your data"<<endl;
        cin>>data;
        msgQueueID = msgget(key, IPC_CREAT|0644);
        if (msgQueueID < 0)
        {
            cout << "msgget fail" << endl;
        }
        mb->mtype = msgType;
        strncpy(mb->mtext,data.c_str(), 30);
        if (msgsnd(msgQueueID, (void*)mb, sizeof(mb->mtext),IPC_NOWAIT)  < 0)
        {
            cout << "msgsnd fail" << endl;
        }
    }
    else
    {
        cout << "Loading ecallApp" << endl;
        usleep(6000000);
        execl(path, path, appId, NULL);
    }
    delete mb;
    return 0;
}



unsigned int volumn(0);
#define BUF_SIZE 1024
#define SHM_KEY 0x1234
#define EXIT_CMD 0
const char *path = "/home/worker/delete/Cpp/ecallApp.exe";
const char *appId = "ecallApp";
int cmd{EXIT_CMD};

int main()
{
    unsigned int mPid{0};
    mPid = fork();
    if (mPid == 0) // parent process
    {
        cout << "This is sldd process" << endl;
        cout << "Fill you cmd:" << endl;
        cin >> cmd;
        while (cmd != EXIT_CMD)
        {
            cout << "Fill you cmd:" << endl;
            cin >> cmd;
            SlddSender sls;
            sls.sendNumber(cmd);
        }
    }
    else // child process
    {
        cout << "Load app" << endl;
        execl(path, path, appId, NULL);
    }

    return 0;
}

#define NO_ERROR 0
#define ENOENT 2
#define MKDIR_ERROR -1
#define LOG_BUF_MAX 2048U
#define LOG_ENTRY_MAX 20U
#define FILE_LIST_MAX 200U
#define READ_DATA_MAX 500
#define OVERVIEW_READ_DATA_MAX 5
#define ECALL_STATE_LINE_NUMBER 6
#define PSAP_MAX_AUTO_FILE_COUNT 5
#define PSAP_REMOVE_TYPE_ALL 0
#define PSAP_REMOVE_TYPE_MANUAL 1
#define NEW_WORD_SIZE 100U
#define TMP_PATH_LENGTH 2048U
#define APP_NUMBER 4
#define TMP_LENGTH 100U
#define MKDIR_MODE 484U
#define REPLACE_TMP "replace.tmp"
#define ECALL_STATE_INCOMPLETE "2"
#define ECALL_STATE_COMPLETE "1"
#define WORD_SIZE 100U
#define MAX_UINT64 18446744073709551615U

string filepathBMW = "/log/encrypted/eCall/bmwecall/log/";
string filepathEU = "/log/encrypted/eCall/euecall/log/";
string filepathERA = "/log/encrypted/eCall/eraecall/log/";
string filepathPsap = "/log/encrypted/eCall/psapecall/log/";
std::ofstream ofile;
string filename = "/home/worker/delete/Cpp/psapManual";
int main()
{
    ofile.open(filename.c_str(), std::ios::in);
    if (ofile.is_open() == false)
    {
        cout<<"open fail"<<endl;
    }
    else // parsedFileCount is over 20, do not make new log file.
    {
        cout<<"open successfully"<<endl;
        ofile.close();
    } 

    ofile.open(filename.c_str(), std::ios::in);
    if (ofile.is_open() == false)
    {
        cout<<"open fail"<<endl;
    }
    else // parsedFileCount is over 20, do not make new log file.
    {
        cout<<"open successfully"<<endl;
        ofile.close();
    }    

    return 0;
}




int32_t checkLogDirWAVE()
{
    cout << "Check log directory WAVE" << endl;
    const char *path = NULL;
    char tmp_path[2048];
    const char *tmp = NULL;
    int64_t len = 0;
    int32_t ret = 0;
    int32_t direResult = 0;
    int32_t resultCheckDir = NO_ERROR;
    mode_t mkdirMode = 0744U;

    for (int8_t i = 0; i < 4; i++)
    {
        cout << endl
             << endl
             << endl;
        // Initialize pointer
        path = NULL;
        tmp = NULL;
        memset(tmp_path, 0, sizeof(tmp_path));

        if (i == 0)
            path = filepathBMW.c_str();
        else if (i == 1)
            path = filepathERA.c_str();
        else if (i == 2)
            path = filepathEU.c_str();
        else if (i == 3)
            path = filepathPsap.c_str();
        else
            cout << "path error" << endl;

        cout << "path == " << path << endl;

        tmp = path;

        if (path == NULL || strnlen(path, 2048U) >= 2048U)
        {
            cout << "path is null or path is too long " << endl;
            resultCheckDir = -ENOENT;
            continue;
        }

        while ((tmp = (const char *)memchr(tmp, '/', strnlen(tmp, 100U))) != NULL)
        {
            len = tmp - path;
            tmp++;

            if (len <= 0 || len >= 2048)
            {
                cout << "len is out of bound: " << len << endl;
                continue;
            }
            uint64_t lenUint64 = 0U;

            if (len > 0)
            {
                lenUint64 = static_cast<uint64_t>(len);
            }

            strncpy(tmp_path, path, lenUint64);
            cout << "temp_path = " << tmp_path << endl;
            tmp_path[lenUint64] = 0x00;

            cout << "mkdir with tmp_path == " << tmp_path << endl;
            /*
            if ((ret = mkdir(tmp_path, mkdirMode)) == -1)
            {
                cout<<"ret = "<<ret<<endl;
                if (errno != EEXIST)
                {
                    cout<<"error = "<<errno<<endl;
                    resultCheckDir = -ENOENT;
                    break;
                }
                else
                {
                    cout<<" The directory exists."<<endl;
                }
            }
            */
        } // end while

        cout << "mkdir with path == " << path << endl;
        /*
        direResult = mkdir(path, mkdirMode);

        if (direResult == -1)
        {
            if (errno != EEXIST)
            {
                cout<<"CheckLogDir_Error!!"<<errno<<endl;
                resultCheckDir = -ENOENT;
            }
            else
            {
                cout<<"CheckLogDir_file exists"<<endl;
            }
        }
        else
        {
            cout<<"CheckLogDir_Directory is creadted = "<<filepathPsap<<endl;
        }
        */
    } // end for

    return resultCheckDir;
}

int32_t checkLogDirICON()
{
    cout << "Check log directory ICON" << endl;
    string path = "";
    char tmp_path[TMP_PATH_LENGTH];
    size_t startPos = 0U;
    int32_t ret = 0;
    int32_t mkdirResult = 0;
    int32_t resultCheckDir = NO_ERROR;
    mode_t mkdirMode = MKDIR_MODE;

    for (int8_t i = 0; i < APP_NUMBER; i++) // 4 apps: BMW, ERA, EU, PSAP
    {
        cout << endl
             << endl
             << endl;
        // Initialize
        path = "";
        startPos = 0;
        (void)memset(tmp_path, 0, sizeof(tmp_path));

        if (i == 0)
        {
            path = filepathBMW;
        }
        else if (i == 1)
        {
            path = filepathERA;
        }
        else if (i == 2)
        {
            path = filepathEU;
        }
        else
        {
            path = filepathPsap;
        }
        cout << "path == " << path << endl;

        if ((path == "") || (path.length() >= TMP_PATH_LENGTH))
        {
            cout << "path is null or path is too long " << endl;
            resultCheckDir = -ENOENT;
            continue;
        }

        while (path.find('/', startPos) != string::npos)
        {
            size_t pos = path.find('/', startPos);
            if (pos < MAX_UINT64)
            {
                startPos = pos + 1U; // next turn starts after '/' character
            }

            if ( (0 >= pos) || (pos >= TMP_PATH_LENGTH) )
            {
                cout << "pos is out of bound: " << pos << endl;
            }
            else
            {
                (void)strncpy(tmp_path, path.c_str(), pos);
                cout << "temp_path = " << tmp_path << endl;
                tmp_path[pos] = '\0';

                cout << "mkdir with tmp_path == " << tmp_path << endl;
                // ret = mkdir(tmp_path, mkdirMode);
                // if (ret == MKDIR_ERROR)
                // {
                //     LOG_INFO(PSAPECALL, DLT_STRING("ret = "), DLT_INT(ret));
                //     if (errno != EEXIST)
                //     {
                //         LOG_INFO(PSAPECALL, DLT_STRING("error = "), DLT_INT(errno));
                //         resultCheckDir = -ENOENT;
                //         break;
                //     }
                //     else
                //     {
                //         LOG_INFO(PSAPECALL, DLT_STRING(" The directory exists."));
                //     }
                // }
            }
        } // end while

        cout << "mkdir with path == " << path << endl;
        // mkdirResult = mkdir(path.c_str(), mkdirMode);
        // if (mkdirResult == MKDIR_ERROR)
        // {
        //     if (errno != EEXIST)
        //     {
        //         LOG_INFO(PSAPECALL, DLT_STRING("CheckLogDir_Error!!"), DLT_INT(errno));
        //         resultCheckDir = -ENOENT;
        //     }
        //     else
        //     {
        //         LOG_INFO(PSAPECALL, DLT_STRING("CheckLogDir_file exists"));
        //     }
        // }
        // else
        // {
        //     LOG_INFO(PSAPECALL, DLT_STRING("CheckLogDir_Directory is creadted = "), DLT_STRING(filepathPsap.c_str()));
        // }
    } // end for

    return resultCheckDir;
}

int main()
{
    checkLogDirWAVE();
    cout << "---------------------------" << endl;
    checkLogDirICON();
    return 0;
}


/* continue and break check*/
string filepathPsap = "0123456";
int start = 100; // start position
int main()
{
    string path = "";
    string tmp = "";
    path = filepathPsap;
    tmp = path;

    if (tmp.find('1', start) != string::npos)
    {
        size_t pos = tmp.find('1', start);
        cout << "found at " << pos << endl;
    }
    else
    {
        cout << "not found" << endl;
    }
    // {
    //     size_t pos = tmp.find('/', start);
    //     cout<<"pos = "<<endl;
    //     start = pos + 1;
    // char tmp_path[2048];
    // const char* s = path.c_str();
    // (void)strncpy(tmp_path, path.c_str(), 4U);
    // tmp_path[4] = '\0';
    // cout<<"mkdir with tmp_path  === "<<tmp_path<<endl;
    // }
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
int volumn = 80;
void decreaseVolumn3();
void increaseVolumn9();
int main()
{
    // volumn = 7;
    decreaseVolumn3();
    increaseVolumn9();
    cout<<volumn<<endl;
    return 0;
}




vector<int> Node;
vector<int> visited;
int numNode = 0;
int sttNode = 0;
unsigned long numCube;
vector<unsigned long> cubeV;
unsigned long result = 0;
unsigned long max = 0;
using namespace std;
// void choose(int ind,const vector<int>& iV, vector<int>& crr, vector<vector<int>>& res)
// {

//     cout<<"choose with ind = "<<ind<<endl;
//     for(int i = ind; i < iV.size(); i++)
//     {
//         // if((i > ind)&&(iV[i] == iV[i-1]))
//         // {
//         //     break;
//         // }
//         if(iV[i] > max)
//         {
//             max = iV[i];
//         }
//         crr.push_back(iV[i]);
//         res.push_back(crr);
//         choose(i+1, iV, crr, res);
//         crr.pop_back();
//     }
// }
int main()
{
    cin>>numCube;
    for(unsigned long i = 0;i<numCube;i++)
    {
        unsigned long tem;
        cin>>tem;
        cubeV.push_back(tem);
    }
    Node.push_back(cubeV[0]);
    
    // for(auto size : cubeV)
    // {
    //     if(size > max)
    //     {
    //         max = size;
    //         Node.push_back(size);
    //     }
    // }
    return 0;
}


string s;
int result = 0;
vector<char> letterVector;
int find_maximum_towers(const long unsigned int &start)
{
    bool equal = true;
    map<char, int> numOfLetter;
    // cout<<"check with start  = "<<start<<endl;
    if (start >= s.length())
        return result;

    for (long unsigned int i = 0; i < s.length(); ++i)
    {
        equal = true;
        if (i < start)
        {
            continue;
        }

        numOfLetter[s[i]]+=1;
        // cout<<" numOfLetter "<<s[i]<<" = "<<numOfLetter[s[i]]<<endl;
        int compare = numOfLetter.begin()->second;
        for(auto let : letterVector)
        {
            // cout<<" numOfLetter "<<let<<" = "<<numOfLetter[let]<<endl;
            // cout<<" numOfLetter.begin()->second = "<<numOfLetter.begin()->second<<endl;
            if(numOfLetter[let] != compare)
            {
                equal = false;
                // cout<<"different. Break. Next i in s"<<endl;
                break;
            }
        }

        // cout<<endl;
        // cout<<"equal = "<<equal<<endl;

        if (equal == true)
        {
            
            result++;
            // cout<<"enough. resutl = "<<result<<endl;
            find_maximum_towers(i + 1);
            break;
        }
        

    }
    return result;
}

int main(int argc, char **argv)
{
    // cout << "start main function" << endl;

    cin >> s;
    for (long unsigned int i = 0; i < s.length(); ++i)
    {
        if(find(letterVector.begin(), letterVector.end(), s[i]) == letterVector.end())
        {
            letterVector.push_back(s[i]);
        }
    }
    // for(auto i : letterVector)
    // {
    //     // cout<<i<<" ";
    // }
    // cout<<endl;
    // auto it = numOfLetter.begin();
    // while (it != numOfLetter.end())
    // {
    //     std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    //     ++it;
    // }

    cout << find_maximum_towers(0) << endl;

    return 0;
}


class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
    sort(nums.begin(),nums.end());
    int sum = 0;
    for(int i = 0;(2*i) <= (nums.size() - 1);i++)
    {
        sum+=nums[2*i];
    }
    return sum;
    }
};

vector<int> iv = {0,1,2,3,4,5,7,8,9};
int k = 6;
int b,e,mid;
int main()
{
    b = 0;
    e = iv.size() - 1;
    while(b <= e)
    {
        mid = b + (e-b)/2;
        cout<<"b "<<b<<" e "<<e<<endl;
        cout<<"mid = "<<mid<<endl;
        if(iv[mid] == k)
        {
            cout<<"find "<<k<<" at "<<mid<<endl;
            return 0;
        }
        else if (iv[mid] > k)
        {
            e = mid -1;
        }
        else
        {
            b = mid + 1;
        }
    }
    cout<<"not found "<<k<<endl;
    return 0;
}

class Solution
{
public:
    int M, N;
    int pr, pc;
    int dr[4] = {0, 0, -1, 1};
    int dc[4] = {-1, 1, 0, 0};
    vector<vector<bool>> visited;
    bool hasLoop;

    void dfs(int arow, int acol, const vector<vector<char>> &grid)
    {
        visited[arow][acol] = 1;
        for (int i = 0; i < 4; i++)
        {
            int nr = arow + dr[i];
            int nc = acol + dc[i];
            if (0 <= nr && nr < M && 0 <= nc && nc < N)
            {
                if (visited[nr][nc] == 1 && nr != pr && nc != pc && grid[nr][nc] == grid[arow][acol])
                {
                    hasLoop = true;
                }
                if (visited[nr][nc] == 0 && grid[nr][nc] == grid[arow][acol])
                {
                    pr = arow;
                    pc = acol;
                    dfs(nr, nc, grid);
                }
            }
        }
    }
    bool containsCycle(vector<vector<char>> &grid)
    {
        hasLoop = false;
        M = grid.size();
        N = grid[0].size();
        cout << "init" << endl;
        visited.assign(grid.size(), vector<bool>(grid[0].size()));
        cout << "done init" << endl;
        for (int i{0}; i < M; i++)
        {
            if (hasLoop == true)
                break;
            for (int j = 0; j < N; j++)
            {
                if (hasLoop == true)
                    break;
                if (visited[i][j] == 0)
                {
                    dfs(i, j, grid);
                }
            }
        }
        return hasLoop;
    }
};

vector<vector<char>> grid = {{'a', 'a', 'b', 'b'}, {'b', 'b', 'a', 'a'}, {'a', 'a', 'b', 'b'}, {'b', 'b', 'a', 'a'}};
int main()
{
    Solution s;
    cout << s.containsCycle(grid) << endl;
    return 0;
}



class Solution
{
public:
    vector<vector<int>> floodFill(vector<vector<int>> &image, int sr, int sc, int color)
    {
        int dr[] = {0, 0, -1, 1};
        int dc[] = {-1, 1, 0, 0};
        queue<pair<int, int>> q;
        int m = image.size();    // number of row
        int n = image[0].size(); // number of comumn

        int visited[m][n];
        for(int i = 0;i<m;i++)
        {
            for(int j =0;j<n;j++)
            {
                visited[i][j] = 0;
            }
        }
        int rootColor = image[sr][sc];
        q.push({sr, sc}); // push root node to queue
        visited[sr][sc] = 1;

        while (!q.empty())
        {
            pair<int, int> cPos = q.front();
            image[cPos.first][cPos.second] = color;
            q.pop();
            for (int i = 0; i < 4; i++)
            {
                int nR = cPos.first + dr[i];
                int nC = cPos.second + dc[i];
                if (0 <= nR && nR < m && 0 <= nC && nC < n)
                {
                    cout<<visited[nR][nC]<<endl;
                    if (visited[nR][nC] == 0 && image[nR][nC] == rootColor) // not visited yet & same value
                    {
                        q.push({nR, nC});
                        visited[nR][nC] = 1;
                    }
                }
            }
        }
        return image;
    }
};

int main()
{
    Solution s;
    s.floodFill(iv,1,1,2);
    return 0;
}


// class Circle : public virtual Color
// {
//     private:
//     int r;
//     public:
//     Circle(int temC, int temR) : Color(temC), r{temR}
//     {
//         cout<<"Circle's constructor"<<endl;
//     }
//     void show()
//     {
//         cout<<"Circle's show"<<endl;
//     }
// };

// class Shape : public Circle, public Square
// {
//     private:

//     public:
//     Shape(int temC, int temX, int temR) : Circle(temC,temR), Square(temC,temX), Color(temC)
//     {
//         cout<<"Shape's constructor"<<endl;
//     }
//     void show()
//     {
//         cout<<"Shape's show"<<endl;
//         cout<<"color = "<<c<<endl;
//     }
// };
class Color
{
protected:
    int c;

public:
    Color()
    {
        cout << "Color's constructor" << endl;
    }
    virtual void show() = 0;
};
class Square : public Color
{
private:
    int x;

public:
    Square(int temX) : x{temX}
    {
        cout << "Square's constructor" << endl;
    }
    void show()
    {
        cout << "Square's show" << endl;
    }
};
int main()
{
    Square s(1);
    s.show();
    return 0;
}



int adj[4][4] = {{1,1,0,0}, {0,0,1,1}, {1,1,0,0}, {0,0,1,1}};
int dR[] = {0, 0, -1, 1};
int dC[] = {-1, 1, 0, 0};
int visited[4][4];
queue<pair<int, int>> q;
int islNum = 0;
void BFS(int row, int col)
{
    q.push({row,col});
    visited[row][col] = 1;
    while (!q.empty())
    {
        pair<int, int> crr = q.front();
        cout << "check value " << adj[crr.first][crr.second] << endl;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nR = crr.first + dR[i];
            int nC = crr.second + dC[i];
            // cout << "check nr nc " << nR << " " << nC << endl;
            if (0 <= nR && nR <= 3 && 0 <= nC && nC <= 3 && adj[nR][nC] != 0)
            {
                // cout<<"visited visited[nR][nC] "<<visited[nR][nC]<<endl;
                if (visited[nR][nC] == 0)
                {
                    // cout << "push to queue nr nc " << nR << " " << nC << endl;
                    q.push({nR, nC});
                    // cout<<"set visited "<<nR<<" "<<nC<<" = 1"<<endl;
                    visited[nR][nC] = 1;
                }
            }
        }
    }
}
int main()
{
    for(int i = 0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(visited[i][j] == 0 && adj[i][j] != 0)
            {
                islNum++;
                BFS(i,j);
            }
            
        }
    }
    cout<<"num of island "<<islNum<<endl;
    return 0;
}



/**
 * BFS implement for array traverse
*/
int adj[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int dR[] = {0, 0, -1, 1};
int dC[] = {-1, 1, 0, 0};
int visited[3][3]; // note: 3, not 2 here!!!!!!!!!!!!!!!
queue<pair<int, int>> q;
int main()
{
    // cout<<"visited visited[0][2] "<<visited[0][2]<<endl;
    q.push({0, 0});
    visited[0][0] = 1;
    while (!q.empty())
    {
        pair<int, int> crr = q.front();
        cout << "check value " << adj[crr.first][crr.second] << endl;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nR = crr.first + dR[i];
            int nC = crr.second + dC[i];
            // cout << "check nr nc " << nR << " " << nC << endl;
            if (0 <= nR && nR <= 2 && 0 <= nC && nC <= 2)
            {
                // cout<<"visited visited[nR][nC] "<<visited[nR][nC]<<endl;
                if (visited[nR][nC] == 0)
                {
                    // cout << "push to queue nr nc " << nR << " " << nC << endl;
                    q.push({nR, nC});
                    // cout<<"set visited "<<nR<<" "<<nC<<" = 1"<<endl;
                    visited[nR][nC] = 1;
                }
            }
        }
    }
    return 0;
}


/**
 * DFS implement
*/
vector<int> adjVector[6];
int visited[6] = {0};
void dfs(int node)
{
    cout<<"traverse node "<<node<<endl;
    visited[node] = 1;
    for(auto i : adjVector[node])
    {
        if(visited[i] == 0)
        {
            visited[i] = 1;
            dfs(i);
            
        }
        
    }
}
int main()
{
    adjVector[0] = {1, 2};
    adjVector[1] = {3, 4};
    adjVector[2] = {4, 5};
    dfs(0);
    return 0;
}


/**
 * BFS implement
*/
queue<int> q;
int visited[6] = {0};
vector<int> node[6];

int main()
{
    node[0] = {1, 2};
    node[1] = {3, 4};
    node[2] = {4, 5};
    q.push(0);
    visited[0] = 1;
    while (!q.empty())
    {
        int crr = q.front();
        cout << "traverse node: " << crr << endl;
        q.pop();
        for (auto i : node[crr])
        {
            if (visited[i] == 0)
            {
                q.push(i);
                visited[i] = 1;
            }
        }
    }
    return 0;
}

vector<int> iv = {2, 7, 11, 15};
int N = 14;


int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    for (auto i : iv)
    {
        if (find(iv.begin(), iv.end(), N - i) != iv.end())
        {
            cout << i << " and " << N - i << endl;
        }
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cout<<"runtime is : "<<elapsed_seconds.count() * 1000<<endl;
    return 0;
}


int sum = 0, k = 0;
int N = 9;
int main()
{
    while(sum <= N)
    {
        k++;
        sum += k;
        if(sum > N)
        {
            sum -= k;
            k -=1;
            break;
        }
        
    }
    cout<< "max row "<<k<< " with sum = "<<sum<<endl;
    return 0;
}


/**
 * Backtracking implementation
*/
vector<int> iV = {1, 2, 3};
vector<int> crr;
vector<vector<int>> res;
/**
 * Với crr, nếu phần tử cuối chưa phải là phần tử có chỉ số cao nhất trong mảng input, thì đẩy phần tử cao hơn vào
 * Nếu phần tử cuối đã là cao nhất, thì pop nó và phần tử trước nó thay bằng phần tử có chỉ số cao hơn tron mảng input
 */
void choose(int ind, const vector<int> &iV, vector<int> &crr, vector<vector<int>> &res)
{
    cout << "choose" << endl;
    for (int i = ind; i < iV.size(); i++)
    {
        cout << "crr before check coincide: ";
        for (auto k : crr)
        {
            cout << " " << k;
        }
        cout << endl;
        cout<<"check with iv[i] = "<<iV[i]<<endl;
        if (find(crr.begin(), crr.end(), iV[i]) == crr.end())
        {

            cout << "not find coincide, push iV[i] to crr" << endl;
            crr.push_back(iV[i]);
            cout << "crr now is: ";
            for (auto k : crr)
            {
                cout << " " << k;
            }
            cout << endl;
        }
        else
        {
            cout << "find coincide. Continue for next i in the same loop layer" << endl;
            continue;
        }
        if (crr.size() >= 3)
        {
            cout << "crr.size = " << crr.size() << " --> push result" << endl;
            res.push_back(crr);
        }
        else
        {
            cout << "crr.size = " << crr.size() << " --> call next loop layer" << endl;
            choose(0, iV, crr, res);
        }
        cout<<"pop back"<<endl;
        crr.pop_back();
    }
}
int main()
{
    choose(0, iV, crr, res);
    cout<<endl<<endl<<"result"<<endl;

    for (auto i : res)
    {
        cout << "{";
        for (auto j : i)
        {
            cout << j << " ";
        }
        cout << "}" << endl;
    }
    return 0;
}



void choose(int ind,const vector<int>& iV, vector<int>& crr, vector<vector<int>>& res)
{
    cout<<"choose with ind = "<<ind<<endl;
    for(int i = ind; i < iV.size(); i++)
    {
        if((i > ind)&&(iV[i] == iV[i-1]))
        {
            break;
        }
        crr.push_back(iV[i]);
        // if(crr.size() == 2)
        {
            res.push_back(crr);
        }
        choose(i, iV, crr, res);
        crr.pop_back();
    }
}

class Color
{
    protected:
    int c;
    public:
    Color(int tem) : c{tem}
    {
        cout<<"Color constructor"<<endl;
    }
    void show()
    {
        cout<<"Color's show"<<endl;
    }

};
class Square : public virtual Color
{   
    private:
    int x;
    public:
    Square(int temC, int temX) : Color(temC), x{temX}
    {
        cout<<"Square's constructor"<<endl;
    }
    void show()
    {
        cout<<"Square's show"<<endl;
    }
};
class Circle : public virtual Color
{   
    private:
    int r;
    public:
    Circle(int temC, int temR) : Color(temC), r{temR}
    {
        cout<<"Circle's constructor"<<endl;
    }
    void show()
    {
        cout<<"Circle's show"<<endl;
    }
};

class Shape : public Circle, public Square
{
    private:

    public:
    Shape(int temC, int temX, int temR) : Circle(temC,temR), Square(temC,temX), Color(temC)
    {
        cout<<"Shape's constructor"<<endl;
    }
    void show()
    {
        cout<<"Shape's show"<<endl;
        cout<<"color = "<<c<<endl;
    }
};

int main()
{
    Shape s(9,2,3);
    s.show();
}


/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
using namespace std;
const int K = 3;
vector<int> iV{1, 7, 1, 3, 2, 6};
int result[K];

int findPos(const vector<int> &v, int num)
{
    auto i = find(v.begin(), v.end(), num);
    if (i != v.end())
    {
        return (i - v.begin());
    }
    else
    {
        return -1;
    }
}
int numAfter(const vector<int> &v, int num)
{
    int x = findPos(v, num);
    if (-1 != x)
    {
        return (v.size() - (x + 1));
    }
    else
    {
        return -1;
    }
}

int main()
{
    cout << "sort v" << endl;
    vector<int> sortV = iV;
    int goodNum = 0;
    sort(sortV.begin(), sortV.end());
    for (auto tem : sortV)
    {
        if (numAfter(iV, tem) >= (K - 1 - goodNum))
        {
            result[goodNum] = tem;
            goodNum++;


        }
    }

    return 0;
}



using namespace std;
int dx[] = {0, 0, -1, 1};
int dy[] = {-1, 1, 0, 0};
const uint8_t N = 4; // number of row/column
int iArr[N][N];      // input array
int vArr[N][N];      // visited array
queue<pair<int, int>> myQ;
pair<int, int> rootNode;
int main()
{
    cout << "Input your root node position:" << endl;
    cout << "rootNode.x ";
    cin >> rootNode.first;
    cout << "rootNode.y ";
    cin >> rootNode.second;

    // push root node and mark as visited
    myQ.push(rootNode);

    // while queue not empty, check adjacent vertex
    while (!myQ.empty())
    {
        // front and pop from queue to check
        pair<int, int> cV = myQ.front();
        myQ.pop();
        cout << "check node arr" << cV.first << cV.second << endl;

        for (int i = 0; i < 4; i++)
        {
            pair<int, int> nV(cV.first + dx[i], cV.second + dy[i]);
            if ((nV.first >= 0 && nV.first <= 3 && nV.second <= 3 && nV.second >= 0) &&
                (vArr[nV.first][nV.second] == 0))
            {
                myQ.push(nV);
                vArr[nV.first][nV.second] = 1;
            }
        }
    }

    cout << "goodbye world" << endl;
    return 0;
}

class myVector
{
    private:
    int mV;
    public:
    myVector(int tem) : mV{tem}
    {
        
    }
    void display()
    {
        cout<<"myVector display "<<mV<<endl;
    }
};

class myPoint
{
    private:
    int mP;
    public:
    myPoint(int tem) : mP{tem}
    {
        
    }
    void display()
    {
        cout<<"myPoint display"<<mP<<endl;
    }
};

template <class T> class myClass
{
    private:
    T t;
    public:
    myClass(T tem) : t{tem}
    {

    }
    void display()
    {
        t.display();
    }
};

int main()
{
    myVector mV(1);
    myPoint mP(2);
    myClass<myVector> mC(mV);
    mC.display();
    myClass<myPoint> mC2(mP);
    mC2.display();
    return 0;
}


class Tree
{
public:
    int data;
    Tree *left;
    Tree *right;
    Tree()
    {
    }
    ~Tree()
    {
    }
};

int tem = 0;
int main()
{

    Tree rootNode;
    cout << "insert rootNode data: " << endl;
    cin >> rootNode.data;
    rootNode.left = nullptr;
    rootNode.right = nullptr;

    while (111 != tem)
    {
        cout << "insert iNode data: " << endl;
        cin >> tem;

        Tree iNode;
        iNode.data = tem;
        iNode.left = nullptr;
        iNode.right = nullptr;

        Tree *head = &rootNode;
        while (iNode.data <= head->data)
        {
            if (head->left != nullptr)
            {
                head = head->left;
            }
            else
            {
                head->left = &iNode;
                break;
            }

            while (iNode.data >= head->data)
            {
                if (head->right != nullptr)
                {
                    head = head->right;
                }
                else
                {
                    head->right = &iNode;
                    break;
                }
            }
        }
    }

    return 0;
}



int n{0};

int main()
{
    cout<<"input n = ";
    cin>>n;
    cout<<endl;
    int arr[n];
    cout<<"input your arr:";
    for(int i = 0; i<n; i++)
    {
        cin>>arr[i];
    }
    cout<<"sorting..."<<endl;
    for(int i = 0; i <= n-2; i++)
    {
        for(int j=0; j<= n-2-i; j++)
        {
            int tem = 0;
            if(arr[j]>arr[j+1])
            {
                tem = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tem;
            }
        }
    }
    cout<<"after sort:"<<endl;
   for(int i = 0; i<n; i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;

    return 0;
}

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

        // SlddReceiver slr;
        // slr.printString();
        // slr.printNumber();
      
    }
    else // child process
    {
        execl(path, path, cmd, NULL);
    }

    return 0;
}

#include <iostream>
using namespace std;

/*
    Name: detach_method
    Copyright: LG
    Author: kien4.hoang
    Date: 22/11/21 11:04
    Description: Illustrate detach method. After the call of detach() function, main thread is not waiting for
    the termination of its child thread.
*/

#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include "lib.h"
using namespace std;

/*
    Name: virtual_function
    Copyright: LG
    Author: kien4.hoang
    Date: 22/11/21 11:04
    Description: Demonstrate how virtual function can fix the problem when using base class pointer (base
    class pointer points to derived object and call method which co-exist in base and derived class)
*/
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
int volumn{0};
std::mutex mt;
bool lock = true;
std::recursive_mutex rm;
void increaseVolumn(unsigned char level)
{
    if (level <= 0)
    {
        return;
    }
    rm.lock();
    volumn++;
    cout << "volumn = " << volumn << endl;
    increaseVolumn(--level);
    rm.unlock();
}

void decreaseVolumn()
{
    for (unsigned int i = 0; i < 100000; i++)
    {
        std::lock_guard<std::mutex> lg(mt);
        volumn--;
    }
}
void printVolumn()
{
    cout << "volumn = " << volumn << endl;
}
class Base
{
private:
    int position;

public:
    Base(int p) : position{p}
    {
        cout << "Base constructor" << endl;
    }
    ~Base()
    {
        cout << "Base detructor" << endl;
    }
    void showPosition()
    {
        cout << "Position = " << position << endl;
    }
    void setPosition(int p)
    {
        position = p;
    }
    virtual void showHello()
    {
        cout << "Base showHello" << endl;
    }
};

class Derived : public Base
{
private:
    int color;

public:
    Derived(int p, int c) : Base{p}, color{c}
    {
        cout << "Derived constructor" << endl;
    }
    ~Derived()
    {
        cout << "Derived detructor" << endl;
    }
    void showColor()
    {
        cout << "color = " << color << endl;
    }
    void setColor(int c)
    {
        color = c;
    }
    void showHello()
    {
        cout << "Derived showHello" << endl;
    }
    void showAll()
    {
        showPosition();
        showColor();
    }
};

class Array
{
private:
    int mLength;
    int *mValue;

public:
    Array()
    {
        cout << "Array constructor" << endl;
        cout << "mLength = ";
        cin >> mLength;
        mValue = new int[mLength];
        cout << "Input value" << endl;
        for (int i = 0; i < mLength; i++)
        {
            cin >> mValue[i];
        }
    }

    Array(const Array& a)
    {
        mLength = a.mLength;
        mValue = new int[mLength];
        if (nullptr != a.mValue)
        {
            for (int i = 0; i < mLength; i++)
            {
                mValue[i] = a.mValue[i];
            }
        }
    }
    ~Array()
    {
        cout << "Array destructor" << endl;
        if (nullptr != mValue)
        {
            cout << "mValue isn't null. Delete." << endl;
            delete mValue;
            mValue = nullptr;
        }
        else
        {
            cout << "mValue is null. Do nothing." << endl;
        }
    }
    void showArray()
    {
        cout << "show array" << endl;
        for (int i = 0; i < mLength; i++)
        {
            cout << mValue[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    Array a1;
    a1.showArray();
    Array a2(a1);
    a2.showArray();
    return 0;
}

string name;
unsigned int arr[10];
unsigned int arrSize=0;

void showArr()
{
    cout<<"your arr is: "<<endl;
    for(unsigned char i=0;i<arrSize;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

void inputArr()
{
    cout<<"input arr size:"<<endl;
    cin>>arrSize;

    cout<<"input arr"<<endl;
    for(unsigned char i=0;i<arrSize;i++)
    {
        cin>>arr[i];
    }
}

unsigned int findMax()
{
    unsigned int max=0;
    for(unsigned char i=0;i<arrSize;i++)
    {
        if(arr[i]>=max)
        {
            max=arr[i];
        }
    }
    return max;
}

void sortArr()
{
    for(unsigned int i = 1 ; i <= arrSize ; i++)
    {
        for(unsigned int k = 0 ; k < (arrSize - i) ; k++)
        {
            if(arr[k] >= arr[k+1])
            {
                unsigned int tem = arr[k];
                arr[k] = arr[k+1];
                arr[k+1] = tem;
            }
        }
    }
}

int main()
{
    inputArr();
    showArr();
    sortArr();
    showArr();



    return 0;
}


//------------------------#if0---------------------------------------------------------


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
	virtual void mDisplay()
	{
		cout<<"mDisplay of base class running..."<<endl;
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
	void mDisplay()
	{
		cout<<"mDisplay of derived class running..."<<endl;
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
	
	ptr->mDisplay();
	
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

#define PARENT_PROCESS 0

const char *PATH = "/home/kienhoang/WORKSPACE/LearnCPPGitHub/ecallApp.exe";
const char *CMD = "c";
const char *COMMENT = "run chat application";



void countUp()
{
    cout<<"start countUp(). thread_id == "<<this_thread::get_id()<< endl;
    int num{0};
    while (true)
    {
        usleep(1000000);
        cout << num++ << endl;
    }
}

void showMessage()
{
    cout<<"start showMessage(). thread_id == "<<this_thread::get_id()<< endl;
    SlddReceiver slr;
    while (true)
    {
        slr.printString();
        // usleep(100000);
    }
}

int main()
{
    cout<<"start main(). thread_id == "<<this_thread::get_id()<< endl;
    unsigned int mPid{0};
    std::thread countUpTh(countUp);
    std::thread showMsgTh(showMessage);
    if(countUpTh.joinable())
    {
        countUpTh.join();
    }
    if(showMsgTh.joinable())
    {
        showMsgTh.join();
    }    
    /*
    mPid = fork();
    if (PARENT_PROCESS == mPid) // parent process
    {
        cout << "This is main process. PID == "<<getpid()<<" and thread_id == "<<this_thread::get_id()<< endl;
    }
    else // child process
    {
        execl(PATH, CMD, COMMENT, NULL);
    }
    */

    return 0;
}

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
