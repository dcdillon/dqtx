#include "appindicator.hpp"
#include <QTimer>

appindicator::appindicator(int _argc, char *_argv[]) : m_application(_argc, _argv),
    m_appIndicator("appindicator-example", "applications-chat-panel", "")
{
    m_appIndicator.addMenuItem("Quit");
    connect(&m_appIndicator, SIGNAL(menuItemActivated(QString, GtkWidget *)),
        this, SLOT(onQuitClicked(QString, GtkWidget *)));
}

void appindicator::run()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    m_application.exec();
}

void appindicator::onQuitClicked(QString _label, GtkWidget *_item)
{
    if (_label == "Quit")
    {
        m_application.quit();
    }
}

void appindicator::onTimeout()
{
    static int32_t count = 0;
    static int32_t iconCount = 0;
    
    if (count % 2)
    {
        if (iconCount % 2)
        {
            m_appIndicator.setIconName("applications-chat-panel");
        }
        else
        {
            m_appIndicator.setIconName("applications-email-panel");
        }
        
        m_appIndicator.setLabel(QString::number(iconCount));
        
        m_appIndicator.show();
        ++iconCount;
    }
    else
    {
        m_appIndicator.hide();
    }
    
    count++;
}