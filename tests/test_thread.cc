#include "helens/helens.h"
#include <unistd.h>

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

int count = 0;
//helens::RWMutex s_mutex;
helens::Mutex s_mutex;

void fun1() {
    HELENS_LOG_INFO(g_logger) << "name: " << helens::Thread::GetName()
                             << " this.name: " << helens::Thread::GetThis()->getName()
                             << " id: " << helens::GetThreadId()
                             << " this.id: " << helens::Thread::GetThis()->getId();

    for(int i = 0; i < 100000; ++i) {
        //helens::RWMutex::WriteLock lock(s_mutex);
        helens::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        HELENS_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        HELENS_LOG_INFO(g_logger) << "========================================";
    }
}

int main(int argc, char** argv) {
    HELENS_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/helens/test/helens/bin/conf/log2.yml");
    helens::Config::LoadFromYaml(root);

    std::vector<helens::Thread::ptr> thrs;
    for(int i = 0; i < 1; ++i) {
        helens::Thread::ptr thr(new helens::Thread(&fun2, "name_" + std::to_string(i * 2)));
        //helens::Thread::ptr thr2(new helens::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        //thrs.push_back(thr2);
    }

    for(size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
    HELENS_LOG_INFO(g_logger) << "thread test end";
    HELENS_LOG_INFO(g_logger) << "count=" << count;

    return 0;
}
