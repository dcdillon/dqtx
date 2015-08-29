#include "appindicator.hpp"

int main(int _argc, char *_argv[])
{
    QApplication application(_argc, _argv);
    appindicator app;
    app.initialize();
    application.exec();
    return 0;
}
