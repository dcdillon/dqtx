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

#include <dqtx/QAppIndicator.hpp>

namespace dqtx
{
void QAppIndicator::menuItemActivatedExternal(GtkMenu *_menu, gpointer _data)
{
    Q_UNUSED(_menu);
    
    MenuItem *data = static_cast< MenuItem * >(_data);
    data->m_indicator->menuItemActivatedInternal(data);
}

QAppIndicator::QAppIndicator(const QString &_name, const QString &_iconName,
    const QString &_label) : m_name(_name), m_iconName(_iconName), m_label(_label)
{
    connect(this, SIGNAL(menuItemAdded(QString)), this, SLOT(onMenuItemAdded(QString)));
    connect(this, SIGNAL(shown(bool)), this, SLOT(onShown(bool)));
    connect(this, SIGNAL(labelChanged(QString)), this, SLOT(onLabelChanged(QString)));
    connect(this, SIGNAL(iconNameChanged(QString)), this, SLOT(onIconNameChanged(QString)));
    
    m_appIndicator = app_indicator_new(m_name.toLocal8Bit().data(), m_iconName.toLocal8Bit().data(), APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_label(m_appIndicator, m_label.toLocal8Bit().data(), NULL);
    
    m_menu = gtk_menu_new();
    app_indicator_set_menu(m_appIndicator, GTK_MENU(m_menu));
}

QAppIndicator::~QAppIndicator()
{
}

void QAppIndicator::addMenuItem(const QString &_label)
{
    emit menuItemAdded(_label);
}

void QAppIndicator::show()
{
    emit shown(true);
}

void QAppIndicator::hide()
{
    emit shown(false);
}

void QAppIndicator::setLabel(const QString &_label)
{
    emit labelChanged(_label);
}

void QAppIndicator::setIconName(const QString &_iconName)
{
    emit iconNameChanged(_iconName);
}

void QAppIndicator::menuItemActivatedInternal(MenuItem *_item)
{
    emit menuItemActivated(_item->m_label, _item->m_item);
}

void QAppIndicator::onMenuItemAdded(QString _label)
{
    MenuItem *item = new MenuItem();
    item->m_label = _label;
    item->m_item = gtk_menu_item_new_with_label(_label.toLocal8Bit().data());
    item->m_indicator = this;
    m_menuItems.push_back(item);
    gtk_menu_shell_append(GTK_MENU_SHELL(m_menu), item->m_item);
    g_signal_connect(item->m_item, "activate", G_CALLBACK(menuItemActivatedExternal), item);
    gtk_widget_show(item->m_item);
}

void QAppIndicator::onShown(bool _visible)
{
    if (_visible)
    {
        app_indicator_set_status(m_appIndicator, APP_INDICATOR_STATUS_ACTIVE);
    }
    else
    {
        app_indicator_set_status(m_appIndicator, APP_INDICATOR_STATUS_PASSIVE);
    }
}

void QAppIndicator::onLabelChanged(QString _label)
{
    m_label = _label;
    app_indicator_set_label(m_appIndicator, m_label.toLocal8Bit().data(), NULL);
}

void QAppIndicator::onIconNameChanged(QString _iconName)
{
    m_iconName = _iconName;
    app_indicator_set_icon_full(m_appIndicator, m_iconName.toLocal8Bit().data(), m_iconName.toLocal8Bit().data());
}

}  // namespace dqtx
