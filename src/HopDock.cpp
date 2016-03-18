/*
 * HopDock.cpp is part of Brewtarget, and is Copyright the following
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

#include <QtGui>
#include <QIcon>
#include "HopDock.h"
#include "hop.h"
#include "database.h"
#include "config.h"
#include "unit.h"
#include "brewtarget.h"

HopDock::HopDock( QWidget* parent)
   : QDockWidget(parent), obsHop(0)
{
   setupUi(this);
}

void HopDock::setHop( Hop* h )
{
   if( obsHop )
      disconnect( obsHop, 0, this, 0 );
   
   obsHop = h;
   if( obsHop )
   {
      connect( obsHop, SIGNAL(changed(QMetaProperty,QVariant)), this, SLOT(changed(QMetaProperty,QVariant)) );
      showChanges();
   }
}

void HopDock::clear()
{
   setHop(0);
}

void HopDock::changed(QMetaProperty prop, QVariant /*val*/)
{
   if( sender() == obsHop )
      showChanges(&prop);
}

void HopDock::showChanges(QMetaProperty* prop)
{
   bool updateAll = false;
   QString propName;
   if( obsHop == 0 )
      return;

   if( prop == 0 )
      updateAll = true;
   else
   {
      propName = prop->name();
   }
   
   if( propName == "name" || updateAll )
   {
      lineEdit_name->setText(obsHop->name());
      lineEdit_name->setCursorPosition(0);
      if( ! updateAll )
         return;
   }
   if( propName == "beta_pct" || updateAll ) {
      lineEdit_beta->setText(obsHop);
      if( ! updateAll )
         return;
   }
   if( propName == "hsi_pct" || updateAll ) {
      lineEdit_HSI->setText(obsHop);
      if( ! updateAll )
         return;
   }
   if( propName == "origin" || updateAll )
   {
      lineEdit_origin->setText(obsHop->origin());
      lineEdit_origin->setCursorPosition(0);
      if( ! updateAll )
         return;
   }
   if( propName == "humulene_pct" || updateAll ) {
      lineEdit_humulene->setText(obsHop);
      if( ! updateAll )
         return;
   }
   if( propName == "caryophyllene_pct" || updateAll ) {
      lineEdit_caryophyllene->setText(obsHop);
      if( ! updateAll )
         return;
   }
   if( propName == "cohumulone_pct" || updateAll ) {
      lineEdit_cohumulone->setText(obsHop);
      if( ! updateAll )
         return;
   }
   if( propName == "myrcene_pct" || updateAll ) {
      lineEdit_myrcene->setText(obsHop);
      if( ! updateAll )
         return;
   }
}
