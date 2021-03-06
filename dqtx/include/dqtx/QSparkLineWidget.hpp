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
class QSparkLineWidget : public QWidget
{
    Q_OBJECT
   public:
    enum DisplayType
    {
        DisplayTypeLine = 1,
        DisplayTypeBars = 2
    };

   public:
    QSparkLineWidget(QWidget *_parent = 0, Qt::WindowFlags _flags = 0);

   public:
    void insertObservation(const double _data);
    void setDisplayType(const DisplayType _type);
    void setMinRange(const double _range);
    void setMinimum(const double _min);
    void setColor(const QColor &_color);
    void setPadding(const int _padding);
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
    QList< double > m_data;
    DisplayType m_displayType;
    double m_minRange;
    double m_minimum;
    QColor m_color;
    int m_padding;
    int m_maxObservations;

signals:
    void observationInserted(double _obs);
    void displayTypeChanged(int _type);
    void minRangeChanged(double _range);
    void minimumChanged(double _min);
    void colorChanged(QColor _color);
    void paddingChanged(int _padding);
    void maxObservationsChanged(int _max);

   private slots:
    void onObservationInserted(double _obs);
    void onDisplayTypeChanged(int _type);
    void onMinRangeChanged(double _range);
    void onMinimumChanged(double _min);
    void onColorChanged(QColor _color);
    void onPaddingChanged(int _padding);
    void onMaxObservationsChanged(int _max);
};
}  // namespace dqtx
