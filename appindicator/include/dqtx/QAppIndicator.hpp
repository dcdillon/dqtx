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
#undef signals
#include <libappindicator/app-indicator.h>
#define signals public

#include <QObject>
#include "QAppIndicatorMenu.hpp"

namespace dqtx
{
class QAppIndicator : public QObject
{
    Q_OBJECT

   public:
    QAppIndicator(const QString &_name,
                  const QString &_iconName,
                  const QString &_label = "",
                  const QString &_iconThemePath = "");
    virtual ~QAppIndicator();
    void setMenu(QAppIndicatorMenu *_menu);
    void show();
    void hide();
    void setLabel(const QString &_label);
    void setIconName(const QString &_iconName);
    void setIconThemePath(const QString &_path);

signals:
    void menuSet(QAppIndicatorMenu *_menu);
    void shown(bool _visible);
    void labelChanged(QString _label);
    void iconNameChanged(QString _iconName);
    void iconThemePathChanged(QString _path);

   public slots:
    void onMenuSet(QAppIndicatorMenu *_menu);
    void onShown(bool _visible);
    void onLabelChanged(QString _label);
    void onIconNameChanged(QString _iconName);
    void onIconThemePathChanged(QString _path);

   private:
    AppIndicator *m_appIndicator;
    QAppIndicatorMenu *m_menu;
    QString m_name;
    QString m_iconName;
    QString m_label;
};
}  // namespace dqtx
