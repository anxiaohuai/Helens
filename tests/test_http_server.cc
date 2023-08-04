#include "helens/http/http_server.h"
#include "helens/log.h"

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

#define XX(...) #__VA_ARGS__


helens::IOManager::ptr worker;
void run() {
    g_logger->setLevel(helens::LogLevel::INFO);
    //helens::http::HttpServer::ptr server(new helens::http::HttpServer(true, worker.get(), helens::IOManager::GetThis()));
    helens::http::HttpServer::ptr server(new helens::http::HttpServer(true));
    helens::Address::ptr addr = helens::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/helens/xx", [](helens::http::HttpRequest::ptr req
                ,helens::http::HttpResponse::ptr rsp
                ,helens::http::HttpSession::ptr session) {
            rsp->setBody(req->toString());
            return 0;
    });

    sd->addGlobServlet("/helens/*", [](helens::http::HttpRequest::ptr req
                ,helens::http::HttpResponse::ptr rsp
                ,helens::http::HttpSession::ptr session) {
            rsp->setBody("Glob:\r\n" + req->toString());
            return 0;
    });

    sd->addGlobServlet("/helensx/*", [](helens::http::HttpRequest::ptr req
                ,helens::http::HttpResponse::ptr rsp
                ,helens::http::HttpSession::ptr session) {
            rsp->setBody(XX(<html>
<head><title>404 Not Found</title></head>
<body>
<center><h1>404 Not Found</h1></center>
<hr><center>nginx/1.16.0</center>
</body>
</html>
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
));
            return 0;
    });

    server->start();
}

int main(int argc, char** argv) {
    helens::IOManager iom(1, true, "main");
    worker.reset(new helens::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
