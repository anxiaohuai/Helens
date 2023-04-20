# 协程模块

## 目标：

要做的协程是：每一个线程有一个主协程，主协程-->n个子协程，每一个子协程的调度都需要返回主协程去进行调度。

![调用图](https://github.com/anxiaohuai/Helens/blob/main/doc/%E8%B0%83%E7%94%A8%E5%9B%BE.jpg)

## 库：

协程是基于上下文实现，因此，调用`ucontext.h`头文件，必须对`ucontext_t`的结构和相关函数非常熟悉。



> ucontext_t

```c++
typedef struct ucontext_t {
    struct ucontext_t *uc_link; // 当前上下文结束后，下一个激活的上下文对象的指针，只在当前上下文是由makecontext创建时有效
    stack_t uc_stack;           // 当前上下文使用的栈内存空间
    sigset_t uc_sigmask;        // 当前上下文信号屏蔽掩码
    mcontext_t uc_mcontext;     // 当前上下文寄存器信息
} ucontext_t;

```

四个函数：

1. `int getcontext(ucontext_t *ucp);`获得当前函数上下文
2. `void makecontext(ucontext_t *ucp, void (*)(void)func, int argc, ...);`：创建上下文，此函数会将ucp和func进行绑定，并且支持指定执行func所需要的参数。但是在调用makecontext之前必须手动开辟一个空间，并使用ucp的uc_stack指定指向这块内存，这一内存空间将作为func执行所需要的内存空间。ucp中的uc_link如果不指定，那么在协程结束时，必须调用setcontext或swapcontext重新指定一个有效的上下文，否则程序将跑飞。
3. `int setcontext(const ucontext_t *ucp);`：恢复上下文，此函数不会返回，会直接跳转到`ucp`所对于的函数执行
4. `int swapcontext(ucontext_t *oucp, const ucontext_t *ucp);`：切换上下文，此函数与`setcontext`类似。都不会返回，而是直接跳转到`ucp`所对应的函数开始执行，但`swapcontext`会将当前函数的上下文保存到`oucp`中

## 协程



**构造函数**：

1. 无参：创建主协程，id=0，设置状态为执行中。调用`getcontext`获取**线程**上下文栈内容，变成主协程的。
2. 有参：创建一个子协程，真正需要分配栈空间的。获取协程栈空间、`getcontext`获取当前上下文，设置当前协程的参数（uc_link、uc_stack）、调用`makecontext`创建上下文。

**析构函数：**

1. 如果不是主协程，回收栈。
2. 如果是主协程，不用回收。

**协程状态：**

```c++
enum State {
        /// 初始化状态
        INIT,
        /// 暂停状态
        HOLD,
        /// 执行中状态
        EXEC,
        /// 结束状态
        TERM,
        /// 可执行状态
        READY,
        /// 异常状态
        EXCEPT
    };

```



**协程切换：**

1. `swapIn（）`：调用`swapcontext`，将两个协程的上下文进行交换。切换到当前协程执行。主协程---swap---->当前协程
2. `swapout:`调用`swapcontext`，将两个协程的上下文进行交换。当前线程切换到后台执行。当前协程---swap---->主协程。
3. `setThis`：设置当前协程。



## 注意

1. 一般不会在协程上大的对象，能用指针尽量用指针

2. 可以通俗一点理解，协程就是看上去和使用起来的都比较特殊的函数。协程在创建的时候都需要指定一个函数入口，这一点可以类比线程。但是协程与函数的区别在于，函数一旦被调用就会从头执行到函数结束，而协程可以在执行途中退出，后续又从退出处恢复继续执行。

3. `CPU`核的基本调度单元是线程，一个线程指的是进程中一个单一顺序的控制流，一个进程中可以有多个线程，当系统具有多个`CPU`核时，这些线程可以实现并行。同样一个线程可以有多个协程，但即便在多核`CPU`下这些协程也是串行无法并行执行。所以协程与进程和线程不属于一个维度的概念。

   