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

#include <dqtx/QHistogramWidget.hpp>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <iostream>
#include <QPaintEvent>

namespace dqtx
{
QHistogramWidget::QHistogramWidget(QWidget *_parent, Qt::WindowFlags _flags)
    : QWidget(_parent, _flags)
    , m_minRange(1)
    , m_color(QColor(Qt::black))
    , m_padding(5)
    , m_maxObservations(30)
{
    QObject::connect(this, SIGNAL(observationInserted()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(rangeChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(colorChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(paddingChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(maxObservationsChanged()), this, SLOT(update()));
}

QList< double > QHistogramWidget::getData() const
{
    m_lock.lock();
    QList< double > retval = m_data;
    m_lock.unlock();
    
    return retval;
}

void QHistogramWidget::insertObservation(const double _data)
{
    m_lock.lock();
    
    if (m_maxObservations > 2)
    {
        while (m_data.size() >= m_maxObservations)
        {
            m_data.pop_front();
        }
    }
    
    m_data.push_back(_data);
    
    m_lock.unlock();
    
    emit observationInserted();
}

double QHistogramWidget::getMinRange() const
{
    m_lock.lock();
    double retval = m_minRange;
    m_lock.unlock();
    
    return retval;
}

void QHistogramWidget::setMinRange(const double _range)
{
    m_lock.lock();
    m_minRange = _range;
    m_lock.unlock();
    
    emit rangeChanged();
}

QColor QHistogramWidget::getColor() const
{
    m_lock.lock();
    QColor retval = m_color;
    m_lock.unlock();
    
    return retval;
}

void QHistogramWidget::setColor(const QColor &_color)
{
    m_lock.lock();
    m_color = _color;
    m_lock.unlock();
    
    emit colorChanged();
}

int QHistogramWidget::getPadding() const
{
    m_lock.lock();
    int retval = m_padding;
    m_lock.unlock();
    
    return retval;
}

void QHistogramWidget::setPadding(int _padding)
{
    m_lock.lock();
    m_padding = _padding;
    m_lock.unlock();
    
    emit paddingChanged();
}

int QHistogramWidget::getMaxObservations() const
{
    m_lock.lock();
    int retval = m_maxObservations;
    m_lock.unlock();
    
    return retval;
}

void QHistogramWidget::setMaxObservations(int _max)
{
    m_lock.lock();
    m_maxObservations = _max;
    
    if (m_maxObservations > 1)
    {
        while (int(m_data.size()) > m_maxObservations)
        {
            m_data.pop_front();
        }
    }
    
    m_lock.unlock();
    
    emit maxObservationsChanged();
}

QSize QHistogramWidget::sizeHint() const
{
    return QSize((m_maxObservations + m_padding) * 2, 29);
}

void QHistogramWidget::paintEvent(QPaintEvent *_event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(m_color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPainterPath path;
    
    m_lock.lock();

    double min = *std::min_element(m_data.begin(), m_data.end());
    double max = *std::max_element(m_data.begin(), m_data.end());
    
    double range = max - min;
    
    if (range < m_minRange)
    {
        min -= (m_minRange - range) / 2;
        max += (m_minRange - range) / 2;
    }
    
    int graphHeight = _event->rect().height() - (2 * m_padding);
    int graphWidth = _event->rect().width() - (2 * m_padding);
    
    double skip = 0;
    
    if (min != max)
    {
        skip = double(graphHeight - 1) / (max - min);
    }
    
    double width = graphWidth / double(m_data.size() - 1);
    
    double x = m_padding;
    double y = -m_padding;
    
    QPoint bl = _event->rect().bottomLeft();
    
    QList< double >::iterator i = m_data.begin();
    QList< double >::iterator iend = m_data.end();
    
    for ( ; i != iend; ++i)
    {
        double height = (*i - min) * skip;
        y = -m_padding - height;
        
        painter.drawLine(bl.x() + x, bl.y() - m_padding,
            bl.x() + x, bl.y() + y);
        
        x += width;
    }

    m_lock.unlock();
}
} // namespace dqtx
