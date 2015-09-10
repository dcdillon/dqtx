#include <QObject>

class SignalProcessor : public QObject
{
    Q_OBJECT

public slots:
    void onUnixSignal(int _sig);
};

