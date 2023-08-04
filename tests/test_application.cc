#include "helens/application.h"

int main(int argc, char** argv) {
    helens::Application app;
    if(app.init(argc, argv)) {
        return app.run();
    }
    return 0;
}
