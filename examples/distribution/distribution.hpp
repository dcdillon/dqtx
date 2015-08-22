#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QString>
#include <cstdint>
#include <vector>
#include <map>
#include <dqtx/QDensityWidget.hpp>
#include <random>

class distribution : public QObject
{
    Q_OBJECT
   private:
    QApplication m_application;
    QTableWidget m_table;
    dqtx::QDensityWidget *m_normalWidget;
    dqtx::QDensityWidget *m_lognormalWidget;
    dqtx::QDensityWidget *m_bernoulliWidget;
    dqtx::QDensityWidget *m_bimodalWidget;

    std::default_random_engine m_generator;
    std::normal_distribution< double > m_normalRand;
    std::normal_distribution< double > m_normalRandNonzero;
    std::lognormal_distribution< double > m_lognormalRand;
    std::bernoulli_distribution m_bernoulliRand;

   public:
    distribution(int _argc, char *_argv[]);
    void run();

   private slots:
    void on_timeout();
};
