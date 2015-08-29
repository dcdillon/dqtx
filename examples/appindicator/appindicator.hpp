#include <QtWidgets/QApplication>
#include <QString>
#include <dqtx/QAppIndicator.hpp>
#include <dqtx/QIconTheme.hpp>

class appindicator : public QObject
{
    Q_OBJECT
   private:
    dqtx::QIconTheme m_iconTheme;
    dqtx::QAppIndicator m_appIndicator;

   public:
    appindicator();
    void initialize();

   private slots:
    void onQuitClicked();
    void onTimeout();
};
