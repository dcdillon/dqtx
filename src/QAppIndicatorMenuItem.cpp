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

#include <dqtx/QAppIndicatorMenuItem.hpp>

namespace dqtx
{
void QAppIndicatorMenuItem::menuItemActivatedExternal(GtkMenuItem *_item, gpointer _data)
{
    Q_UNUSED(_item);

    QAppIndicatorMenuItem *data = static_cast< QAppIndicatorMenuItem * >(_data);
    data->menuItemActivatedInternal();
}

QAppIndicatorMenuItem::QAppIndicatorMenuItem(const QString &_label)
    : m_label(_label)
{
    connect(this, SIGNAL(shown(bool)), this, SLOT(onShown(bool)));
    
    m_item = gtk_menu_item_new_with_label(m_label.toLocal8Bit().data());
    g_signal_connect(
        m_item, "activate", G_CALLBACK(menuItemActivatedExternal), this);
}

QAppIndicatorMenuItem::~QAppIndicatorMenuItem()
{
    g_object_unref(G_OBJECT(m_item));
}

void QAppIndicatorMenuItem::show() { emit shown(true); }

void QAppIndicatorMenuItem::hide() { emit shown(false); }

void QAppIndicatorMenuItem::menuItemActivatedInternal()
{
    emit menuItemActivated();
}

void QAppIndicatorMenuItem::onShown(bool _visible)
{
    if (_visible)
    {
        gtk_widget_show(m_item);
    }
    else
    {
        gtk_widget_hide(m_item);
    }
}

}  // namespace dqtx
