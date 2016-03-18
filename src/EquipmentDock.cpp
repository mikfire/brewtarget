/*
 * EquipmentDock.cpp is part of Brewtarget, and is Copyright the following
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
#include "EquipmentDock.h"
#include "equipment.h"
#include "database.h"
#include "config.h"
#include "unit.h"
#include "brewtarget.h"

EquipmentDock::EquipmentDock( QWidget* parent)
   : QDockWidget(parent), obsEquipment(0)
{
   setupUi(this);
}

void EquipmentDock::setEquipment( Equipment* e )
{
   if( obsEquipment )
      disconnect( obsEquipment, 0, this, 0 );
   
   obsEquipment = e;
   if( obsEquipment )
      showChanges();
}

void EquipmentDock::clear()
{
   setEquipment(0);
}

void EquipmentDock::changed(QMetaProperty prop, QVariant /*val*/)
{
   if( sender() == obsEquipment )
      showChanges();
}

void EquipmentDock::showChanges()
{
   Equipment *e = obsEquipment;
   if( e == 0 )
   {
      clear();
      return;
   }

   // Get weight and volume units for grain absorption.

   //equipmentComboBox->setIndexByEquipment(e);

   lineEdit_tunVolume->setText(e);
   lineEdit_tunWeight->setText(e);
   lineEdit_tunSpecificHeat->setText(e);

   lineEdit_evaporationRate->setText(e);
   lineEdit_topUpKettle->setText(e);
   lineEdit_topUpWater->setText(e);

   lineEdit_trubChillerLoss->setText(e);
   lineEdit_lauterDeadspace->setText(e);

}
