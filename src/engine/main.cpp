
#include "app.h"

App g_app;

int main(int argc, char *argv[]) {
    g_app.init(argc, argv);
    g_app.run();
    g_app.shutdown();
    return 0;
}

