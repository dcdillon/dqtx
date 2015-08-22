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

namespace dqtx
{
class QAppIndicator : public QObject
{
    Q_OBJECT
   public:
    static void menuItemActivatedExternal(GtkMenu *_menu, gpointer _data);
   public:
    QAppIndicator(const QString &_name, const QString &_iconName, const QString &_label);
    virtual ~QAppIndicator();
    void addMenuItem(const QString &_label);
    void show();
    void hide();
    void setLabel(const QString &_label);
    void setIconName(const QString &_iconName);
    
   protected:
    class MenuItem
    {
       public:
        QAppIndicator *m_indicator;
        QString m_label;
        GtkWidget *m_item;
    };
    
    void menuItemActivatedInternal(MenuItem *_item);
    
   signals:
    void menuItemAdded(QString _label);
    void menuItemActivated(QString _label, GtkWidget *_item);
    void shown(bool _visible);
    void labelChanged(QString _label);
    void iconNameChanged(QString _iconName);
    
   public slots:
    void onMenuItemAdded(QString _label);
    void onShown(bool _visible);
    void onLabelChanged(QString _label);
    void onIconNameChanged(QString _iconName);
    
   private:
    AppIndicator *m_appIndicator;
    GtkWidget *m_menu;
    QString m_name;
    QString m_iconName;
    QString m_label;
    QList< MenuItem * > m_menuItems;
};
}  // namespace dqtx
