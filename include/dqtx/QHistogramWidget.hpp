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
class QHistogramWidget : public QWidget
{
    Q_OBJECT
public:
    QHistogramWidget(QWidget *_parent = 0, Qt::WindowFlags _flags = 0);
    
public:
    QList< double > getData() const;
    void insertObservation(const double _data);
    
    double getMinRange() const;
    void setMinRange(const double _range);
    
    QColor getColor() const;
    void setColor(const QColor &_color);
    
    int getPadding() const;
    void setPadding(const int _padding);
    
    int getMaxObservations() const;
    void setMaxObservations(const int _max);
    
public:
    QSize sizeHint() const;
    
protected:
    void paintEvent(QPaintEvent *_event);
    
private:
    mutable QMutex m_lock;
    QList< double > m_data;
    double m_minRange;
    QColor m_color;
    int m_padding;
    int m_maxObservations;
    
signals:
    void observationInserted();
    void rangeChanged();
    void colorChanged();
    void paddingChanged();
    void maxObservationsChanged();
};
} // namespace dqtx
