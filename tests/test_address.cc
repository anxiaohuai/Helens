#include "helens/address.h"
#include "helens/log.h"

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void test() {
    std::vector<helens::Address::ptr> addrs;

    HELENS_LOG_INFO(g_logger) << "begin";
    bool v = helens::Address::Lookup(addrs, "localhost:3080");
    //bool v = helens::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    //bool v = helens::Address::Lookup(addrs, "www.helens.top", AF_INET);
    HELENS_LOG_INFO(g_logger) << "end";
    if(!v) {
        HELENS_LOG_ERROR(g_logger) << "lookup fail";
        return;
    }

    for(size_t i = 0; i < addrs.size(); ++i) {
        HELENS_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }

    auto addr = helens::Address::LookupAny("localhost:4080");
    if(addr) {
        HELENS_LOG_INFO(g_logger) << *addr;
    } else {
        HELENS_LOG_ERROR(g_logger) << "error";
    }
}

void test_iface() {
    std::multimap<std::string, std::pair<helens::Address::ptr, uint32_t> > results;

    bool v = helens::Address::GetInterfaceAddresses(results);
    if(!v) {
        HELENS_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
        return;
    }

    for(auto& i: results) {
        HELENS_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
            << i.second.second;
    }
}

void test_ipv4() {
    //auto addr = helens::IPAddress::Create("www.helens.top");
    auto addr = helens::IPAddress::Create("127.0.0.8");
    if(addr) {
        HELENS_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char** argv) {
    //test_ipv4();
    //test_iface();
    test();
    return 0;
}
