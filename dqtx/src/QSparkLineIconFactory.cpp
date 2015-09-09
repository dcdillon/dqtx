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

#include <dqtx/QSparkLineIconFactory.hpp>
#include <QPainter>
#include <QPen>
#include <QPixmap>

namespace dqtx
{
QIcon QSparkLineIconFactory::create(const QList< double > &_observations,
                               const double _minimum,
                               const double _minRange,
                               const QColor &_color,
                               const QColor &_bgColor,
                               const int _width,
                               const int _height,
                               const int _leftPadding,
                               const int _rightPadding,
                               const int _topPadding,
                               const int _bottomPadding)
{
    QPixmap pixmap(_width, _height);
    pixmap.fill(_bgColor);
    QPainter painter(&pixmap);

    int graphHeight = pixmap.rect().height() - _topPadding - _bottomPadding;
    int graphWidth = pixmap.rect().width() - _leftPadding - _rightPadding;
    QPoint bl = pixmap.rect().bottomLeft();
    
    QPainterPath path;
    
    if (_observations.size() > 1)
    {
        double min = *std::min_element(_observations.begin(), _observations.end());
        min = std::min(min, _minimum);
        double max = *std::max_element(_observations.begin(), _observations.end());
        
        double range = max - min;
        
        if (range < _minRange)
        {
            max += _minRange - range;
        }
        
        double skip = 0;
        
        if (min != max)
        {
            skip = double(graphHeight - 1) / (max - min);
            
            double width = graphWidth / double(_observations.size() - 1);
            
            double x = _leftPadding;
            double y = -_bottomPadding;
            
            bool first = true;
            
            QList< double >::const_iterator i = _observations.begin();
            QList< double >::const_iterator iend = _observations.end();
            
            for ( ; i != iend; ++i)
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
    }

    if (!path.isEmpty())
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
    }
    
    return QIcon(pixmap);
}

QIcon QSparkLineIconFactory::create(const QList< QPair< double, time_t > > &_observations,
                        const QColor &_color,
                        QList< QPair< time_t, QColor > > _divisions,
                        const QColor &_bgColor,
                               const double _minimum,
                               const double _minRange,
                               const int _width,
                               const int _height,
                               const int _leftPadding,
                               const int _rightPadding,
                               const int _topPadding,
                               const int _bottomPadding)
{
    QPixmap pixmap(_width, _height);
    pixmap.fill(_bgColor);
    QPainter painter(&pixmap);

    int graphHeight = pixmap.rect().height() - _topPadding - _bottomPadding;
    int graphWidth = pixmap.rect().width() - _leftPadding - _rightPadding;
    QPoint bl = pixmap.rect().bottomLeft();
    
    _divisions.push_front(QPair< time_t, QColor >(0, _color));
    QList< QPair< time_t, QColor > >::iterator color = _divisions.begin();
    
    QPainterPath path;
    
    if (_observations.size() > 1)
    {
        double min = std::min_element(_observations.begin(), _observations.end())->first;
        min = std::min(min, _minimum);
        double max = std::max_element(_observations.begin(), _observations.end())->first;
        
        double range = max - min;
        
        if (range < _minRange)
        {
            max += _minRange - range;
        }
        
        double skip = 0;
        
        if (min != max)
        {
            skip = double(graphHeight - 1) / (max - min);
            
            double width = graphWidth / double(_observations.size() - 1);
            
            double x = _leftPadding;
            double y = -_bottomPadding;
            
            bool first = true;
            
            QList< QPair< double, time_t > >::const_iterator i = _observations.begin();
            QList< QPair< double, time_t > >::const_iterator iend = _observations.end();
            
            for ( ; i != iend; ++i)
            {
                QList< QPair< time_t, QColor > >::iterator j = color;
                QList< QPair< time_t, QColor > >::iterator jend = _divisions.end();
                QList< QPair< time_t, QColor > >::iterator newColor = color;
                
                for ( ; j != jend; ++j)
                {
                    if (i->second >= j->first)
                    {   
                        newColor = j;
                    }
                }
                
                if (newColor != color)
                {
                    if (!path.isEmpty())
                    {
                        painter.setRenderHint(QPainter::Antialiasing, true);
                        painter.setPen(QPen(color->second, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                        painter.drawPath(path);
                        QPainterPath newPath;
                        newPath.moveTo(path.currentPosition());
                        path = newPath;
                    }
                    
                    color = newColor;
                }
                
                double height = (i->first - min) * skip;
                
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
    }

    if (!path.isEmpty())
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(color->second, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
    }
    
    return QIcon(pixmap);
}

}  // namespace dqtx
