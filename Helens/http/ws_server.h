#ifndef __HELENS_HTTP_WS_SERVER_H__
#define __HELENS_HTTP_WS_SERVER_H__

#include "helens/tcp_server.h"
#include "ws_session.h"
#include "ws_servlet.h"

namespace helens {
namespace http {

class WSServer : public TcpServer {
public:
    typedef std::shared_ptr<WSServer> ptr;

    WSServer(helens::IOManager* worker = helens::IOManager::GetThis()
             , helens::IOManager* io_worker = helens::IOManager::GetThis()
             , helens::IOManager* accept_worker = helens::IOManager::GetThis());

    WSServletDispatch::ptr getWSServletDispatch() const { return m_dispatch;}
    void setWSServletDispatch(WSServletDispatch::ptr v) { m_dispatch = v;}
protected:
    virtual void handleClient(Socket::ptr client) override;
protected:
    WSServletDispatch::ptr m_dispatch;
};

}
}

#endif
