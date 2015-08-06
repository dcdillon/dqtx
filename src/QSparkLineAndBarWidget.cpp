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

#include <dqtx/QSparkLineAndBarWidget.hpp>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <iostream>
#include <QPaintEvent>
#include <algorithm>

namespace dqtx
{
QSparkLineAndBarWidget::QSparkLineAndBarWidget(QWidget *_parent, Qt::WindowFlags _flags)
    : QWidget(_parent, _flags)
    , m_minLineRange(1)
    , m_minBarRange(1)
    , m_lineColor(QColor(Qt::black))
    , m_barColor(QColor(Qt::black))
    , m_leftPadding(5)
    , m_rightPadding(5)
    , m_topPadding(5)
    , m_bottomPadding(1)
    , m_maxObservations(30)
    , m_barHeightRatio(.33)
{
    QObject::connect(this, SIGNAL(observationInserted()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(rangeChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(colorChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(paddingChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(maxObservationsChanged()), this, SLOT(update()));
    QObject::connect(this, SIGNAL(barHeightRatioChanged()), this, SLOT(update()));
}

void QSparkLineAndBarWidget::insertObservation(const double _lineData, const double _barData)
{
    m_lock.lock();
    
    if (m_maxObservations > 2)
    {
        while (m_lineData.size() >= m_maxObservations)
        {
            m_lineData.pop_front();
            m_barData.pop_front();
        }
    }
    
    m_lineData.push_back(_lineData);
    m_barData.push_back(_barData);
    
    double min = *std::min_element(m_lineData.begin(), m_lineData.end());
    double max = *std::max_element(m_lineData.begin(), m_lineData.end());
    double change = m_lineData.back() - m_lineData.front();
    
    setToolTip(QString("Min: %1\nMax: %2\nChange: %3").arg(min, max, change));
    
    m_lock.unlock();
    
    emit observationInserted();
}

void QSparkLineAndBarWidget::setMinLineRange(const double _range)
{
    m_lock.lock();
    m_minLineRange = _range;
    m_lock.unlock();
    
    emit rangeChanged();
}

void QSparkLineAndBarWidget::setMinBarRange(const double _range)
{
    m_lock.lock();
    m_minBarRange = _range;
    m_lock.unlock();
    
    emit rangeChanged();
}

void QSparkLineAndBarWidget::setLineColor(const QColor &_color)
{
    m_lock.lock();
    m_lineColor = _color;
    m_lock.unlock();
    
    emit colorChanged();
}

void QSparkLineAndBarWidget::setBarColor(const QColor &_color)
{
    m_lock.lock();
    m_barColor = _color;
    m_lock.unlock();
    
    emit colorChanged();
}

void QSparkLineAndBarWidget::setLeftPadding(const int _padding)
{
    m_lock.lock();
    m_leftPadding = _padding;
    m_lock.unlock();
    
    emit paddingChanged();
}

void QSparkLineAndBarWidget::setRightPadding(const int _padding)
{
    m_lock.lock();
    m_rightPadding = _padding;
    m_lock.unlock();
    
    emit paddingChanged();
}

void QSparkLineAndBarWidget::setTopPadding(const int _padding)
{
    m_lock.lock();
    m_topPadding = _padding;
    m_lock.unlock();
    
    emit paddingChanged();
}

void QSparkLineAndBarWidget::setBottomPadding(const int _padding)
{
    m_lock.lock();
    m_bottomPadding = _padding;
    m_lock.unlock();
    
    emit paddingChanged();
}

void QSparkLineAndBarWidget::setPadding(const int _left, const int _right, const int _top, const int _bottom)
{
    m_lock.lock();
    m_leftPadding = _left;
    m_rightPadding = _right;
    m_topPadding = _top;
    m_bottomPadding = _bottom;
    m_lock.unlock();
    
    emit paddingChanged();
}

void QSparkLineAndBarWidget::setBarHeightRatio(const double _ratio)
{
    m_lock.lock();
    m_barHeightRatio = _ratio;
    m_lock.unlock();
    
    emit barHeightRatioChanged();
}

void QSparkLineAndBarWidget::setMaxObservations(int _max)
{
    m_lock.lock();
    m_maxObservations = _max;
    
    if (m_maxObservations > 1)
    {
        while (int(m_lineData.size()) > m_maxObservations)
        {
            m_lineData.pop_front();
            m_barData.pop_front();
        }
    }
    
    m_lock.unlock();
    
    emit maxObservationsChanged();
}

QSize QSparkLineAndBarWidget::sizeHint() const
{
    return QSize(((m_maxObservations - 1) * 2) + m_leftPadding + m_rightPadding, 29);
}

void QSparkLineAndBarWidget::paintEvent(QPaintEvent *_event)
{
    QPainter painter(this);
    QPainterPath path;
    QList< QPair< QPoint, QPoint > > lines;
    
    m_lock.lock();
    
    int graphHeight = _event->rect().height() - m_topPadding - m_bottomPadding;
    int graphWidth = _event->rect().width() - m_leftPadding - m_rightPadding;
    
    int barHeight = int(m_barHeightRatio * graphHeight);
    int lineHeight = graphHeight - barHeight;
    int barWidth = graphWidth;
    int lineWidth = graphWidth;
    
    QPoint barBL = _event->rect().bottomLeft();
    QPoint lineBL(barBL.x(), barBL.y() - barHeight);

    if (m_lineData.size() > 1)
    {
        double min = *std::min_element(m_lineData.begin(), m_lineData.end());
        double max = *std::max_element(m_lineData.begin(), m_lineData.end());
        
        double range = max - min;
        
        if (range < m_minLineRange)
        {
            min -= (m_minLineRange - range) / 2;
            max += (m_minLineRange - range) / 2;
        }
        
        
        
        double skip = 0;
        
        if (min != max)
        {
            skip = double(lineHeight - 1) / (max - min);
        }
        
        double width = lineWidth / double(m_lineData.size() - 1);
        
        double x = m_leftPadding;
        double y = -m_bottomPadding;
        
        bool first = true;
        
        QList< double >::iterator i = m_lineData.begin();
        QList< double >::iterator iend = m_lineData.end();
        
        for ( ; i != iend; ++i)
        {
            double height = (*i - min) * skip;
            
            if (first)
            {
                y -= height;
                first = false;
                path.moveTo(lineBL.x() + int(x), lineBL.y() + int(y));
            }
            else
            {
                x += width;
                y = -m_bottomPadding - height;
                
                
                path.lineTo(lineBL.x() + int(x), lineBL.y() + int(y));
            }
        }
    }
    
    
    // calculate bars
    if (!m_barData.empty())
    {
        double min = *std::min_element(m_barData.begin(), m_barData.end());
        double max = *std::max_element(m_barData.begin(), m_barData.end());
        
        double range = max - min;
        
        if (range < m_minBarRange)
        {
            max += (m_minBarRange - range);
        }
        
        double skip = 0;
        
        if (min != max)
        {
            skip = double(barHeight - 1) / (max - min);
        }
        
        double width = barWidth / double(m_barData.size() - 1);
        
        double x = m_leftPadding;
        double y = -m_bottomPadding;
        
        QList< double >::iterator i = m_barData.begin();
        QList< double >::iterator iend = m_barData.end();
        
        for ( ; i != iend; ++i)
        {
            double height = (*i - min) * skip;
            y = -m_bottomPadding - height;
            
            lines.push_back(QPair< QPoint, QPoint >(QPoint(barBL.x() + x, barBL.y() - m_bottomPadding),
                QPoint(barBL.x() + x, barBL.y() + y)));
            
            x += width;
        }
    }

    m_lock.unlock();
    
    if (!path.isEmpty())
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(m_lineColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
    }
    
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(m_barColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    
    QList< QPair< QPoint, QPoint > >::iterator i = lines.begin();
    QList< QPair< QPoint, QPoint > >::iterator iend = lines.end();
    
    for ( ; i != iend; ++i)
    {
        painter.drawLine(i->first, i->second);
    }
}
} // namespace dqtx
