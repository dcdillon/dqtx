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
    , m_accumulator(boost::accumulators::tag::density::num_bins = 30, boost::accumulators::tag::density::cache_size = 10)
    , m_displayType(DisplayTypeLine)
    , m_color(QColor(Qt::black))
    , m_padding(5)
{
    QObject::connect(this,
                     SIGNAL(observationInserted(double)),
                     this,
                     SLOT(onObservationInserted(double)));
    QObject::connect(this,
                     SIGNAL(displayTypeChanged(int)),
                     this,
                     SLOT(onDisplayTypeChanged(int)));
    QObject::connect(
        this, SIGNAL(colorChanged(QColor)), this, SLOT(onColorChanged(QColor)));
    QObject::connect(
        this, SIGNAL(paddingChanged(int)), this, SLOT(onPaddingChanged(int)));
}

void QDensityWidget::insertObservation(const double _data)
{
    emit observationInserted(_data);
}

void QDensityWidget::setDisplayType(const DisplayType _type)
{
    emit displayTypeChanged(int(_type));
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

    switch (m_displayType)
    {
        case DisplayTypeBars:
            drawBars(painter,
                     _event->rect(),
                     m_padding,
                     m_padding,
                     m_padding,
                     m_padding);
            break;
        case DisplayTypeLine:
        default:
            drawLine(painter,
                     _event->rect(),
                     m_padding,
                     m_padding,
                     m_padding,
                     m_padding);
            break;
    }
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

    boost::iterator_range< std::vector< std::pair<double, double> >::iterator >
        histogram = boost::accumulators::density(m_accumulator);
        
    QList< QPair< double, double > > points;
    double minX = std::numeric_limits< double >::max();
    double maxX = std::numeric_limits< double >::min();
    double minY = std::numeric_limits< double >::max();
    double maxY = std::numeric_limits< double >::min();
    
    for (uint32_t i = 0; i < histogram.size(); ++i)
    {
        const double x = histogram[i].first;
        const double y = histogram[i].second;
        
        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
        
        points.push_back(QPair< double, double >(x, y));
    }
    
    const double xStep = double(graphWidth) / points.size();
    const double yStep = double(graphHeight) / (maxY - minY);
    
    bool first = true;
    QList< QPair< double, double > >::iterator i = points.begin();
    QList< QPair< double, double > >::iterator iend = points.end();
    
    double x = 0;
    double y = -_bottomPadding;
    for ( ; i != iend; ++i)
    {
        y = -_bottomPadding + (yStep * i->second);
        
        if (first)
        {
            path.moveTo(int(bl.x() + _leftPadding + x), int(bl.y() - y));
            first = false;
        }
        else
        {
            path.lineTo(int(bl.x() + _leftPadding + x), int(bl.y() - y));
        }
        
        x += xStep;
    }
    
    if (!path.isEmpty())
    {
        _painter.setRenderHint(QPainter::Antialiasing, true);
        _painter.setPen(
            QPen(m_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _painter.drawPath(path);
    }
    
}

void QDensityWidget::drawBars(QPainter &_painter,
                                const QRect &_rect,
                                int _leftPadding,
                                int _rightPadding,
                                int _topPadding,
                                int _bottomPadding)
{
}

void QDensityWidget::onObservationInserted(double _data)
{
    m_accumulator(_data);
    update();
}

void QDensityWidget::onDisplayTypeChanged(int _type)
{
    m_displayType = static_cast< DisplayType >(_type);
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
