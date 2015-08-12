#include "distribution.hpp"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <QTimer>
#include <QColor>

distribution::distribution(int _argc, char *_argv[]) : m_application(_argc, _argv)
{
}

void distribution::run()
{
    m_table.setColumnCount(2);
    m_table.setRowCount(2);
    m_table.show();

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer->start();

    m_application.exec();
}

void distribution::on_timeout()
{
    static bool firstTime = true;

    if (firstTime)
    {
        m_normalWidget = new dqtx::QDensityWidget();
        m_table.setItem(0, 0, new QTableWidgetItem(QString("Normal")));
        m_table.setCellWidget(0, 1, m_normalWidget);

        m_lognormalWidget = new dqtx::QDensityWidget();
        m_table.setItem(1, 0, new QTableWidgetItem(QString("Lognormal")));
        m_table.setCellWidget(1, 1, m_lognormalWidget);
        firstTime = false;
    }

    double val = m_normalRand(m_generator);
    m_normalWidget->insertObservation(val);

    val = m_lognormalRand(m_generator);
    m_lognormalWidget->insertObservation(val);
}
