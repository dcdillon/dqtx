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

#include <dqtx/QIconTheme.hpp>
#include <QFile>
#include <QTextStream>

namespace dqtx
{
QIconTheme::QIconTheme()
{
    m_uuid = QUuid::createUuid();
    m_dir = QDir::temp();
    m_dir.mkdir(m_uuid.toString());
    m_dir.cd(m_uuid.toString());

    QFile file(m_dir.absolutePath() + QString("/index.theme"));
    file.open(QIODevice::WriteOnly);
    QTextStream outfile(&file);

    outfile << "[Icon Theme]\n"
            << "Name=QIconTheme\n"
            << "Comment=Temporary icon theme\n"
            << "Hidden=true\n";
    file.close();
}

QIconTheme::~QIconTheme()
{
    QStringList files = m_dir.entryList();
    QStringList::iterator i = files.begin();
    QStringList::iterator iend = files.end();

    for (; i != iend; ++i)
    {
        m_dir.remove(*i);
    }

    m_dir.cdUp();
    m_dir.rmdir(m_uuid.toString());
}

void QIconTheme::addIcon(const QIcon &_icon, const QString &_name)
{
    m_lock.lock();
    _icon.pixmap(48).toImage().save(m_dir.absolutePath() + QString("/") +
                                    _name + QString(".png"));
    m_iconByName[_name] = _icon;
    m_lock.unlock();
}

bool QIconTheme::hasIcon(const QString &_name) const
{
    bool retval = false;

    m_lock.lock();
    QMap< QString, QIcon >::const_iterator i = m_iconByName.find(_name);

    if (i != m_iconByName.end())
    {
        retval = true;
    }
    m_lock.unlock();

    return retval;
}

QDir QIconTheme::dir() const { return m_dir; }

}  // namespace dqtx
