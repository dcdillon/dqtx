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

namespace dqtx
{
class QSparkLineAndBarsWidget : public QWidget
{
    Q_OBJECT
   public:
    QSparkLineAndBarsWidget(QWidget *_parent = 0, Qt::WindowFlags _flags = 0);

   public:
    void insertObservation(const double _lineData, const double _barData);

    void setMinLineRange(const double _range);
    void setLineMinimum(const double _min);
    void setMinBarRange(const double _range);
    void setBarMinimum(const double _min);
    void setLineColor(const QColor &_color);
    void setBarColor(const QColor &_color);
    void setLeftPadding(const int _padding);
    void setRightPadding(const int _padding);
    void setTopPadding(const int _padding);
    void setBottomPadding(const int _padding);
    void setPadding(const int _left,
                    const int _right,
                    const int _top,
                    const int _bottom);
    void setBarHeightRatio(const double _ratio);
    void setMaxObservations(const int _max);

   public:
    QSize sizeHint() const;

   protected:
    void paintEvent(QPaintEvent *_event);

   private:
    void drawLine(const QList< double > &_data,
                  QPainter &_painter,
                  const QRect &_rect,
                  int _leftPadding,
                  int _rightPadding,
                  int _topPadding,
                  int _bottomPadding);
    void drawBars(const QList< double > &_data,
                  QPainter &_painter,
                  const QRect &_rect,
                  int _leftPadding,
                  int _rightPadding,
                  int _topPadding,
                  int _bottomPadding);

   private:
    QList< double > m_lineData;
    QList< double > m_barData;

    double m_minLineRange;
    double m_lineMinimum;
    double m_minBarRange;
    double m_barMinimum;

    QColor m_lineColor;
    QColor m_barColor;

    int m_leftPadding;
    int m_rightPadding;
    int m_topPadding;
    int m_bottomPadding;

    int m_maxObservations;
    double m_barHeightRatio;

signals:
    void observationInserted(double _line, double _bar);
    void minLineRangeChanged(double _range);
    void lineMinimumChanged(double _min);
    void minBarRangeChanged(double _range);
    void barMinimumChanged(double _min);
    void lineColorChanged(QColor _color);
    void barColorChanged(QColor _color);
    void leftPaddingChanged(int _padding);
    void rightPaddingChanged(int _padding);
    void topPaddingChanged(int _padding);
    void bottomPaddingChanged(int _padding);
    void paddingChanged(int _left, int _right, int _top, int _bottom);
    void maxObservationsChanged(int _max);
    void barHeightRatioChanged(double _ratio);

   private slots:
    void onObservationInserted(double _line, double _bar);
    void onMinLineRangeChanged(double _range);
    void onLineMinimumChanged(double _min);
    void onMinBarRangeChanged(double _range);
    void onBarMinimumChanged(double _min);
    void onLineColorChanged(QColor _color);
    void onBarColorChanged(QColor _color);
    void onLeftPaddingChanged(int _padding);
    void onRightPaddingChanged(int _padding);
    void onTopPaddingChanged(int _padding);
    void onBottomPaddingChanged(int _padding);
    void onPaddingChanged(int _left, int _right, int _top, int _bottom);
    void onMaxObservationsChanged(int _max);
    void onBarHeightRatioChanged(double _ratio);
};
}  // namespace dqtx
