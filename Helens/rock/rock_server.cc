#include "rock_server.h"
#include "helens/log.h"
#include "helens/module.h"

namespace helens {

static helens::Logger::ptr g_logger = HELENS_LOG_NAME("system");

RockServer::RockServer(const std::string& type
                       ,helens::IOManager* worker
                       ,helens::IOManager* io_worker
                       ,helens::IOManager* accept_worker)
    :TcpServer(worker, io_worker, accept_worker) {
    m_type = type;
}

void RockServer::handleClient(Socket::ptr client) {
    HELENS_LOG_DEBUG(g_logger) << "handleClient " << *client;
    helens::RockSession::ptr session(new helens::RockSession(client));
    session->setWorker(m_worker);
    ModuleMgr::GetInstance()->foreach(Module::ROCK,
            [session](Module::ptr m) {
        m->onConnect(session);
    });
    session->setDisconnectCb(
        [](AsyncSocketStream::ptr stream) {
             ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [stream](Module::ptr m) {
                m->onDisconnect(stream);
            });
        }
    );
    session->setRequestHandler(
        [](helens::RockRequest::ptr req
           ,helens::RockResponse::ptr rsp
           ,helens::RockStream::ptr conn)->bool {
            //HELENS_LOG_INFO(g_logger) << "handleReq " << req->toString()
            //                         << " body=" << req->getBody();
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, req, rsp, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                rt = m->handleRequest(req, rsp, conn);
            });
            return rt;
        }
    ); 
    session->setNotifyHandler(
        [](helens::RockNotify::ptr nty
           ,helens::RockStream::ptr conn)->bool {
            HELENS_LOG_INFO(g_logger) << "handleNty " << nty->toString()
                                     << " body=" << nty->getBody();
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, nty, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                rt = m->handleNotify(nty, conn);
            });
            return rt;
        }
    );
    session->start();
}

}
