#include "helens/uri.h"
#include <iostream>

int main(int argc, char** argv) {
    //helens::Uri::ptr uri = helens::Uri::Create("http://www.helens.top/test/uri?id=100&name=helens#frg");
    //helens::Uri::ptr uri = helens::Uri::Create("http://admin@www.helens.top/test/中文/uri?id=100&name=helens&vv=中文#frg中文");
    helens::Uri::ptr uri = helens::Uri::Create("http://admin@www.helens.top");
    //helens::Uri::ptr uri = helens::Uri::Create("http://www.helens.top/test/uri");
    std::cout << uri->toString() << std::endl;
    auto addr = uri->createAddress();
    std::cout << *addr << std::endl;
    return 0;
}
