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

#ifndef CLASSVIEWUTILS_H
#define CLASSVIEWUTILS_H

#include "classviewsymbollocation.h"
#include "classviewsymbolinformation.h"
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QSet>

QT_FORWARD_DECLARE_CLASS(QStandardItem)

namespace ClassView {
namespace Internal {

/*!
   \class Utils
   \brief Some common utils
 */

class Utils
{
    //! Private constructor
    Utils();
public:

    /*!
       \brief convert internal location container to QVariant compatible
       \param locations Set of SymbolLocations
       \return List of variant locations (can be added to an item's data)
     */
    static QList<QVariant> locationsToRole(const QSet<SymbolLocation> &locations);

    /*!
       \brief convert QVariant location container to internal
       \param locations List of variant locations (from an item's data)
       \return Set of SymbolLocations
     */
    static QSet<SymbolLocation> roleToLocations(const QList<QVariant> &locations);

    /*!
       \brief Returns sort order value for the icon type
       \param iconType Icon type
       \return Sort order value for the provided icon type
     */
    static int iconTypeSortOrder(int iconType);

    /*!
       \brief Get symbol information for the \a QStandardItem
       \param item Item
       \return Filled symbol information.
     */
    static SymbolInformation symbolInformationFromItem(const QStandardItem *item);

    /*!
       \brief Set symbol information to the \a QStandardItem
       \param information
       \param item Item
       \return Filled item
     */
    static QStandardItem *setSymbolInformationToItem(const SymbolInformation &information,
                                                     QStandardItem *item);

    /*!
       \brief Update an item to the target. (sorted, for fetching)
       \param item Source item
       \param target Target item
     */
    static void fetchItemToTarget(QStandardItem *item, const QStandardItem *target);

    /*!
       \brief Move an item to the target. (sorted)
       \param item Source item
       \param target Target item
     */
    static void moveItemToTarget(QStandardItem *item, const QStandardItem *target);
};

} // namespace Internal
} // namespace ClassView

#endif // CLASSVIEWUTILS_H
