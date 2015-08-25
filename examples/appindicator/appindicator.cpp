#include "appindicator.hpp"
#include <QTimer>
#include <dqtx/QTextIconFactory.hpp>

appindicator::appindicator(int _argc, char *_argv[])
    : m_application(_argc, _argv)
    , m_appIndicator("appindicator-example",
                     "applications-chat-panel",
                     "",
                     m_iconTheme.dir().absolutePath())
{
    dqtx::QAppIndicatorMenu *menu = new dqtx::QAppIndicatorMenu();
    dqtx::QAppIndicatorMenuItem *item = new dqtx::QAppIndicatorMenuItem("Quit");
    menu->addMenuItem(item);
    item->show();
    m_appIndicator.setMenu(menu);
    connect(item,
            SIGNAL(menuItemActivated()),
            this,
            SLOT(onQuitClicked()),
            Qt::QueuedConnection);
}

void appindicator::run()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    m_application.exec();
}

void appindicator::onQuitClicked() { QApplication::quit(); }

void appindicator::onTimeout()
{
    static int32_t count = 0;
    static int32_t iconCount = 0;

    if (count % 2)
    {
        m_iconTheme.addIcon(dqtx::QTextIconFactory::create(
                                QString::number(iconCount) + QString("k")),
                            QString::number(iconCount));
        m_appIndicator.setIconName(QString::number(iconCount));

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
