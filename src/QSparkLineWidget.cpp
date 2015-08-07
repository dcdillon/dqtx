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

#include <dqtx/QSparkLineWidget.hpp>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <iostream>
#include <QPaintEvent>
#include <algorithm>

namespace dqtx
{
QSparkLineWidget::QSparkLineWidget(QWidget *_parent, Qt::WindowFlags _flags)
    : QWidget(_parent, _flags)
    , m_displayType(DisplayTypeLine)
    , m_minRange(1)
    , m_color(QColor(Qt::black))
    , m_padding(5)
    , m_maxObservations(30)
{
    QObject::connect(this, SIGNAL(observationInserted(double)), this,
                     SLOT(onObservationInserted(double)));
    QObject::connect(this, SIGNAL(displayTypeChanged(int)), this,
                     SLOT(onDisplayTypeChanged(int)));
    QObject::connect(this, SIGNAL(minRangeChanged(double)), this,
                     SLOT(onMinRangeChanged(double)));
    QObject::connect(this, SIGNAL(colorChanged(QColor)), this,
                     SLOT(onColorChanged(QColor)));
    QObject::connect(this, SIGNAL(paddingChanged(int)), this,
                     SLOT(onPaddingChanged(int)));
    QObject::connect(this, SIGNAL(maxObservationsChanged(int)), this,
                     SLOT(onMaxObservationsChanged(int)));
}

void QSparkLineWidget::insertObservation(const double _data)
{
    emit observationInserted(_data);
}

void QSparkLineWidget::setDisplayType(const DisplayType _type)
{
    emit displayTypeChanged(int(_type));
}

void QSparkLineWidget::setMinRange(const double _range)
{
    emit minRangeChanged(_range);
}

void QSparkLineWidget::setColor(const QColor &_color)
{
    emit colorChanged(_color);
}

void QSparkLineWidget::setPadding(int _padding)
{
    emit paddingChanged(_padding);
}

void QSparkLineWidget::setMaxObservations(int _max)
{
    emit maxObservationsChanged(_max);
}

QSize QSparkLineWidget::sizeHint() const
{
    return QSize((m_maxObservations - 1 + m_padding) * 2, 29);
}

void QSparkLineWidget::paintEvent(QPaintEvent *_event)
{
    QPainter painter(this);

    switch (m_displayType)
    {
        case DisplayTypeBars:
            drawBars(m_data, painter, _event->rect(), m_padding, m_padding,
                     m_padding, m_padding);
            break;
        case DisplayTypeLine:
        default:
            drawLine(m_data, painter, _event->rect(), m_padding, m_padding,
                     m_padding, m_padding);
            break;
    }
}

void QSparkLineWidget::drawLine(const QList< double > &_data,
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
        double max = *std::max_element(_data.begin(), _data.end());

        double range = max - min;

        if (range < m_minRange)
        {
            min -= (m_minRange - range) / 2;
            max += (m_minRange - range) / 2;
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
            QPen(m_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _painter.drawPath(path);
    }
}

void QSparkLineWidget::drawBars(const QList< double > &_data,
                                QPainter &_painter,
                                const QRect &_rect,
                                int _leftPadding,
                                int _rightPadding,
                                int _topPadding,
                                int _bottomPadding)
{
    _painter.setRenderHint(QPainter::Antialiasing, false);
    _painter.setPen(
        QPen(m_color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    int graphHeight = _rect.height() - _topPadding - _bottomPadding;
    int graphWidth = _rect.width() - _leftPadding - _rightPadding;
    QPoint bl = _rect.bottomLeft();

    if (!_data.empty())
    {
        double min = *std::min_element(_data.begin(), _data.end());
        double max = *std::max_element(_data.begin(), _data.end());

        double range = max - min;

        if (range < m_minRange)
        {
            max += m_minRange - range;
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

            _painter.drawLine(bl.x() + x, bl.y() - _bottomPadding, bl.x() + x,
                              bl.y() + y);

            x += width;
        }
    }
}

void QSparkLineWidget::onObservationInserted(double _data)
{
    if (m_maxObservations > 2)
    {
        while (m_data.size() >= m_maxObservations)
        {
            m_data.pop_front();
        }
    }

    m_data.push_back(_data);

    double min = *std::min_element(m_data.begin(), m_data.end());
    double max = *std::max_element(m_data.begin(), m_data.end());
    double change = m_data.back() - m_data.front();

    setToolTip(QString("Min: %1\nMax: %2\nChange: %3").arg(min, max, change));
    update();
}

void QSparkLineWidget::onDisplayTypeChanged(int _type)
{
    m_displayType = static_cast< DisplayType >(_type);
    update();
}

void QSparkLineWidget::onMinRangeChanged(double _range)
{
    m_minRange = _range;
    update();
}

void QSparkLineWidget::onColorChanged(QColor _color)
{
    m_color = _color;
    update();
}

void QSparkLineWidget::onPaddingChanged(int _padding)
{
    m_padding = _padding;
    update();
}

void QSparkLineWidget::onMaxObservationsChanged(int _max)
{
    m_maxObservations = _max;

    if (m_maxObservations > 2)
    {
        while (m_data.size() > m_maxObservations)
        {
            m_data.pop_front();
        }
    }

    update();
}

}  // namespace dqtx
