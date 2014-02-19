/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef IVERSIONCONTROL_H
#define IVERSIONCONTROL_H

#include "core_global.h"
#include "id.h"

#include <QDateTime>
#include <QFlags>
#include <QHash>
#include <QObject>
#include <QString>

namespace Core {

class CORE_EXPORT IVersionControl : public QObject
{
    Q_OBJECT
    Q_ENUMS(SettingsFlag Operation)
public:
    enum SettingsFlag {
        AutoOpen = 0x1
    };
    Q_DECLARE_FLAGS(SettingsFlags, SettingsFlag)

    enum Operation {
        AddOperation, DeleteOperation, MoveOperation,
        CreateRepositoryOperation,
        SnapshotOperations,
        AnnotateOperation,
        CheckoutOperation,
        GetRepositoryRootOperation
        };

    enum OpenSupportMode {
        NoOpen,        /*!< Files can be edited without noticing the VCS */
        OpenOptional,  /*!< Files can be opened by the VCS, or hijacked */
        OpenMandatory  /*!< Files must always be opened by the VCS */
    };

    class CORE_EXPORT TopicCache
    {
    public:
        virtual ~TopicCache();
        QString topic(const QString &topLevel);

    protected:
        virtual QString trackFile(const QString &repository) = 0;
        virtual QString refreshTopic(const QString &repository) = 0;

    private:
        struct TopicData
        {
            QDateTime timeStamp;
            QString topic;
        };

        QHash<QString, TopicData> m_cache;

    };

    explicit IVersionControl(TopicCache *topicCache = 0) : m_topicCache(topicCache) {}
    virtual ~IVersionControl();

    virtual QString displayName() const = 0;
    virtual Id id() const = 0;

    /*!
     * Returns whether files in this directory should be managed with this
     * version control.
     * If \a topLevel is non-null, it should return the topmost directory,
     * for which this IVersionControl should be used. The VcsManager assumes
     * that all files in the returned directory are managed by the same IVersionControl.
     */

    virtual bool managesDirectory(const QString &filename, QString *topLevel = 0) const = 0;

    /*!
     * Returns whether \a fileName is managed by this version control.
     *
     * \a workingDirectory is assumed to be part of a valid repository (not necessarily its
     * top level). \a fileName is expected to be relative to workingDirectory.
     */
    virtual bool managesFile(const QString &workingDirectory, const QString &fileName) const = 0;

    /*!
     * Returns true is the VCS is configured to run.
     */
    virtual bool isConfigured() const = 0;
    /*!
     * Called to query whether a VCS supports the respective operations.
     *
     * Return false if the VCS is not configured yet.
     */
    virtual bool supportsOperation(Operation operation) const = 0;

    /*!
     * Returns the open support mode for \a fileName.
     */
    virtual OpenSupportMode openSupportMode(const QString &fileName) const;

    /*!
     * Called prior to save, if the file is read only. Should be implemented if
     * the scc requires a operation before editing the file, e.g. 'p4 edit'
     *
     * \note The EditorManager calls this for the editors.
     */
    virtual bool vcsOpen(const QString &fileName) = 0;

    /*!
     * Returns settings.
     */

    virtual SettingsFlags settingsFlags() const { return 0; }

    /*!
     * Called after a file has been added to a project If the version control
     * needs to know which files it needs to track you should reimplement this
     * function, e.g. 'p4 add', 'cvs add', 'svn add'.
     *
     * \note This function should be called from IProject subclasses after
     *       files are added to the project.
     */
    virtual bool vcsAdd(const QString &filename) = 0;

    /*!
     * Called after a file has been removed from the project (if the user
     * wants), e.g. 'p4 delete', 'svn delete'.
     */
    virtual bool vcsDelete(const QString &filename) = 0;

    /*!
     * Called to rename a file, should do the actual on disk renaming
     * (e.g. git mv, svn move, p4 move)
     */
    virtual bool vcsMove(const QString &from, const QString &to) = 0;

    /*!
     * Called to initialize the version control system in a directory.
     */
    virtual bool vcsCreateRepository(const QString &directory) = 0;

    /*!
     * Called to clone/checkout the version control system in a directory.
     */
    virtual bool vcsCheckout(const QString &directory, const QByteArray &url) = 0;

    /*!
     * Called to get the version control repository root.
     */
    virtual QString vcsGetRepositoryURL(const QString &directory) = 0;

    /*!
     * Topic (e.g. name of the current branch)
     */
    virtual QString vcsTopic(const QString &topLevel);

    /*!
     * Display annotation for a file and scroll to line
     */
    virtual bool vcsAnnotate(const QString &file, int line) = 0;

    /*!
     * Display text for Open operation
     */
    virtual QString vcsOpenText() const;

    /*!
     * Display text for Make Writable
     */
    virtual QString vcsMakeWritableText() const;

signals:
    void repositoryChanged(const QString &repository);
    void filesChanged(const QStringList &files);
    void configurationChanged();

private:
    TopicCache *m_topicCache;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Core::IVersionControl::SettingsFlags)

} // namespace Core

#if defined(WITH_TESTS)

#include <QSet>

namespace Core {

class CORE_EXPORT TestVersionControl : public IVersionControl
{
    Q_OBJECT
public:
    TestVersionControl(Core::Id id, const QString &name) :
        m_id(id), m_displayName(name), m_dirCount(0), m_fileCount(0)
    { }
    ~TestVersionControl();

    void setManagedDirectories(const QHash<QString, QString> &dirs);
    void setManagedFiles(const QSet<QString> &files);

    int dirCount() const { return m_dirCount; }
    int fileCount() const { return m_fileCount; }

    // IVersionControl interface
    QString displayName() const { return m_displayName; }
    Id id() const { return m_id; }
    bool managesDirectory(const QString &filename, QString *topLevel) const;
    bool managesFile(const QString &workingDirectory, const QString &fileName) const;
    bool isConfigured() const { return true; }
    bool supportsOperation(Operation) const { return false; }
    bool vcsOpen(const QString &) { return false; }
    bool vcsAdd(const QString &) { return false; }
    bool vcsDelete(const QString &) { return false; }
    bool vcsMove(const QString &, const QString &) { return false; }
    bool vcsCreateRepository(const QString &) { return false; }
    bool vcsCheckout(const QString &, const QByteArray &) { return false; }
    QString vcsGetRepositoryURL(const QString &) { return QString(); }
    bool vcsAnnotate(const QString &, int) { return false; }

private:
    Id m_id;
    QString m_displayName;
    QHash<QString, QString> m_managedDirs;
    QSet<QString> m_managedFiles;
    mutable int m_dirCount;
    mutable int m_fileCount;
};

} // namespace Core
#endif


#endif // IVERSIONCONTROL_H
