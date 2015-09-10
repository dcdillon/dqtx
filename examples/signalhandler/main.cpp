#include <QApplication>
#include <QObject>
#include <dqtx/QUnixSignalHandler.hpp>
#include <sys/signal.h>
#include <iostream>
#include "SignalProcessor.hpp"

int main(int _argc, char *_argv[])
{
    QApplication application(_argc, _argv);
    dqtx::QUnixSignalHandler signalHandler;
    SignalProcessor processor;

    QObject::connect(&signalHandler, SIGNAL(signal(int)), &processor, SLOT(onUnixSignal(int)));
    signalHandler.registerSignal(SIGINT);
    signalHandler.registerSignal(SIGTERM);
    signalHandler.registerSignal(SIGHUP);

    
    application.exec();
    return 0;
}
