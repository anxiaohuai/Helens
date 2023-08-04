#ifndef __HELENS_HTTP_SERVLETS_CONFIG_SERVLET_H__
#define __HELENS_HTTP_SERVLETS_CONFIG_SERVLET_H__

#include "helens/http/servlet.h"

namespace helens {
namespace http {

class ConfigServlet : public Servlet {
public:
    ConfigServlet();
    virtual int32_t handle(helens::http::HttpRequest::ptr request
                   , helens::http::HttpResponse::ptr response
                   , helens::http::HttpSession::ptr session) override;
};

}
}

#endif
