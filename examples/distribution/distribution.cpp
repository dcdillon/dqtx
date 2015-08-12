#include "distribution.hpp"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <QTimer>
#include <QColor>

distribution::distribution(int _argc, char *_argv[]) : m_application(_argc, _argv), m_table(4, 2),
    m_normalRandNonzero(5.0)
{
    m_table.show();
}

void distribution::run()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer->start();

    m_application.exec();
}

void distribution::on_timeout()
{
    static bool firstTime = true;
    static int64_t count = 0;

    if (firstTime)
    {
        m_normalWidget = new dqtx::QDensityWidget();
        m_table.setItem(0, 0, new QTableWidgetItem(QString("Normal")));
        m_table.setCellWidget(0, 1, m_normalWidget);

        m_lognormalWidget = new dqtx::QDensityWidget();
        m_table.setItem(1, 0, new QTableWidgetItem(QString("Lognormal")));
        m_table.setCellWidget(1, 1, m_lognormalWidget);
        
        m_bernoulliWidget = new dqtx::QDensityWidget();
        m_table.setItem(2, 0, new QTableWidgetItem(QString("Bernoulli")));
        m_table.setCellWidget(2, 1, m_bernoulliWidget);
        
        m_bimodalWidget = new dqtx::QDensityWidget();
        m_table.setItem(3, 0, new QTableWidgetItem(QString("Bimodal")));
        m_table.setCellWidget(3, 1, m_bimodalWidget);
        
        firstTime = false;
    }

    double val = m_normalRand(m_generator);
    m_normalWidget->insertObservation(val);

    val = m_lognormalRand(m_generator);
    m_lognormalWidget->insertObservation(val);
    
    val = (int32_t)m_bernoulliRand(m_generator);
    m_bernoulliWidget->insertObservation(val);
    
    if (count++ % 2)
    {
        val = m_normalRand(m_generator);
        m_bimodalWidget->insertObservation(val);
        val = m_normalRandNonzero(m_generator);
        m_bimodalWidget->insertObservation(val);
    }
    else
    {
        val = m_normalRandNonzero(m_generator);
        m_bimodalWidget->insertObservation(val);
        val = m_normalRand(m_generator);
        m_bimodalWidget->insertObservation(val);
    }
}
