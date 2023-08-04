#ifndef __HELENS_NS_NAME_SERVER_MODULE_H__
#define __HELENS_NS_NAME_SERVER_MODULE_H__

#include "helens/module.h"
#include "ns_protocol.h"

namespace helens {
namespace ns {

class NameServerModule;
class NSClientInfo {
friend class NameServerModule;
public:
    typedef std::shared_ptr<NSClientInfo> ptr;
private:
    NSNode::ptr m_node;
    std::map<std::string, std::set<uint32_t> > m_domain2cmds;
};

class NameServerModule : public RockModule {
public:
    typedef std::shared_ptr<NameServerModule> ptr;
    NameServerModule();

    virtual bool handleRockRequest(helens::RockRequest::ptr request
                        ,helens::RockResponse::ptr response
                        ,helens::RockStream::ptr stream) override;
    virtual bool handleRockNotify(helens::RockNotify::ptr notify
                        ,helens::RockStream::ptr stream) override;
    virtual bool onConnect(helens::Stream::ptr stream) override;
    virtual bool onDisconnect(helens::Stream::ptr stream) override;
    virtual std::string statusString() override;
private:
    bool handleRegister(helens::RockRequest::ptr request
                        ,helens::RockResponse::ptr response
                        ,helens::RockStream::ptr stream);
    bool handleQuery(helens::RockRequest::ptr request
                        ,helens::RockResponse::ptr response
                        ,helens::RockStream::ptr stream);
    bool handleTick(helens::RockRequest::ptr request
                        ,helens::RockResponse::ptr response
                        ,helens::RockStream::ptr stream);

private:
    NSClientInfo::ptr get(helens::RockStream::ptr rs);
    void set(helens::RockStream::ptr rs, NSClientInfo::ptr info);

    void setQueryDomain(helens::RockStream::ptr rs, const std::set<std::string>& ds);

    void doNotify(std::set<std::string>& domains, std::shared_ptr<NotifyMessage> nty);

    std::set<helens::RockStream::ptr> getStreams(const std::string& domain);
private:
    NSDomainSet::ptr m_domains;

    helens::RWMutex m_mutex;
    std::map<helens::RockStream::ptr, NSClientInfo::ptr> m_sessions;

    /// sessoin 关注的域名
    std::map<helens::RockStream::ptr, std::set<std::string> > m_queryDomains;
    /// 域名对应关注的session
    std::map<std::string, std::set<helens::RockStream::ptr> > m_domainToSessions;
};

}
}

#endif
