#include "my_module.h"
#include "helens/config.h"
#include "helens/log.h"

namespace name_space {

static helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

MyModule::MyModule()
    :helens::Module("project_name", "1.0", "") {
}

bool MyModule::onLoad() {
    HELENS_LOG_INFO(g_logger) << "onLoad";
    return true;
}

bool MyModule::onUnload() {
    HELENS_LOG_INFO(g_logger) << "onUnload";
    return true;
}

bool MyModule::onServerReady() {
    HELENS_LOG_INFO(g_logger) << "onServerReady";
    return true;
}

bool MyModule::onServerUp() {
    HELENS_LOG_INFO(g_logger) << "onServerUp";
    return true;
}

}

extern "C" {

helens::Module* CreateModule() {
    helens::Module* module = new name_space::MyModule;
    HELENS_LOG_INFO(name_space::g_logger) << "CreateModule " << module;
    return module;
}

void DestoryModule(helens::Module* module) {
    HELENS_LOG_INFO(name_space::g_logger) << "CreateModule " << module;
    delete module;
}

}
