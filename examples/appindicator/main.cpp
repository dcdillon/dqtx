#include "appindicator.hpp"

int main(int _argc, char *_argv[])
{
    appindicator app(_argc, _argv);
    app.run();
    return 0;
}
