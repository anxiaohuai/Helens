#include "helens/module.h"
#include "helens/singleton.h"
#include <iostream>
#include "helens/log.h"
#include "helens/db/redis.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

class A {
public:
    A() {
        std::cout << "A::A " << this << std::endl;
    }

    ~A() {
        std::cout << "A::~A " << this << std::endl;
    }

};

class MyModule : public helens::RockModule {
public:
    MyModule()
        :RockModule("hello", "1.0", "") {
        //helens::Singleton<A>::GetInstance();
    }

    bool onLoad() override {
        helens::Singleton<A>::GetInstance();
        std::cout << "-----------onLoad------------" << std::endl;
        return true;
    }

    bool onUnload() override {
        helens::Singleton<A>::GetInstance();
        std::cout << "-----------onUnload------------" << std::endl;
        return true;
    }

    bool onServerReady() {
        registerService("rock", "helens.top", "blog");
        auto rpy = helens::RedisUtil::Cmd("local", "get abc");
        if(!rpy) {
            HELENS_LOG_ERROR(g_logger) << "redis cmd get abc error";
        } else {
            HELENS_LOG_ERROR(g_logger) << "redis get abc: "
                << (rpy->str ? rpy->str : "(null)");
        }
        return true;
    }

    bool handleRockRequest(helens::RockRequest::ptr request
                        ,helens::RockResponse::ptr response
                        ,helens::RockStream::ptr stream) {
        //HELENS_LOG_INFO(g_logger) << "handleRockRequest " << request->toString();
        //sleep(1);
        response->setResult(0);
        response->setResultStr("ok");
        response->setBody("echo: " + request->getBody());

        usleep(100 * 1000);
        auto addr = stream->getLocalAddressString();
        if(addr.find("8061") != std::string::npos) {
            if(rand() % 100 < 50) {
                usleep(10 * 1000);
            } else if(rand() % 100 < 10) {
                response->setResult(-1000);
            }
        } else {
            //if(rand() % 100 < 25) {
            //    usleep(10 * 1000);
            //} else if(rand() % 100 < 10) {
            //    response->setResult(-1000);
            //}
        }
        return true;
        //return rand() % 100 < 90;
    }

    bool handleRockNotify(helens::RockNotify::ptr notify 
                        ,helens::RockStream::ptr stream) {
        HELENS_LOG_INFO(g_logger) << "handleRockNotify " << notify->toString();
        return true;
    }

};

extern "C" {

helens::Module* CreateModule() {
    helens::Singleton<A>::GetInstance();
    std::cout << "=============CreateModule=================" << std::endl;
    return new MyModule;
}

void DestoryModule(helens::Module* ptr) {
    std::cout << "=============DestoryModule=================" << std::endl;
    delete ptr;
}

}
