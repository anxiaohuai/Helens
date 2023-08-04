#include "status_servlet.h"
#include "helens/helens.h"

namespace helens {
namespace http {

StatusServlet::StatusServlet()
    :Servlet("StatusServlet") {
}

std::string format_used_time(int64_t ts) {
    std::stringstream ss;
    bool v = false;
    if(ts >= 3600 * 24) {
        ss << (ts / 3600 / 24) << "d ";
        ts = ts % (3600 * 24);
        v = true;
    }
    if(ts >= 3600) {
        ss << (ts / 3600) << "h ";
        ts = ts % 3600;
        v = true;
    } else if(v) {
        ss << "0h ";
    }

    if(ts >= 60) {
        ss << (ts / 60) << "m ";
        ts = ts % 60;
    } else if(v) {
        ss << "0m ";
    }
    ss << ts << "s";
    return ss.str();
}

int32_t StatusServlet::handle(helens::http::HttpRequest::ptr request
                              ,helens::http::HttpResponse::ptr response
                              ,helens::http::HttpSession::ptr session) {
    response->setHeader("Content-Type", "text/text; charset=utf-8");
#define XX(key) \
    ss << std::setw(30) << std::right << key ": "
    std::stringstream ss;
    ss << "===================================================" << std::endl;
    XX("server_version") << "helens/1.0.0" << std::endl;
    
    std::vector<Module::ptr> ms;
    ModuleMgr::GetInstance()->listAll(ms);

    XX("modules");
    for(size_t i = 0; i < ms.size(); ++i) {
        if(i) {
            ss << ";";
        }
        ss << ms[i]->getId();
    }
    ss << std::endl;
    XX("host") << GetHostName() << std::endl;
    XX("ipv4") << GetIPv4() << std::endl;
    XX("daemon_id") << ProcessInfoMgr::GetInstance()->parent_id << std::endl;
    XX("main_id") << ProcessInfoMgr::GetInstance()->main_id << std::endl;
    XX("daemon_start") << Time2Str(ProcessInfoMgr::GetInstance()->parent_start_time) << std::endl;
    XX("main_start") << Time2Str(ProcessInfoMgr::GetInstance()->main_start_time) << std::endl;
    XX("restart_count") << ProcessInfoMgr::GetInstance()->restart_count << std::endl;
    XX("daemon_running_time") << format_used_time(time(0) - ProcessInfoMgr::GetInstance()->parent_start_time) << std::endl;
    XX("main_running_time") << format_used_time(time(0) - ProcessInfoMgr::GetInstance()->main_start_time) << std::endl;
    ss << "===================================================" << std::endl;
    XX("fibers") << helens::Fiber::TotalFibers() << std::endl;
    ss << "===================================================" << std::endl;
    ss << "<Logger>" << std::endl;
    ss << helens::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    ss << "===================================================" << std::endl;
    ss << "<Woker>" << std::endl;
    helens::WorkerMgr::GetInstance()->dump(ss) << std::endl;

    std::map<std::string, std::vector<TcpServer::ptr> > servers;
    helens::Application::GetInstance()->listAllServer(servers);
    ss << "===================================================" << std::endl;
    for(auto it = servers.begin();
            it != servers.end(); ++it) {
        if(it != servers.begin()) {
            ss << "***************************************************" << std::endl;
        }
        ss << "<Server." << it->first << ">" << std::endl;
        helens::http::HttpServer::ptr hs;
        for(auto iit = it->second.begin();
                iit != it->second.end(); ++iit) {
            if(iit != it->second.begin()) {
                ss << "---------------------------------------------------" << std::endl;
            }
            if(!hs) {
                hs = std::dynamic_pointer_cast<helens::http::HttpServer>(*iit);
            }
            ss << (*iit)->toString() << std::endl;
        }
        if(hs) {
            auto sd = hs->getServletDispatch();
            if(sd) {
                std::map<std::string, IServletCreator::ptr> infos;
                sd->listAllServletCreator(infos);
                if(!infos.empty()) {
                    ss << "[Servlets]" << std::endl;
#define XX2(key) \
    ss << std::setw(30) << std::right << key << ": "
                    for(auto& i : infos) {
                        XX2(i.first) << i.second->getName() << std::endl;
                    }
                    infos.clear();
                }
                sd->listAllGlobServletCreator(infos);
                if(!infos.empty()) {
                    ss << "[Servlets.Globs]" << std::endl;
                    for(auto& i : infos) {
                        XX2(i.first) << i.second->getName() << std::endl;
                    }
                    infos.clear();
                }
            }
        }
    }
    ss << "===================================================" << std::endl;
    for(size_t i = 0; i < ms.size(); ++i) {
        if(i) {
            ss << "***************************************************" << std::endl;
        }
        ss << ms[i]->statusString() << std::endl;
    }

    response->setBody(ss.str());
    return 0;
}

}
}
