#include "helens/helens.h"
#include "helens/rock/rock_stream.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

helens::RockConnection::ptr conn(new helens::RockConnection);
void run() {
    conn->setAutoConnect(true);
    helens::Address::ptr addr = helens::Address::LookupAny("127.0.0.1:8061");
    if(!conn->connect(addr)) {
        HELENS_LOG_INFO(g_logger) << "connect " << *addr << " false";
    }
    conn->start();

    helens::IOManager::GetThis()->addTimer(1000, [](){
        helens::RockRequest::ptr req(new helens::RockRequest);
        static uint32_t s_sn = 0;
        req->setSn(++s_sn);
        req->setCmd(100);
        req->setBody("hello world sn=" + std::to_string(s_sn));

        auto rsp = conn->request(req, 300);
        if(rsp->response) {
            HELENS_LOG_INFO(g_logger) << rsp->response->toString();
        } else {
            HELENS_LOG_INFO(g_logger) << "error result=" << rsp->result;
        }
    }, true);
}

int main(int argc, char** argv) {
    helens::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
