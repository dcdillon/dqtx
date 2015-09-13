#include <dqtx/QUnixSignalHandler.hpp>
#include <sys/socket.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdexcept>

namespace dqtx
{
std::atomic< int32_t > QUnixSignalHandler::s_initializationCount(0);
int QUnixSignalHandler::s_socketPair[2];

void QUnixSignalHandler::signalHandler(int _sig)
{
    Q_UNUSED(::write(s_socketPair[0], &_sig, sizeof(_sig)));
}

QUnixSignalHandler::QUnixSignalHandler() : m_notifier(nullptr)
{
    int32_t expected = 0;

    if (!s_initializationCount.compare_exchange_strong(expected, 1))
    {
        throw std::runtime_error(
            "Only one instance of QUnixSignalHandler per process.");
    }

    ::socketpair(AF_UNIX, SOCK_STREAM, 0, s_socketPair);
    m_notifier =
        new QSocketNotifier(s_socketPair[1], QSocketNotifier::Read, this);
    QObject::connect(
        m_notifier, SIGNAL(activated(int)), this, SLOT(onSignal(int)));
}

bool QUnixSignalHandler::registerSignal(int _signal)
{
    struct sigaction sig;
    sig.sa_handler = QUnixSignalHandler::signalHandler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags |= SA_RESTART;

    if (sigaction(_signal, &sig, 0) > 0)
    {
        return false;
    }

    return true;
}

void QUnixSignalHandler::onSignal(int _unused)
{
    Q_UNUSED(_unused);

    ssize_t status;
    int sig;

    do
    {
        status = ::read(s_socketPair[1], &sig, sizeof(sig));
    } while (status == -1 && errno == EINTR);

    if (status == sizeof(sig))
    {
        emit signal(sig);
    }
}

}  // namespace dqtx
