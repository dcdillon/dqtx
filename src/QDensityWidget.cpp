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

#include <dqtx/QDensityWidget.hpp>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <iostream>
#include <QPaintEvent>
#include <algorithm>
#include <limits>
#include <stdint.h>

namespace dqtx
{
QDensityWidget::QDensityWidget(QWidget *_parent, Qt::WindowFlags _flags)
    : QWidget(_parent, _flags)
    , m_accumulator(boost::accumulators::tag::tail<boost::accumulators::right>::cache_size = 60)
    , m_color(QColor(Qt::black))
    , m_padding(5)
    , m_bandwidth(.5)
{
    QObject::connect(this,
                     SIGNAL(observationInserted(double)),
                     this,
                     SLOT(onObservationInserted(double)));
    QObject::connect(
        this, SIGNAL(colorChanged(QColor)), this, SLOT(onColorChanged(QColor)));
    QObject::connect(
        this, SIGNAL(paddingChanged(int)), this, SLOT(onPaddingChanged(int)));
}

void QDensityWidget::insertObservation(const double _data)
{
    emit observationInserted(_data);
}

void QDensityWidget::setColor(const QColor &_color)
{
    emit colorChanged(_color);
}

void QDensityWidget::setPadding(int _padding)
{
    emit paddingChanged(_padding);
}

QSize QDensityWidget::sizeHint() const
{
    return QSize(68, 29);
}

void QDensityWidget::paintEvent(QPaintEvent *_event)
{
    QPainter painter(this);

    drawLine(painter,
             _event->rect(),
             m_padding,
             m_padding,
             m_padding,
             m_padding);
}

double QDensityWidget::density(double _x)
{
    double sum = 0;
    
    QList< double >::iterator i = m_data.begin();
    QList< double >::iterator iend = m_data.end();
    
    for ( ; i != iend; ++i)
    {
        sum += normal((_x - *i) / m_bandwidth);
    }
    
    sum *= 1 / (m_data.size() * m_bandwidth);
    
    return sum;
}

double QDensityWidget::normal(double _x)
{
    const double variance = boost::accumulators::variance(m_accumulator);
    const double sigma = sqrt(variance);
    const double mean = boost::accumulators::mean(m_accumulator);
    
    return (1 / sigma) * phi((_x - mean) / sigma);
}

double QDensityWidget::phi(double _x)
{
    return exp(-3.1415926535898 * _x * _x);
}

void QDensityWidget::drawLine(QPainter &_painter,
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
    
    double minX = *std::min_element(m_data.begin(), m_data.end());
    double maxX = *std::max_element(m_data.begin(), m_data.end());
    double minY = std::numeric_limits< double >::max();
    double maxY = std::numeric_limits< double >::min();
    
    const double xStep = (maxX - minX) /double(graphWidth - 1);
    
    QList< QPair< double, double > > points;
    for (double x = minX; x < maxX; x += xStep)
    {
        double y = density(x);
        points.push_back(QPair< double, double >(x, y));
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
    }

    const double yStep = double(graphHeight - 1) / (maxY);
    
    bool first = true;
    QList< QPair< double, double > >::iterator i = points.begin();
    QList< QPair< double, double > >::iterator iend = points.end();
    
    int x = _leftPadding;
    double y = -_bottomPadding;
    for ( ; i != iend; ++i)
    {
        y = -_bottomPadding - (yStep * i->second);
        //x = _leftPadding + (xStep * i->first);
        
        if (first)
        {
            path.moveTo(int(bl.x() + x), int(bl.y() + y));
            first = false;
        }
        else
        {
            path.lineTo(int(bl.x() + x), int(bl.y() + y));
        }
        
        ++x;
    }
    
    if (!path.isEmpty())
    {
        _painter.setRenderHint(QPainter::Antialiasing, true);
        _painter.setPen(
            QPen(m_color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _painter.drawPath(path);
    }
    
}

void QDensityWidget::onObservationInserted(double _data)
{
    m_data.push_back(_data);
    m_accumulator(_data);
    
    const double variance = boost::accumulators::variance(m_accumulator);
    const double sigma = sqrt(variance);
    const double interQuartile = boost::accumulators::quantile(m_accumulator, boost::accumulators::quantile_probability = .75)
        - boost::accumulators::quantile(m_accumulator, boost::accumulators::quantile_probability = .25);
    const double min = *std::min_element(m_data.begin(), m_data.end());
    const double max = *std::max_element(m_data.begin(), m_data.end());
    const double mean = boost::accumulators::mean(m_accumulator);
    
    m_bandwidth = pow(4.0 / (3 * m_data.size()), .2) * (std::min(sigma, interQuartile) / fabs(mean));
    
    setToolTip(QString("%1: %2\n%3: %4\nMin: %5\nMax: %6\nbw: %7").arg(QString(QChar(0x03BC)),
        QString::number(mean), QString(QChar(0x03C3)),
        QString::number(sigma), QString::number(min), QString::number(max), QString::number(m_bandwidth)));
    update();
}

void QDensityWidget::onColorChanged(QColor _color)
{
    m_color = _color;
    update();
}

void QDensityWidget::onPaddingChanged(int _padding)
{
    m_padding = _padding;
    update();
}

}  // namespace dqtx
