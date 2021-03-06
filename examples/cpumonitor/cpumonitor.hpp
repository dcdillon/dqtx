#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QString>
#include <cstdint>
#include <vector>
#include <map>
#include <dqtx/QSparkLineWidget.hpp>
#include <dqtx/QSparkLineAndBarsWidget.hpp>
#include <dqtx/QAppIndicator.hpp>
#include <dqtx/QIconTheme.hpp>

class cpu_info
{
   public:
    QString m_name;
    int64_t m_user;
    int64_t m_nice;
    int64_t m_system;
    int64_t m_idle;
    int64_t m_iowait;
    int64_t m_irq;
    int64_t m_softirq;
    dqtx::QSparkLineWidget *m_sparklineWidget;
    dqtx::QSparkLineAndBarsWidget *m_sparkLineAndBarsWidget;

    inline cpu_info()
        : m_user(0)
        , m_nice(0)
        , m_system(0)
        , m_idle(0)
        , m_iowait(0)
        , m_irq(0)
        , m_softirq(0)
        , m_sparklineWidget(nullptr)
        , m_sparkLineAndBarsWidget(nullptr)
    {
    }
};

class cpumonitor : public QObject
{
    Q_OBJECT
   private:
    QTableWidget m_table;
    dqtx::QIconTheme m_iconTheme;
    dqtx::QAppIndicator m_appIndicator;
    QList< double > m_cpuUsageHistory;
    std::map< std::string, cpu_info > m_cpuInfoByCPU;

   public:
    cpumonitor();
    ~cpumonitor();
    void initialize();

   protected:
    void read_proc_stat();

   private slots:
    void on_timeout();
};
