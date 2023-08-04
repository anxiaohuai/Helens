#include "helens/http/ws_connection.h"
#include "helens/iomanager.h"
#include "helens/util.h"

void run() {
    auto rt = helens::http::WSConnection::Create("http://127.0.0.1:8020/helens", 1000);
    if(!rt.second) {
        std::cout << rt.first->toString() << std::endl;
        return;
    }

    auto conn = rt.second;
    while(true) {
        //for(int i = 0; i < 1100; ++i) {
        for(int i = 0; i < 1; ++i) {
            conn->sendMessage(helens::random_string(60), helens::http::WSFrameHead::TEXT_FRAME, false);
        }
        conn->sendMessage(helens::random_string(65), helens::http::WSFrameHead::TEXT_FRAME, true);
        auto msg = conn->recvMessage();
        if(!msg) {
            break;
        }
        std::cout << "opcode=" << msg->getOpcode()
                  << " data=" << msg->getData() << std::endl;

        sleep(10);
    }
}

int main(int argc, char** argv) {
    srand(time(0));
    helens::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
