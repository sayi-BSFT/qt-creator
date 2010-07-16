/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Denis Mingulov.
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#include "classviewnavigationwidgetfactory.h"
#include "classviewnavigationwidget.h"
#include "classviewconstants.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/qtcassert.h>

#include <QtGui/QKeySequence>
#include <QtCore/QSettings>

namespace ClassView {
namespace Internal {

///////////////////////////////// NavigationWidget //////////////////////////////////

/*!
   \struct NavigationWidgetFactoryPrivate
   \brief Private class data for \a NavigationWidgetFactory
   \sa NavigationWidgetFactory
 */
struct NavigationWidgetFactoryPrivate
{
    //! constructor
    NavigationWidgetFactoryPrivate() : naviPaneEnabled(false) {}

    //! NavigationWidgetFactory instance
    static NavigationWidgetFactory *instance;

    //! Navigation pane visibility
    bool naviPaneEnabled;
};

// static variable initialization
NavigationWidgetFactory *NavigationWidgetFactoryPrivate::instance = 0;

///////////////////////////////// NavigationWidgetFactory //////////////////////////////////

NavigationWidgetFactory::NavigationWidgetFactory()
    : d_ptr(new NavigationWidgetFactoryPrivate())
{
}

NavigationWidgetFactory::~NavigationWidgetFactory()
{
    NavigationWidgetFactoryPrivate::instance = 0;
}

NavigationWidgetFactory *NavigationWidgetFactory::instance()
{
    if (!NavigationWidgetFactoryPrivate::instance)
        NavigationWidgetFactoryPrivate::instance = new NavigationWidgetFactory();
    return NavigationWidgetFactoryPrivate::instance;
}

QString NavigationWidgetFactory::displayName() const
{
    return tr("Class View");
}

QString NavigationWidgetFactory::id() const
{
    return QLatin1String(Constants::CLASSVIEWNAVIGATION_ID);
}

QKeySequence NavigationWidgetFactory::activationSequence() const
{
    return QKeySequence();
}

Core::NavigationView NavigationWidgetFactory::createWidget()
{
    Core::NavigationView navigationView;
    NavigationWidget *widget = new NavigationWidget();
    navigationView.widget = widget;
    navigationView.dockToolBarWidgets = widget->createToolButtons();
    emit widgetIsCreated();
    return navigationView;
}

QString NavigationWidgetFactory::settingsPrefix(int position) const
{
    QChar sep('/');
    QString group = QLatin1String(Constants::CLASSVIEW_SETTINGS_GROUP) + sep;
    group += QLatin1String(Constants::CLASSVIEW_SETTINGS_TREEWIDGET_PREFIX)
             + QString::number(position) + sep;
    return  group;
}

void NavigationWidgetFactory::saveSettings(int position, QWidget *widget)
{
    NavigationWidget *pw = qobject_cast<NavigationWidget *>(widget);
    QTC_ASSERT(pw, return);

    QSettings *settings = Core::ICore::instance()->settings();
    QTC_ASSERT(settings, return);

    // .beginGroup is not used - to prevent simultaneous access
    QString group = settingsPrefix(position);

    // save settings
    settings->setValue(group + Constants::CLASSVIEW_SETTINGS_FLATMODE, pw->flatMode());
}

void NavigationWidgetFactory::restoreSettings(int position, QWidget *widget)
{
    NavigationWidget *pw = qobject_cast<NavigationWidget *>(widget);
    QTC_ASSERT(pw, return);

    QSettings *settings = Core::ICore::instance()->settings();
    QTC_ASSERT(settings, return);

    // .beginGroup is not used - to prevent simultaneous access
    QString group = settingsPrefix(position);

    // load settings
    pw->setFlatMode(settings->value(group + Constants::CLASSVIEW_SETTINGS_FLATMODE, false).toBool());
}

} // namespace Internal
} // namespace ClassView
