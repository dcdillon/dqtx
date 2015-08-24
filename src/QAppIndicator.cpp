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
QAppIndicator::QAppIndicator(const QString &_name,
                             const QString &_iconName,
                             const QString &_label,
                             const QString &_iconThemePath)
    : m_menu(NULL), m_name(_name), m_iconName(_iconName), m_label(_label)
{
    connect(this,
            SIGNAL(menuSet(QAppIndicatorMenu *)),
            this,
            SLOT(onMenuSet(QAppIndicatorMenu *)));
    connect(this, SIGNAL(shown(bool)), this, SLOT(onShown(bool)));
    connect(this,
            SIGNAL(labelChanged(QString)),
            this,
            SLOT(onLabelChanged(QString)));
    connect(this,
            SIGNAL(iconNameChanged(QString)),
            this,
            SLOT(onIconNameChanged(QString)));
    connect(this,
            SIGNAL(iconThemePathChanged(QString)),
            this,
            SLOT(onIconThemePathChanged(QString)));

    m_appIndicator =
        app_indicator_new(m_name.toLocal8Bit().data(),
                          m_iconName.toLocal8Bit().data(),
                          APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

    if (_iconThemePath != "")
    {
        app_indicator_set_icon_theme_path(m_appIndicator,
                                          _iconThemePath.toLocal8Bit().data());
    }

    app_indicator_set_label(m_appIndicator, m_label.toLocal8Bit().data(), NULL);
}

QAppIndicator::~QAppIndicator()
{
    //g_object_unref(G_OBJECT(m_appIndicator));
}

void QAppIndicator::setMenu(QAppIndicatorMenu *_menu)
{
    emit menuSet(_menu);
}

void QAppIndicator::show() { emit shown(true); }

void QAppIndicator::hide() { emit shown(false); }

void QAppIndicator::setLabel(const QString &_label)
{
    emit labelChanged(_label);
}

void QAppIndicator::setIconName(const QString &_iconName)
{
    emit iconNameChanged(_iconName);
}

void QAppIndicator::setIconThemePath(const QString &_path)
{
    emit iconThemePathChanged(_path);
}

void QAppIndicator::onMenuSet(QAppIndicatorMenu *_menu)
{
    if (m_menu)
    {
        delete m_menu;
    }
    
    m_menu = _menu;
    app_indicator_set_menu(m_appIndicator, GTK_MENU(_menu->m_menu));
    _menu->setParent(this);
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
    app_indicator_set_icon_full(m_appIndicator,
                                m_iconName.toLocal8Bit().data(),
                                m_iconName.toLocal8Bit().data());
}

void QAppIndicator::onIconThemePathChanged(QString _path)
{
    app_indicator_set_icon_theme_path(m_appIndicator,
                                      _path.toLocal8Bit().data());
}

}  // namespace dqtx
