#include "helens/helens.h"
#include "helens/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

int sock = 0;

void test_fiber() {
    HELENS_LOG_INFO(g_logger) << "test_fiber sock=" << sock;

    //sleep(3);

    //close(sock);
    //helens::IOManager::GetThis()->cancelAll(sock);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "115.239.210.27", &addr.sin_addr.s_addr);

    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr))) {
    } else if(errno == EINPROGRESS) {
        HELENS_LOG_INFO(g_logger) << "add event errno=" << errno << " " << strerror(errno);
        helens::IOManager::GetThis()->addEvent(sock, helens::IOManager::READ, [](){
            HELENS_LOG_INFO(g_logger) << "read callback";
        });
        helens::IOManager::GetThis()->addEvent(sock, helens::IOManager::WRITE, [](){
            HELENS_LOG_INFO(g_logger) << "write callback";
            //close(sock);
            helens::IOManager::GetThis()->cancelEvent(sock, helens::IOManager::READ);
            close(sock);
        });
    } else {
        HELENS_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
    }

}

void test1() {
    std::cout << "EPOLLIN=" << EPOLLIN
              << " EPOLLOUT=" << EPOLLOUT << std::endl;
    helens::IOManager iom(2, false);
    iom.schedule(&test_fiber);
}

helens::Timer::ptr s_timer;
void test_timer() {
    helens::IOManager iom(2);
    s_timer = iom.addTimer(1000, [](){
        static int i = 0;
        HELENS_LOG_INFO(g_logger) << "hello timer i=" << i;
        if(++i == 3) {
            s_timer->reset(2000, true);
            //s_timer->cancel();
        }
    }, true);
}

int main(int argc, char** argv) {
    //test1();
    test_timer();
    return 0;
}
