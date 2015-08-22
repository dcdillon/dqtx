#include <QtWidgets/QApplication>
#include <QString>
#include <dqtx/QAppIndicator.hpp>

class appindicator : public QObject
{
    Q_OBJECT
   private:
    QApplication m_application;
    dqtx::QAppIndicator m_appIndicator;
    
   public:
    appindicator(int _argc, char *_argv[]);
    void run();

   private slots:
    void onQuitClicked(QString _label, GtkWidget *_item);
    void onTimeout();
};