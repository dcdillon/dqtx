#include <QtWidgets/QApplication>
#include <QString>
#include <dqtx/QAppIndicator.hpp>
#include <dqtx/QIconTheme.hpp>

class appindicator : public QObject
{
    Q_OBJECT
   private:
    QApplication m_application;
    dqtx::QIconTheme m_iconTheme;
    dqtx::QAppIndicator m_appIndicator;

   public:
    appindicator(int _argc, char *_argv[]);
    void run();

   private slots:
    void onQuitClicked();
    void onTimeout();
};
