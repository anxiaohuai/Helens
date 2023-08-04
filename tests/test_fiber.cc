#include "helens/helens.h"

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void run_in_fiber() {
    HELENS_LOG_INFO(g_logger) << "run_in_fiber begin";
    helens::Fiber::YieldToHold();
    HELENS_LOG_INFO(g_logger) << "run_in_fiber end";
    helens::Fiber::YieldToHold();
}

void test_fiber() {
    HELENS_LOG_INFO(g_logger) << "main begin -1";
    {
        helens::Fiber::GetThis();
        HELENS_LOG_INFO(g_logger) << "main begin";
        helens::Fiber::ptr fiber(new helens::Fiber(run_in_fiber));
        fiber->swapIn();
        HELENS_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        HELENS_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    HELENS_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv) {
    helens::Thread::SetName("main");

    std::vector<helens::Thread::ptr> thrs;
    for(int i = 0; i < 3; ++i) {
        thrs.push_back(helens::Thread::ptr(
                    new helens::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs) {
        i->join();
    }
    return 0;
}
