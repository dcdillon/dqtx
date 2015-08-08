#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QString>
#include <cstdint>
#include <vector>
#include <map>
#include <dqtx/QSparkLineWidget.hpp>

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
};

class cpumonitor : public QObject
{
    Q_OBJECT
   private:
    QApplication m_application;
    QTableWidget m_table;
    std::map< std::string, cpu_info > m_cpuInfoByCPU;

   public:
    cpumonitor(int _argc, char *_argv[]);
    void run();

   protected:
    void read_proc();

   private slots:
    void on_timeout();
};
