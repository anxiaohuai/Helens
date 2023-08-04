#include "helens/env.h"
#include <unistd.h>
#include <iostream>
#include <fstream>

struct A {
    A() {
        std::ifstream ifs("/proc/" + std::to_string(getpid()) + "/cmdline", std::ios::binary);
        std::string content;
        content.resize(4096);

        ifs.read(&content[0], content.size());
        content.resize(ifs.gcount());

        for(size_t i = 0; i < content.size(); ++i) {
            std::cout << i << " - " << content[i] << " - " << (int)content[i] << std::endl;
        }
    }
};

A a;

int main(int argc, char** argv) {
    std::cout << "argc=" << argc << std::endl;
    helens::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    helens::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    helens::EnvMgr::GetInstance()->addHelp("p", "print help");
    if(!helens::EnvMgr::GetInstance()->init(argc, argv)) {
        helens::EnvMgr::GetInstance()->printHelp();
        return 0;
    }

    std::cout << "exe=" << helens::EnvMgr::GetInstance()->getExe() << std::endl;
    std::cout << "cwd=" << helens::EnvMgr::GetInstance()->getCwd() << std::endl;

    std::cout << "path=" << helens::EnvMgr::GetInstance()->getEnv("PATH", "xxx") << std::endl;
    std::cout << "test=" << helens::EnvMgr::GetInstance()->getEnv("TEST", "") << std::endl;
    std::cout << "set env " << helens::EnvMgr::GetInstance()->setEnv("TEST", "yy") << std::endl;
    std::cout << "test=" << helens::EnvMgr::GetInstance()->getEnv("TEST", "") << std::endl;
    if(helens::EnvMgr::GetInstance()->has("p")) {
        helens::EnvMgr::GetInstance()->printHelp();
    }
    return 0;
}
