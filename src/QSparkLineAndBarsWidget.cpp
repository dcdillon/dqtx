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

#include <dqtx/QSparkLineAndBarsWidget.hpp>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <iostream>
#include <QPaintEvent>
#include <algorithm>
#include <limits>

namespace dqtx
{
QSparkLineAndBarsWidget::QSparkLineAndBarsWidget(QWidget *_parent,
                                                 Qt::WindowFlags _flags)
    : QWidget(_parent, _flags)
    , m_minLineRange(1)
    , m_lineMinimum(std::numeric_limits< double >::max())
    , m_minBarRange(1)
    , m_barMinimum(std::numeric_limits< double >::max())
    , m_lineColor(QColor(Qt::black))
    , m_barColor(QColor(Qt::black))
    , m_leftPadding(5)
    , m_rightPadding(5)
    , m_topPadding(5)
    , m_bottomPadding(1)
    , m_maxObservations(30)
    , m_barHeightRatio(.33)
{
    QObject::connect(this,
                     SIGNAL(observationInserted(double, double)),
                     this,
                     SLOT(onObservationInserted(double, double)));
    QObject::connect(this,
                     SIGNAL(minLineRangeChanged(double)),
                     this,
                     SLOT(onMinLineRangeChanged(double)));
    QObject::connect(this,
                     SIGNAL(lineMinimumChanged(double)),
                     this,
                     SLOT(onLineMinimumChanged(double)));
    QObject::connect(this,
                     SIGNAL(minBarRangeChanged(double)),
                     this,
                     SLOT(onMinBarRangeChanged(double)));
    QObject::connect(this,
                     SIGNAL(barMinimumChanged(double)),
                     this,
                     SLOT(onBarMinimumChanged(double)));
    QObject::connect(this,
                     SIGNAL(lineColorChanged(QColor)),
                     this,
                     SLOT(onLineColorChanged(QColor)));
    QObject::connect(this,
                     SIGNAL(barColorChanged(QColor)),
                     this,
                     SLOT(onBarColorChanged(QColor)));
    QObject::connect(this,
                     SIGNAL(leftPaddingChanged(int)),
                     this,
                     SLOT(onLeftPaddingChanged(int)));
    QObject::connect(this,
                     SIGNAL(rightPaddingChanged(int)),
                     this,
                     SLOT(onRightPaddingChanged(int)));
    QObject::connect(this,
                     SIGNAL(topPaddingChanged(int)),
                     this,
                     SLOT(onTopPaddingChanged(int)));
    QObject::connect(this,
                     SIGNAL(bottomPaddingChanged(int)),
                     this,
                     SLOT(onBottomPaddingChanged(int)));
    QObject::connect(this,
                     SIGNAL(paddingChanged(int, int, int, int)),
                     this,
                     SLOT(onPaddingChanged(int, int, int, int)));
    QObject::connect(this,
                     SIGNAL(maxObservationsChanged(int)),
                     this,
                     SLOT(onMaxObservationsChanged(int)));
    QObject::connect(this,
                     SIGNAL(barHeightRatioChanged(double)),
                     this,
                     SLOT(onBarHeightRatioChanged(double)));
}

void QSparkLineAndBarsWidget::insertObservation(const double _lineData,
                                                const double _barData)
{
    emit observationInserted(_lineData, _barData);
}

void QSparkLineAndBarsWidget::setMinLineRange(const double _range)
{
    emit minLineRangeChanged(_range);
}

void QSparkLineAndBarsWidget::setLineMinimum(const double _min)
{
    emit lineMinimumChanged(_min);
}

void QSparkLineAndBarsWidget::setMinBarRange(const double _range)
{
    emit minBarRangeChanged(_range);
}

void QSparkLineAndBarsWidget::setBarMinimum(const double _min)
{
    emit lineMinimumChanged(_min);
}

void QSparkLineAndBarsWidget::setLineColor(const QColor &_color)
{
    emit lineColorChanged(_color);
}

void QSparkLineAndBarsWidget::setBarColor(const QColor &_color)
{
    emit barColorChanged(_color);
}

void QSparkLineAndBarsWidget::setLeftPadding(const int _padding)
{
    emit leftPaddingChanged(_padding);
}

void QSparkLineAndBarsWidget::setRightPadding(const int _padding)
{
    emit rightPaddingChanged(_padding);
}

void QSparkLineAndBarsWidget::setTopPadding(const int _padding)
{
    emit topPaddingChanged(_padding);
}

void QSparkLineAndBarsWidget::setBottomPadding(const int _padding)
{
    emit bottomPaddingChanged(_padding);
}

void QSparkLineAndBarsWidget::setPadding(const int _left,
                                         const int _right,
                                         const int _top,
                                         const int _bottom)
{
    emit paddingChanged(_left, _right, _top, _bottom);
}

void QSparkLineAndBarsWidget::setBarHeightRatio(const double _ratio)
{
    emit barHeightRatioChanged(_ratio);
}

void QSparkLineAndBarsWidget::setMaxObservations(int _max)
{
    emit maxObservationsChanged(_max);
}

QSize QSparkLineAndBarsWidget::sizeHint() const
{
    return QSize(((m_maxObservations - 1) * 2) + m_leftPadding + m_rightPadding,
                 29);
}

void QSparkLineAndBarsWidget::paintEvent(QPaintEvent *_event)
{
    QPainter painter(this);

    const int graphHeight = _event->rect().height();
    const int graphWidth = _event->rect().width();

    const int barHeight = int(m_barHeightRatio * graphHeight);
    const int lineHeight = graphHeight - barHeight;
    const int barWidth = graphWidth;
    const int lineWidth = graphWidth;

    QRect barRect(_event->rect().topLeft().x(),
                  _event->rect().topLeft().y() + lineHeight,
                  barWidth,
                  barHeight);
    QRect lineRect(_event->rect().topLeft().x(),
                   _event->rect().topLeft().y(),
                   lineWidth,
                   lineHeight);

    drawBars(m_barData,
             painter,
             barRect,
             m_leftPadding,
             m_rightPadding,
             0,
             m_bottomPadding);
    drawLine(m_lineData,
             painter,
             lineRect,
             m_leftPadding,
             m_rightPadding,
             m_topPadding,
             0);
}

void QSparkLineAndBarsWidget::drawLine(const QList< double > &_data,
                                       QPainter &_painter,
                                       const QRect &_rect,
                                       int _leftPadding,
                                       int _rightPadding,
                                       int _topPadding,
                                       int _bottomPadding)
{
    int graphHeight = _rect.height() - _topPadding - _bottomPadding;
    int graphWidth = _rect.width() - _leftPadding - _rightPadding;
    QPoint bl = _rect.bottomLeft();

    QPainterPath path;

    if (_data.size() > 1)
    {
        double min = *std::min_element(_data.begin(), _data.end());
        min = std::min(min, m_lineMinimum);
        double max = *std::max_element(_data.begin(), _data.end());

        double range = max - min;

        if (range < m_minLineRange)
        {
            max += m_minLineRange - range;
        }

        double skip = 0;

        if (min != max)
        {
            skip = double(graphHeight - 1) / (max - min);
        }

        double width = graphWidth / double(_data.size() - 1);

        double x = _leftPadding;
        double y = -_bottomPadding;

        bool first = true;

        QList< double >::const_iterator i = _data.begin();
        QList< double >::const_iterator iend = _data.end();

        for (; i != iend; ++i)
        {
            double height = (*i - min) * skip;

            if (first)
            {
                y -= height;
                first = false;

                path.moveTo(bl.x() + int(x), bl.y() + int(y));
            }
            else
            {
                x += width;
                y = -_bottomPadding - height;

                path.lineTo(bl.x() + int(x), bl.y() + int(y));
            }
        }
    }

    if (!path.isEmpty())
    {
        _painter.setRenderHint(QPainter::Antialiasing, true);
        _painter.setPen(
            QPen(m_lineColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _painter.drawPath(path);
    }
}

void QSparkLineAndBarsWidget::drawBars(const QList< double > &_data,
                                       QPainter &_painter,
                                       const QRect &_rect,
                                       int _leftPadding,
                                       int _rightPadding,
                                       int _topPadding,
                                       int _bottomPadding)
{
    _painter.setRenderHint(QPainter::Antialiasing, false);
    _painter.setPen(
        QPen(m_barColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    int graphHeight = _rect.height() - _topPadding - _bottomPadding;
    int graphWidth = _rect.width() - _leftPadding - _rightPadding;
    QPoint bl = _rect.bottomLeft();

    if (!_data.empty())
    {
        double min = *std::min_element(_data.begin(), _data.end());
        min = std::min(min, m_barMinimum);
        double max = *std::max_element(_data.begin(), _data.end());

        double range = max - min;

        if (range < m_minBarRange)
        {
            max += m_minBarRange - range;
        }

        double skip = 0;

        if (min != max)
        {
            skip = double(graphHeight - 1) / (max - min);
        }

        double width = graphWidth / double(_data.size() - 1);

        double x = _leftPadding;
        double y = -_bottomPadding;

        QList< double >::const_iterator i = _data.begin();
        QList< double >::const_iterator iend = _data.end();

        for (; i != iend; ++i)
        {
            double height = (*i - min) * skip;
            y = -_bottomPadding - height;

            _painter.drawLine(
                bl.x() + x, bl.y() - _bottomPadding, bl.x() + x, bl.y() + y);

            x += width;
        }
    }
}

void QSparkLineAndBarsWidget::onObservationInserted(double _line, double _bar)
{
    if (m_maxObservations > 2)
    {
        while (m_lineData.size() >= m_maxObservations)
        {
            m_lineData.pop_front();
            m_barData.pop_front();
        }
    }

    m_lineData.push_back(_line);
    m_barData.push_back(_bar);

    double min = *std::min_element(m_lineData.begin(), m_lineData.end());
    double max = *std::max_element(m_lineData.begin(), m_lineData.end());
    double change = m_lineData.back() - m_lineData.front();

    setToolTip(QString("Min: %1\nMax: %2\nChange: %3").arg(
        QString::number(min), QString::number(max), QString::number(change)));

    update();
}

void QSparkLineAndBarsWidget::onMinLineRangeChanged(double _range)
{
    m_minLineRange = _range;
    update();
}

void QSparkLineAndBarsWidget::onLineMinimumChanged(double _min)
{
    m_lineMinimum = _min;
    update();
}

void QSparkLineAndBarsWidget::onMinBarRangeChanged(double _range)
{
    m_minBarRange = _range;
    update();
}

void QSparkLineAndBarsWidget::onBarMinimumChanged(double _min)
{
    m_barMinimum = _min;
    update();
}

void QSparkLineAndBarsWidget::onLineColorChanged(QColor _color)
{
    m_lineColor = _color;
    update();
}

void QSparkLineAndBarsWidget::onBarColorChanged(QColor _color)
{
    m_barColor = _color;
    update();
}

void QSparkLineAndBarsWidget::onLeftPaddingChanged(int _padding)
{
    m_leftPadding = _padding;
    update();
}

void QSparkLineAndBarsWidget::onRightPaddingChanged(int _padding)
{
    m_rightPadding = _padding;
    update();
}

void QSparkLineAndBarsWidget::onTopPaddingChanged(int _padding)
{
    m_topPadding = _padding;
    update();
}

void QSparkLineAndBarsWidget::onBottomPaddingChanged(int _padding)
{
    m_bottomPadding = _padding;
    update();
}

void QSparkLineAndBarsWidget::onPaddingChanged(int _left,
                                               int _right,
                                               int _top,
                                               int _bottom)
{
    m_leftPadding = _left;
    m_rightPadding = _right;
    m_topPadding = _top;
    m_bottomPadding = _bottom;

    update();
}

void QSparkLineAndBarsWidget::onMaxObservationsChanged(int _max)
{
    m_maxObservations = _max;

    if (m_maxObservations > 1)
    {
        while (int(m_lineData.size()) > m_maxObservations)
        {
            m_lineData.pop_front();
            m_barData.pop_front();
        }
    }
}

void QSparkLineAndBarsWidget::onBarHeightRatioChanged(double _ratio)
{
    m_barHeightRatio = _ratio;
    update();
}

}  // namespace dqtx
