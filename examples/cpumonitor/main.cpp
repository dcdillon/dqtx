#include "cpumonitor.hpp"

int main(int _argc, char *_argv[])
{
    QApplication application(_argc, _argv);
    cpumonitor monitor;
    monitor.initialize();
    application.exec();
    return 0;
}
