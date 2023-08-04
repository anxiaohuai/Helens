#include "helens/http/ws_server.h"
#include "helens/log.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void run() {
    helens::http::WSServer::ptr server(new helens::http::WSServer);
    helens::Address::ptr addr = helens::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        HELENS_LOG_ERROR(g_logger) << "get address error";
        return;
    }
    auto fun = [](helens::http::HttpRequest::ptr header
                  ,helens::http::WSFrameMessage::ptr msg
                  ,helens::http::WSSession::ptr session) {
        session->sendMessage(msg);
        return 0;
    };

    server->getWSServletDispatch()->addServlet("/helens", fun);
    while(!server->bind(addr)) {
        HELENS_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }
    server->start();
}

int main(int argc, char** argv) {
    helens::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
