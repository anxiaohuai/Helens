#include "helens/helens.h"
#include "helens/ds/dict.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

struct PidVid {
    PidVid(uint32_t p = 0, uint32_t v = 0)
        :pid(p), vid(v) {}
    uint32_t pid;
    uint32_t vid;

    bool operator<(const PidVid& o) const {
        return memcmp(this, &o, sizeof(o)) < 0;
    }
};

void gen() {
    helens::ds::Dict<int, PidVid> tmp;
    for(int i = 0; i < 500000; ++i) {
        int32_t len = rand() % 10 + 5;
        int k = rand();
        std::vector<PidVid> pvs;
        for(int n = 0; n < len; ++n) {
            pvs.push_back(PidVid(rand(), rand()));
        }
        tmp.insert(k, pvs.data(), pvs.size());
    }

    std::ofstream ofs("./dict.data");
    tmp.writeTo(ofs);
}

void test() {
    for(int i = 0; i < 10000; ++i) {
        HELENS_LOG_INFO(g_logger) << "i=" << i;
        std::ifstream ifs("./dict.data");
        helens::ds::Dict<int, PidVid> tmp;
        if(!tmp.readFrom(ifs)) {
            HELENS_LOG_INFO(g_logger) << "error";
        }
        if(i % 100 == 0) {
            HELENS_LOG_INFO(g_logger) << "over..." << (i + 1);
        }
    }
}

int main(int argc, char** argv) {
    gen();
    test();
    return 0;
}
