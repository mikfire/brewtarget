/*
 * HopDock.h is part of Brewtarget, and is Copyright the following
 * authors 2009-2014
 * - Mik Firestone <mikfire@gmail.com>
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _HOPDOCK_H
#define   _HOPDOCK_H

class HopDock;

#include <QDockWidget>
#include <QMetaProperty>
#include <QVariant>
#include "ui_hopDock.h"

// Forward declarations.
class Hop;

/*!
 * \class HopDock
 * \author Mik Firestone
 *
 * \brief Simple class to display info about a hop
 */
class HopDock : public QDockWidget, private Ui_hopDock
{
   Q_OBJECT

public:
   HopDock(QWidget *parent=0);
   virtual ~HopDock() {}
   //! Edit the given hop.
   void setHop( Hop* h );

public slots:
   //! Clear the dialog and close it.
   void clear();
   void changed(QMetaProperty,QVariant);

private:
   Hop* obsHop;

   /*! Updates the UI elements based on \b prop.
    *  If null, updates all UI elements.
    */
   void showChanges(QMetaProperty* prop = 0);
};

#endif   /* _HOPDOCK_H */
