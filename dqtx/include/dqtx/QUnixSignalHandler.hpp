#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <atomic>
#include <cstdint>

namespace dqtx
{
class QUnixSignalHandler : public QObject
{
    Q_OBJECT

   private:
    static std::atomic< int32_t > s_initializationCount;
    static int s_socketPair[2];

   private:
    QSocketNotifier *m_notifier;

   public:
    static void signalHandler(int _sig);

   public:
    QUnixSignalHandler();
    bool registerSignal(int _signal);

signals:
    void signal(int _signal);

   public slots:
    void onSignal(int _unused);
};

}  // namespace dqtx
