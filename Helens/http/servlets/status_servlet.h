#ifndef __HELENS_HTTP_SERVLETS_STATUS_SERVLET_H__
#define __HELENS_HTTP_SERVLETS_STATUS_SERVLET_H__

#include "helens/http/servlet.h"

namespace helens {
namespace http {

class StatusServlet : public Servlet {
public:
    StatusServlet();
    virtual int32_t handle(helens::http::HttpRequest::ptr request
                   , helens::http::HttpResponse::ptr response
                   , helens::http::HttpSession::ptr session) override;
};

}
}

#endif
