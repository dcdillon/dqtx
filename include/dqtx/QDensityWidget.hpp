/* Copyright (c) 2015, Daniel C. Dillon
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <QWidget>
#include <QMutex>
#include <QColor>
#include <QList>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/mean.hpp>

namespace dqtx
{
class QDensityWidget : public QWidget
{
    Q_OBJECT
   public:
    QDensityWidget(QWidget *_parent = 0, Qt::WindowFlags _flags = 0);

   public:
    void insertObservation(const double _data);
    void setColor(const QColor &_color);
    void setPadding(const int _padding);
    void setMaxObservations(const int _max);

   public:
    QSize sizeHint() const;

   protected:
    void paintEvent(QPaintEvent *_event);

   private:
    double density(double _x);
    double normal(double _x);

   private:
    void drawLine(QPainter &_painter,
                  const QRect &_rect,
                  int _leftPadding,
                  int _rightPadding,
                  int _topPadding,
                  int _bottomPadding);

   private:
    QList< double > m_data;
    boost::accumulators::accumulator_set<
        double,
        boost::accumulators::stats< boost::accumulators::tag::variance,
                                    boost::accumulators::tag::mean > >
        m_accumulator;
    QColor m_color;
    int m_padding;
    double m_bandwidth;
    int m_maxObservations;

signals:
    void observationInserted(double _obs);
    void colorChanged(QColor _color);
    void paddingChanged(int _padding);
    void maxObservationsChanged(int _max);

   private slots:
    void onObservationInserted(double _obs);
    void onColorChanged(QColor _color);
    void onPaddingChanged(int _padding);
    void onMaxObservationsChanged(int _max);

   private:
    void updateBandwidth();
    void updateToolTip();
};
}  // namespace dqtx
