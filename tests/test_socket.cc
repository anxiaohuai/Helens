#include "helens/socket.h"
#include "helens/helens.h"
#include "helens/iomanager.h"

static helens::Logger::ptr g_looger = HELENS_LOG_ROOT();

void test_socket() {
    //std::vector<helens::Address::ptr> addrs;
    //helens::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    //helens::IPAddress::ptr addr;
    //for(auto& i : addrs) {
    //    HELENS_LOG_INFO(g_looger) << i->toString();
    //    addr = std::dynamic_pointer_cast<helens::IPAddress>(i);
    //    if(addr) {
    //        break;
    //    }
    //}
    helens::IPAddress::ptr addr = helens::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr) {
        HELENS_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        HELENS_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    helens::Socket::ptr sock = helens::Socket::CreateTCP(addr);
    addr->setPort(80);
    HELENS_LOG_INFO(g_looger) << "addr=" << addr->toString();
    if(!sock->connect(addr)) {
        HELENS_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        HELENS_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0) {
        HELENS_LOG_INFO(g_looger) << "send fail rt=" << rt;
        return;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if(rt <= 0) {
        HELENS_LOG_INFO(g_looger) << "recv fail rt=" << rt;
        return;
    }

    buffs.resize(rt);
    HELENS_LOG_INFO(g_looger) << buffs;
}

void test2() {
    helens::IPAddress::ptr addr = helens::Address::LookupAnyIPAddress("www.baidu.com:80");
    if(addr) {
        HELENS_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        HELENS_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    helens::Socket::ptr sock = helens::Socket::CreateTCP(addr);
    if(!sock->connect(addr)) {
        HELENS_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        HELENS_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    uint64_t ts = helens::GetCurrentUS();
    for(size_t i = 0; i < 10000000000ul; ++i) {
        if(int err = sock->getError()) {
            HELENS_LOG_INFO(g_looger) << "err=" << err << " errstr=" << strerror(err);
            break;
        }

        //struct tcp_info tcp_info;
        //if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
        //    HELENS_LOG_INFO(g_looger) << "err";
        //    break;
        //}
        //if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
        //    HELENS_LOG_INFO(g_looger)
        //            << " state=" << (int)tcp_info.tcpi_state;
        //    break;
        //}
        static int batch = 10000000;
        if(i && (i % batch) == 0) {
            uint64_t ts2 = helens::GetCurrentUS();
            HELENS_LOG_INFO(g_looger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}

int main(int argc, char** argv) {
    helens::IOManager iom;
    //iom.schedule(&test_socket);
    iom.schedule(&test2);
    return 0;
}
