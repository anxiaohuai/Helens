#include "helens/zk_client.h"
#include "helens/log.h"
#include "helens/iomanager.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

int g_argc;

void on_watcher(int type, int stat, const std::string& path, helens::ZKClient::ptr client) {
    HELENS_LOG_INFO(g_logger) << " type=" << type
        << " stat=" << stat
        << " path=" << path
        << " client=" << client
        << " fiber=" << helens::Fiber::GetThis()
        << " iomanager=" << helens::IOManager::GetThis();

    if(stat == ZOO_CONNECTED_STATE) {
        if(g_argc == 1) {
            std::vector<std::string> vals;
            Stat stat;
            int rt = client->getChildren("/", vals, true, &stat);
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "[" << helens::Join(vals.begin(), vals.end(), ",") << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "getChildren error " << rt;
            }
        } else {
            std::string new_val;
            new_val.resize(255);
            int rt = client->create("/zkxxx", "", new_val, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL);
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "[" << new_val.c_str() << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "getChildren error " << rt;
            }

//extern ZOOAPI const int ZOO_SEQUENCE;
//extern ZOOAPI const int ZOO_CONTAINER;
            rt = client->create("/zkxxx", "", new_val, &ZOO_OPEN_ACL_UNSAFE, ZOO_SEQUENCE | ZOO_EPHEMERAL);
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "create [" << new_val.c_str() << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "create error " << rt;
            }

            rt = client->get("/hello", new_val, true);
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "get [" << new_val.c_str() << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "get error " << rt;
            }

            rt = client->create("/hello", "", new_val, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL);
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "get [" << new_val.c_str() << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "get error " << rt;
            }

            rt = client->set("/hello", "xxx");
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "set [" << new_val.c_str() << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "set error " << rt;
            }

            rt = client->del("/hello");
            if(rt == ZOK) {
                HELENS_LOG_INFO(g_logger) << "del [" << new_val.c_str() << "]";
            } else {
                HELENS_LOG_INFO(g_logger) << "del error " << rt;
            }

        }
    } else if(stat == ZOO_EXPIRED_SESSION_STATE) {
        client->reconnect();
    }
}

int main(int argc, char** argv) {
    g_argc = argc;
    helens::IOManager iom(1);
    helens::ZKClient::ptr client(new helens::ZKClient);
    if(g_argc > 1) {
        HELENS_LOG_INFO(g_logger) << client->init("127.0.0.1:21811", 3000, on_watcher);
        //HELENS_LOG_INFO(g_logger) << client->init("127.0.0.1:21811,127.0.0.1:21812,127.0.0.1:21811", 3000, on_watcher);
        iom.addTimer(1115000, [client](){client->close();});
    } else {
        HELENS_LOG_INFO(g_logger) << client->init("127.0.0.1:21811,127.0.0.1:21812,127.0.0.1:21811", 3000, on_watcher);
        iom.addTimer(5000, [](){}, true);
    }
    iom.stop();
    return 0;
}
