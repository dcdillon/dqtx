#include "cpumonitor.hpp"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <QTimer>
#include <QColor>
#include <dqtx/QSparkLineIconFactory.hpp>

cpumonitor::cpumonitor()
    : m_appIndicator("dqtx-cpu-monitor", "", "", m_iconTheme.dir().absolutePath())
{
    read_proc_stat();
}

cpumonitor::~cpumonitor()
{
    for (auto &i : m_cpuInfoByCPU)
    {
        if (i.second.m_sparklineWidget)
        {
            delete i.second.m_sparklineWidget;
        }
        else if (i.second.m_sparkLineAndBarsWidget)
        {
            delete i.second.m_sparkLineAndBarsWidget;
        }
    }
}

void cpumonitor::initialize()
{
    m_table.setColumnCount(3);
    m_table.setRowCount(m_cpuInfoByCPU.size());
    m_table.show();
    
    dqtx::QAppIndicatorMenu *menu = new dqtx::QAppIndicatorMenu();
    m_appIndicator.setMenu(menu);
    
    int cpuIndex = 0;
    for (auto &i : m_cpuInfoByCPU)
    {
        m_table.setItem(
            cpuIndex, 0, new QTableWidgetItem(QString(i.first.c_str())));

        if (cpuIndex > 0)
        {
            i.second.m_sparklineWidget = new dqtx::QSparkLineWidget();
            i.second.m_sparklineWidget->setMinimum(0);
            i.second.m_sparklineWidget->setColor(QColor(Qt::blue));
            m_table.setCellWidget(cpuIndex, 2, i.second.m_sparklineWidget);
        }
        else
        {
            i.second.m_sparkLineAndBarsWidget =
                new dqtx::QSparkLineAndBarsWidget();
            i.second.m_sparkLineAndBarsWidget->setLineMinimum(0);
            i.second.m_sparkLineAndBarsWidget->setBarMinimum(0);
            i.second.m_sparkLineAndBarsWidget->setLineColor(
                QColor(Qt::blue));
            m_table.setCellWidget(
                cpuIndex, 2, i.second.m_sparkLineAndBarsWidget);
        }

        ++cpuIndex;
    }
    
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer->start();
}

void cpumonitor::on_timeout()
{
    read_proc_stat();
    
    static int32_t count = 0;
    
    QIcon sparkLineIcon = dqtx::QSparkLineIconFactory::create(m_cpuUsageHistory,
        0, 1, QColor(Qt::blue));
    
    if (count % 2)
    {
        m_iconTheme.addIcon(sparkLineIcon, "cpu-sparkline");
        m_appIndicator.setIconName("cpu-sparkline");
    }
    else
    {
        m_iconTheme.addIcon(sparkLineIcon, "cpu-sparkline-2");
        m_appIndicator.setIconName("cpu-sparkline-2");
    }
    ++count;
    m_appIndicator.show();
}

void cpumonitor::read_proc_stat()
{
    std::ifstream infile("/proc/stat");

    int cpuIndex = 0;
    double combinedIdlePercent = 1;
    int64_t procsRunning = 0;

    while (infile.good())
    {
        std::string line;
        std::getline(infile, line);

        char buf[16384];
        strcpy(buf, line.c_str());

        char *tok = strtok(buf, " ");
        std::string name;
        std::vector< std::string > values;

        if (tok)
        {
            name = tok;

            while ((tok = strtok(NULL, " ")))
            {
                values.push_back(tok);
            }
        }

        if (name.find(std::string("cpu")) == 0)
        {
            auto i = m_cpuInfoByCPU.find(name);

            if (i == m_cpuInfoByCPU.end())
            {
                cpu_info info;
                info.m_user = atol(values[0].c_str());
                info.m_nice = atol(values[1].c_str());
                info.m_system = atol(values[2].c_str());
                info.m_idle = atol(values[3].c_str());
                info.m_iowait = atol(values[4].c_str());
                info.m_irq = atol(values[5].c_str());
                info.m_softirq = atol(values[6].c_str());
                m_cpuInfoByCPU[name] = info;
            }
            else
            {
                const int64_t user = atol(values[0].c_str());
                const int64_t nice = atol(values[1].c_str());
                const int64_t system = atol(values[2].c_str());
                const int64_t idle = atol(values[3].c_str());
                const int64_t iowait = atol(values[4].c_str());
                const int64_t irq = atol(values[5].c_str());
                const int64_t softirq = atol(values[6].c_str());

                const int64_t delta_user = user - i->second.m_user;
                const int64_t delta_nice = nice - i->second.m_nice;
                const int64_t delta_system = system - i->second.m_system;
                const int64_t delta_idle = idle - i->second.m_idle;
                const int64_t delta_iowait = iowait - i->second.m_iowait;
                const int64_t delta_irq = irq - i->second.m_irq;
                const int64_t delta_softirq = softirq - i->second.m_softirq;

                const double totalDelta =
                    delta_user + delta_nice + delta_system + delta_idle +
                    delta_iowait + delta_irq + delta_softirq;

                const double idlePercent = delta_idle / totalDelta;

                if (cpuIndex > 0)
                {
                    i->second.m_sparklineWidget->insertObservation(1 -
                                                                   idlePercent);
                }
                else
                {
                    combinedIdlePercent = idlePercent;
                }

                i->second.m_user = user;
                i->second.m_nice = nice;
                i->second.m_system = system;
                i->second.m_idle = idle;
                i->second.m_iowait = iowait;
                i->second.m_irq = irq;
                i->second.m_softirq = softirq;

                m_table.setItem(
                    cpuIndex,
                    1,
                    new QTableWidgetItem(QString("%1%").arg(
                        QString::number((1 - idlePercent) * 100, 'f', 1))));
            }

            ++cpuIndex;
        }
        else if (name == "procs_running")
        {
            procsRunning = atol(values[0].c_str());
        }
    }

    if (m_cpuInfoByCPU["cpu"].m_sparkLineAndBarsWidget)
    {
        m_cpuInfoByCPU["cpu"].m_sparkLineAndBarsWidget->insertObservation(
            1 - combinedIdlePercent, procsRunning);
    }
    
    if (m_cpuUsageHistory.size() == 30)
    {
        m_cpuUsageHistory.pop_front();
    }
    
    m_cpuUsageHistory.push_back(1 - combinedIdlePercent);
}
