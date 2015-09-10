#include "SignalProcessor.hpp"
#include <QApplication>
#include <QObject>
#include <iostream>

void SignalProcessor::onUnixSignal(int _sig)
{
    std::cout << "Received signal: " << _sig << std::endl;
    QApplication::quit();
}
