#include "helens/socket.h"
#include "helens/log.h"
#include "helens/iomanager.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void run() {
    helens::IPAddress::ptr addr = helens::Address::LookupAnyIPAddress("0.0.0.0:8050");
    helens::Socket::ptr sock = helens::Socket::CreateUDP(addr);
    if(sock->bind(addr)) {
        HELENS_LOG_INFO(g_logger) << "udp bind : " << *addr;
    } else {
        HELENS_LOG_ERROR(g_logger) << "udp bind : " << *addr << " fail";
        return;
    }
    while(true) {
        char buff[1024];
        helens::Address::ptr from(new helens::IPv4Address);
        int len = sock->recvFrom(buff, 1024, from);
        if(len > 0) {
            buff[len] = '\0';
            HELENS_LOG_INFO(g_logger) << "recv: " << buff << " from: " << *from;
            len = sock->sendTo(buff, len, from);
            if(len < 0) {
                HELENS_LOG_INFO(g_logger) << "send: " << buff << " to: " << *from
                    << " error=" << len;
            }
        }
    }
}

int main(int argc, char** argv) {
    helens::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
