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

#include <dqtx/QTextIconFactory.hpp>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QFont>

namespace dqtx
{
QIcon QTextIconFactory::create(const QString &_text,
                               const QColor &_textColor,
                               const QColor &_bgColor,
                               int _width,
                               int _height,
                               int _fontSizeHint)
{
    QPixmap pixmap(_width, _height);
    pixmap.fill(_bgColor);
    QPainter painter(&pixmap);
    painter.setPen(QPen(_textColor));

    QFont font = painter.font();
    font.setPointSize(_fontSizeHint);
    painter.setFont(font);

    while (painter.fontMetrics().width(_text) > _width - 2 ||
           painter.fontMetrics().xHeight() > _height - 2)
    {
        font.setPointSize(font.pointSize() - 1);
        painter.setFont(font);
    }

    painter.drawText(pixmap.rect(), Qt::AlignCenter, _text);
    return QIcon(pixmap);
}

}  // namespace dqtx
