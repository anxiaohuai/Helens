#include "helens/helens.h"
#include "helens/ns/ns_protocol.h"
#include "helens/ns/ns_client.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

int type = 0;

void run() {
    g_logger->setLevel(helens::LogLevel::INFO);
    auto addr = helens::IPAddress::Create("127.0.0.1", 8072);
    //if(!conn->connect(addr)) {
    //    HELENS_LOG_ERROR(g_logger) << "connect to: " << *addr << " fail";
    //    return;
    //}
    if(type == 0) {
        for(int i = 0; i < 5000; ++i) {
            helens::RockConnection::ptr conn(new helens::RockConnection);
            conn->connect(addr);
            helens::IOManager::GetThis()->addTimer(3000, [conn, i](){
                    helens::RockRequest::ptr req(new helens::RockRequest);
                    req->setCmd((int)helens::ns::NSCommand::REGISTER);
                    auto rinfo = std::make_shared<helens::ns::RegisterRequest>();
                    auto info = rinfo->add_infos();
                    info->set_domain(std::to_string(rand() % 2) + "domain.com");
                    info->add_cmds(rand() % 2 + 100);
                    info->add_cmds(rand() % 2 + 200);
                    info->mutable_node()->set_ip("127.0.0.1");
                    info->mutable_node()->set_port(1000 + i);
                    info->mutable_node()->set_weight(100);
                    req->setAsPB(*rinfo);

                    auto rt = conn->request(req, 100);
                    HELENS_LOG_INFO(g_logger) << "[result="
                        << rt->result << " response="
                        << (rt->response ? rt->response->toString() : "null")
                        << "]";
            }, true);
            conn->start();
        }
    } else {
        for(int i = 0; i < 1000; ++i) {
            helens::ns::NSClient::ptr nsclient(new helens::ns::NSClient);
            nsclient->init();
            nsclient->addQueryDomain(std::to_string(i % 2) + "domain.com");
            nsclient->connect(addr);
            nsclient->start();
            HELENS_LOG_INFO(g_logger) << "NSClient start: i=" << i;

            if(i == 0) {
                //helens::IOManager::GetThis()->addTimer(1000, [nsclient](){
                //    auto domains = nsclient->getDomains();
                //    domains->dump(std::cout, "    ");
                //}, true);
            }
        }

        //conn->setConnectCb([](helens::AsyncSocketStream::ptr ss) {
        //    auto conn = std::dynamic_pointer_cast<helens::RockConnection>(ss);
        //    helens::RockRequest::ptr req(new helens::RockRequest);
        //    req->setCmd((int)helens::ns::NSCommand::QUERY);
        //    auto rinfo = std::make_shared<helens::ns::QueryRequest>();
        //    rinfo->add_domains("0domain.com");
        //    req->setAsPB(*rinfo);
        //    auto rt = conn->request(req, 1000);
        //    HELENS_LOG_INFO(g_logger) << "[result="
        //        << rt->result << " response="
        //        << (rt->response ? rt->response->toString() : "null")
        //        << "]";
        //    return true;
        //});

        //conn->setNotifyHandler([](helens::RockNotify::ptr nty,helens::RockStream::ptr stream){
        //        auto nm = nty->getAsPB<helens::ns::NotifyMessage>();
        //        if(!nm) {
        //            HELENS_LOG_ERROR(g_logger) << "invalid notify message";
        //            return true;
        //        }
        //        HELENS_LOG_INFO(g_logger) << helens::PBToJsonString(*nm);
        //        return true;
        //});
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        type = 1;
    }
    helens::IOManager iom(5);
    iom.schedule(run);
    return 0;
}
