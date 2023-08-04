#include "helens/daemon.h"
#include "helens/iomanager.h"
#include "helens/log.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

helens::Timer::ptr timer;
int server_main(int argc, char** argv) {
    HELENS_LOG_INFO(g_logger) << helens::ProcessInfoMgr::GetInstance()->toString();
    helens::IOManager iom(1);
    timer = iom.addTimer(1000, [](){
            HELENS_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if(++count > 10) {
                exit(1);
            }
    }, true);
    return 0;
}

int main(int argc, char** argv) {
    return helens::start_daemon(argc, argv, server_main, argc != 1);
}
