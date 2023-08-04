#include "helens/tcp_server.h"
#include "helens/iomanager.h"
#include "helens/log.h"

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void run() {
    auto addr = helens::Address::LookupAny("0.0.0.0:8033");
    //auto addr2 = helens::UnixAddress::ptr(new helens::UnixAddress("/tmp/unix_addr"));
    std::vector<helens::Address::ptr> addrs;
    addrs.push_back(addr);
    //addrs.push_back(addr2);

    helens::TcpServer::ptr tcp_server(new helens::TcpServer);
    std::vector<helens::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails)) {
        sleep(2);
    }
    tcp_server->start();
    
}
int main(int argc, char** argv) {
    helens::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
