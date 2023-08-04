#include "helens/streams/service_discovery.h"
#include "helens/iomanager.h"
#include "helens/rock/rock_stream.h"
#include "helens/log.h"
#include "helens/worker.h"

helens::ZKServiceDiscovery::ptr zksd(new helens::ZKServiceDiscovery("127.0.0.1:21812"));
helens::RockSDLoadBalance::ptr rsdlb(new helens::RockSDLoadBalance(zksd));

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

std::atomic<uint32_t> s_id;
void on_timer() {
    g_logger->setLevel(helens::LogLevel::INFO);
    //HELENS_LOG_INFO(g_logger) << "on_timer";
    helens::RockRequest::ptr req(new helens::RockRequest);
    req->setSn(++s_id);
    req->setCmd(100);
    req->setBody("hello");

    auto rt = rsdlb->request("helens.top", "blog", req, 1000);
    if(!rt->response) {
        if(req->getSn() % 50 == 0) {
            HELENS_LOG_ERROR(g_logger) << "invalid response: " << rt->toString();
        }
    } else {
        if(req->getSn() % 1000 == 0) {
            HELENS_LOG_INFO(g_logger) << rt->toString();
        }
    }
}

void run() {
    zksd->setSelfInfo("127.0.0.1:2222");
    zksd->setSelfData("aaaa");

    std::unordered_map<std::string, std::unordered_map<std::string,std::string> > confs;
    confs["helens.top"]["blog"] = "fair";
    rsdlb->start(confs);
    //HELENS_LOG_INFO(g_logger) << "on_timer---";

    helens::IOManager::GetThis()->addTimer(1, on_timer, true);
}

int main(int argc, char** argv) {
    helens::WorkerMgr::GetInstance()->init({
        {"service_io", {
            {"thread_num", "1"}
        }}
    });
    helens::IOManager iom(1);
    iom.addTimer(1000, [](){
            std::cout << rsdlb->statusString() << std::endl;
    }, true);
    iom.schedule(run);
    return 0;
}
