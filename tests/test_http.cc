#include "helens/http/http.h"
#include "helens/log.h"

void test_request() {
    helens::http::HttpRequest::ptr req(new helens::http::HttpRequest);
    req->setHeader("host" , "www.helens.top");
    req->setBody("hello helens");
    req->dump(std::cout) << std::endl;
}

void test_response() {
    helens::http::HttpResponse::ptr rsp(new helens::http::HttpResponse);
    rsp->setHeader("X-X", "helens");
    rsp->setBody("hello helens");
    rsp->setStatus((helens::http::HttpStatus)400);
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    test_request();
    test_response();
    return 0;
}
