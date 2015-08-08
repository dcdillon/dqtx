#include "cpumonitor.hpp"

int main(int _argc, char *_argv[])
{
    cpumonitor monitor(_argc, _argv);
    monitor.run();
    return 0;
}
