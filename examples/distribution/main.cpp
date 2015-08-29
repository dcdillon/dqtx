#include "distribution.hpp"

int main(int _argc, char *_argv[])
{
    QApplication application(_argc, _argv);
    distribution app;
    app.initialize();
    application.exec();
    return 0;
}
