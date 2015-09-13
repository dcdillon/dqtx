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

#include <QIcon>
#include <QList>
#include <limits>
#include <ctime>

namespace dqtx
{
class QSparkLineIconFactory
{
   public:
    static QIcon create(
        const QList< double > &_observations,
        const double _minimum = std::numeric_limits< double >::max(),
        const double _minRange = 0,
        const QColor &_color = QColor(Qt::black),
        const QColor &_bgColor = QColor(Qt::transparent),
        const int _width = 48,
        const int _height = 48,
        const int _leftPadding = 3,
        const int _rightPadding = 3,
        const int _topPadding = 3,
        const int _bottomPadding = 3);

    static QIcon create(
        const QList< QPair< double, time_t > > &_observations,
        const QColor &_color,
        QList< QPair< time_t, QColor > > _divisions,
        const QColor &_bgColor = QColor(Qt::transparent),
        const double _minimum = std::numeric_limits< double >::max(),
        const double _minRange = 0,
        const int _width = 48,
        const int _height = 48,
        const int _leftPadding = 3,
        const int _rightPadding = 3,
        const int _topPadding = 3,
        const int _bottomPadding = 3);
};
}  // namespace dqtx
