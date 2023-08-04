#include "helens/helens.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void test_fiber() {
    static int s_count = 5;
    HELENS_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    sleep(1);
    if(--s_count >= 0) {
        helens::Scheduler::GetThis()->schedule(&test_fiber, helens::GetThreadId());
    }
}

int main(int argc, char** argv) {
    HELENS_LOG_INFO(g_logger) << "main";
    helens::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    HELENS_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    HELENS_LOG_INFO(g_logger) << "over";
    return 0;
}
