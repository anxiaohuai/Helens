#include "helens/http/http_server.h"
#include "helens/log.h"

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();
helens::IOManager::ptr worker;
void run() {
    g_logger->setLevel(helens::LogLevel::INFO);
    helens::Address::ptr addr = helens::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        HELENS_LOG_ERROR(g_logger) << "get address error";
        return;
    }

    helens::http::HttpServer::ptr http_server(new helens::http::HttpServer(true, worker.get()));
    //helens::http::HttpServer::ptr http_server(new helens::http::HttpServer(true));
    bool ssl = false;
    while(!http_server->bind(addr, ssl)) {
        HELENS_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }

    if(ssl) {
        //http_server->loadCertificates("/home/apps/soft/helens/keys/server.crt", "/home/apps/soft/helens/keys/server.key");
    }

    http_server->start();
}

int main(int argc, char** argv) {
    helens::IOManager iom(1);
    worker.reset(new helens::IOManager(4, false));
    iom.schedule(run);
    return 0;
}
