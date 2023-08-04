### IO协程调度模块

IO调度NO、1：18分16秒

支持异步IO，使用epoll

IOManager(epoll)------>Scheduler

​				 |

​				 |

​				V

​			idel(epoll_wait)



在**协程调度模块**中，调度器对协程的调度是无条件执行的，在调度器已经启动调度的情况下，任务一旦添加成功，就会排队等待调度器执行。调度器不支持删除调度任务，并且调度器在正常退出之前一定会执行完全部的调度任务。



**`IO`协程调度器**直接继承协程调度器实现，因此支持协程调度器的全部功能。此外`IO`协程调度还**增加了`IO`事件调度的功能**，这个功能针对`socket fd`。**`IO`协程调度支持为`socket fd`注册可读和可写事件的回调函数或协程，当描述符可读或可写时，执行对应的回调函数或协程**



**`IO`事件调度功能**对服务器开发至关重要，因为服务器通常需要处理大量来自客户端的`socket fd`，**使用`IO`事件调度可以将开发者从判断`socket fd`是否可读或可写的工作中解放出来，使得程序员只需要关心`socket fd`的`IO`操作**



#### 实现

基于epoll

##### 事件

两类：read事件EPOLLIN 、write事件EPOLLOUT。为了简化操作，`helens`是将所有事件全部归为`EPOLLIN`和`EPOLLOUT`中，也就是所有的事件都可以表示为可读或可写事件，甚至有的事件还可以同时表示可读及可写事件，比如`EPOLLERR`事件发生时，`socket fd`将同时触发可读和可写事件。

（当然`epoll`本身除了支持了`EPOLLIN`和`EPOLLOUT`两类事件外，还支持`EPOLLRDHUP`，`EPOLLERR`，`EPOLLHUP`等。）

```c++
enum EventStatus {
    NONE = 0x0,
    READ = 0x001,
    WRITE = 0x004
};
```

##### 三元组信息

对于`IO`协程调度来说，每次调度都包含一个三元组信息(`描述符-事件类型-回调信息`)，我们通过一个结构体`Context`来存储三原组信息。因为一个描述符可能同时注册读和写事件，因此分别使用`read`和`write`属性来保存读和写的回调信息，从而在不同事件类型被触发时执行对应回调

```c++
typedef struct {
    struct EventContext {
        Scheduler::ptr scheduler;
        pid_t thread_id;
        Coroutine::ptr co;
        std::function<void()> cb;
    };
    MutexLock mutex;
    int fd = 0;
    EventStatus statuses = EventStatus::NONE;
    EventContext read;
    EventContext write;

    EventContext &get_event_context(const EventStatus status);
    void reset_event_context(EventContext &ctx);
    void trigger_event(const EventStatus status);
} Context;

```

##### 注册事件

epoll

三个方法：epoll_create、epoll_wait、epoll_ctl

一个结构体：epoll_event



IO协程调度器会将全部的三元组信息存储在属性m_contexts中，m_contexts的类型为std::vector<Context *>。IO调度器直接使用三元组的fd的值作为m_contexts数组的下标，这样可以快速找到一个fd对应的Context对象。由于关闭的fd会被重复利用，所以这里也不用担心m_contexts数组膨胀太快，或是利用率低的问题



`IO`协程调度器提供一个添加注册事件的接口`bool IOManager::add_event(int fd, EventStatus status, std::function<void()> cb)`，接口需要指定`fd`，事件类型，回调函数三个参数。



首先我们通过`fd`从`m_contexts`中取出一个`Context`对象(如果`fd`大于`m_contexts`的长度，表明`m_contexts`需要先进行扩容)，然后使用`status`和`cb`两个参数来重新初始化`Context`对象





注册事件即向一个`epoll`实例中的一个`fd`添加一个`epoll_event`，然后使用`epoll_event.data.ptr`指针指向初始化好的`Context`对象，以便后续`epoll_wait`返回时能拿到`fd`的上下文信息即`Context`，并且执行其中的回调函数或协程。



##### 删除事件

类似注册事件

##### 取消事件

类似删除，区别在于，取消事件在找到事件后，强制触发执行该事件的回调。

##### idel协程

IO协程调度器为了解决协程调度器在idle状态下忙等待导致CPU占用率高的问题。IO协程调度器使用一对管道fd来tickle调度协程，当调度器空闲时idle协程通过epoll_wait阻塞在管道的读描述符上，等待管道的可读事件。添加新任务时tickle方法写管道，idle协程检测到**管道pipe**可读后退出，调度器执行调度


#### pipe函数

创建管道

  int pipe(int pipefd[2]); 成功：0；失败：-1，设置errno

函数调用成功返回r/w两个文件描述符。无需open，但需手动close。规定：fd[0] → r； fd[1] → w，就像0对应标准输入，1对应标准输出一样。向管道文件读写数据其实是在读写内核缓冲区。

管道创建成功以后，创建该管道的进程（父进程）同时掌握着管道的读端和写端。如何实现父子进程间通信呢？通常可以采用如下步骤：

![调用图](https://github.com/anxiaohuai/Helens/blob/main/doc/pipe.jpg)

1. 父进程调用pipe函数创建管道，得到两个文件描述符fd[0]、fd[1]指向管道的读端和写端。

2. 父进程调用fork创建子进程，那么子进程也有两个文件描述符指向同一管道。

3. 父进程关闭管道读端，子进程关闭管道写端。父进程可以向管道中写入数据，子进程将管道中的数据读出。由于管道是利用环形队列实现的，数据从写端流入管道，从读端流出，这样就实现了进程间通信。

##### **管道的读写行为**

  使用管道需要注意以下4种特殊情况（假设都是阻塞I/O操作，没有设置O_NONBLOCK标志）：

1. 读管道：

   1. 管道中有数据：read返回实际读到的字节数。

   2.  管道中无数据：

      (1) 管道写端被全部关闭，read返回0 (好像读到文件结尾)

       (2) 写端没有全部被关闭，read阻塞等待(不久的将来可能有数据递达，此时会让出cpu)

2. 写管道：

   1. 管道读端全部被关闭， 进程异常终止(也可使用捕捉SIGPIPE信号，使进程不终止)

   2. 管道读端没有全部关闭：

      (1) 管道已满，write阻塞。

      (2) 管道未满，write将数据写入，并返回实际写入的字节数。