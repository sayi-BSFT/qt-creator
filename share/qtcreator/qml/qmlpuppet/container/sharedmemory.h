/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef QMLDESIGNER_SHAREDMEMORY_H
#define QMLDESIGNER_SHAREDMEMORY_H

#include <QSharedMemory>
#include <QSystemSemaphore>

namespace QmlDesigner {

class SharedMemory
{
    friend class SharedMemoryLocker;
public:
    SharedMemory();
    SharedMemory(const QString &key);
    ~SharedMemory();

    void setKey(const QString &key);
    QString key() const;

    bool create(int size, QSharedMemory::AccessMode mode = QSharedMemory::ReadWrite);
    int size() const;

    bool attach(QSharedMemory::AccessMode mode = QSharedMemory::ReadWrite);
    bool isAttached() const;
    bool detach();

    void *data();
    const void* constData() const;
    const void *data() const;

    bool lock();
    bool unlock();

    QSharedMemory::SharedMemoryError error() const;
    QString errorString() const;

protected:
#ifdef Q_OS_UNIX
    bool initKeyInternal();
    bool cleanHandleInternal();
    bool createInternal(QSharedMemory::AccessMode mode, int size);
    bool attachInternal(QSharedMemory::AccessMode mode);
    bool detachInternal();
    int handle();

    void setErrorString(const QString &function);
#endif
private:
#ifndef Q_OS_UNIX
    QSharedMemory m_sharedMemory;
#else
    void *m_memory;
    int m_size;
    QString m_key;
    QByteArray m_nativeKey;
    QSharedMemory::SharedMemoryError m_error;
    QString m_errorString;
    QSystemSemaphore m_systemSemaphore;
    bool m_lockedByMe;
    int m_fileHandle;
    bool m_createdByMe;
#endif
};

} // namespace QmlDesigner

#endif // QMLDESIGNER_SHAREDMEMORY_H
