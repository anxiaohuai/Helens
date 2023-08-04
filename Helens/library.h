#ifndef __HELENS_LIBRARY_H__
#define __HELENS_LIBRARY_H__

#include <memory>
#include "module.h"

namespace helens {

class Library {
public:
    static Module::ptr GetModule(const std::string& path);
};

}

#endif
