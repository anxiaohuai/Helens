#ifndef __HELENS_ROCK_SERVER_H__
#define __HELENS_ROCK_SERVER_H__

#include "helens/rock/rock_stream.h"
#include "helens/tcp_server.h"

namespace helens {

class RockServer : public TcpServer {
public:
    typedef std::shared_ptr<RockServer> ptr;
    RockServer(const std::string& type = "rock"
               ,helens::IOManager* worker = helens::IOManager::GetThis()
               ,helens::IOManager* io_worker = helens::IOManager::GetThis()
               ,helens::IOManager* accept_worker = helens::IOManager::GetThis());

protected:
    virtual void handleClient(Socket::ptr client) override;
};

}

#endif
