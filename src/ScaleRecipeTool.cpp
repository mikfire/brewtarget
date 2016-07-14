/*
 * ScaleRecipeTool.cpp is part of Brewtarget, and is Copyright the following
 * authors 2009-2015
 * - Philip Greggory Lee <rocketman768@gmail.com>
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

#include "ScaleRecipeTool.h"
#include <QMessageBox>
#include <QButtonGroup>
#include "brewtarget.h"
#include "recipe.h"
#include "fermentable.h"
#include "mash.h"
#include "mashstep.h"
#include "hop.h"
#include "misc.h"
#include "yeast.h"
#include "water.h"
#include "database.h"
#include "equipment.h"
#include "EquipmentListModel.h"
#include "BeerXMLSortProxyModel.h"

ScaleRecipeTool::ScaleRecipeTool(QWidget* parent) :
   QWizard(parent),
   equipListModel(new EquipmentListModel(this)),
   equipSortProxyModel(new BeerXMLSortProxyModel(equipListModel))
{
   addPage(new ScaleRecipeIntroPage);
   addPage(new ScaleRecipeEquipmentPage(equipSortProxyModel));
}

void ScaleRecipeTool::accept() {
   int row = field("equipComboBox").toInt();
   QModelIndex equipProxyNdx( equipSortProxyModel->index(row, 0) );
   QModelIndex equipNdx = equipSortProxyModel->mapToSource(equipProxyNdx);

   Equipment* selectedEquip = equipListModel->at(equipNdx.row());
   double newEff = field("effLineEdit").toString().toDouble();
   scale(selectedEquip, newEff);

   QWizard::accept();
}

void ScaleRecipeTool::setRecipe(Recipe* rec)
{
   recObs = rec;
}

void ScaleRecipeTool::scale(Equipment* equip, double newEff)
{
   if( recObs == 0 || equip == 0 )
      return;
   
   // Calculate volume ratio
   double currentBatchSize_l = recObs->batchSize_l();
   double newBatchSize_l = equip->batchSize_l();
   double volRatio = newBatchSize_l / currentBatchSize_l;
   
   // Calculate efficiency ratio
   double oldEfficiency = recObs->efficiency_pct();
   double effRatio = oldEfficiency / newEff;
   
   Database::instance().addToRecipe(recObs, equip);
   recObs->setBatchSize_l(newBatchSize_l);
   recObs->setBoilSize_l(equip->boilSize_l());
   recObs->setEfficiency_pct(newEff);
   recObs->setBoilTime_min(equip->boilTime_min());
   
   foreach( Fermentable* ferm, recObs->fermentables() )
   {
      if( !ferm->isSugar() && !ferm->isExtract() ) {
         ferm->setAmount_kg(ferm->amount_kg() * effRatio * volRatio);
      }
      else {
         ferm->setAmount_kg(ferm->amount_kg() * volRatio);
      }
   }
   
   foreach(Hop* hop, recObs->hops() )
      hop->setAmount_kg(hop->amount_kg() * volRatio);
 
   foreach( Misc* misc, recObs->miscs() )
      misc->setAmount( misc->amount() * volRatio );
  
   foreach( Water* water,recObs->waters() )
      water->setAmount_l(water->amount_l() * volRatio);

   Mash* mash = recObs->mash();
   if( mash == 0 )
      return;
   
   foreach( MashStep* step, mash->mashSteps()  ) {
      // Reset all these to zero so that the user
      // will know to re-run the mash wizard.
      step->setDecoctionAmount_l(0);
      step->setInfuseAmount_l(0);
   }
   
   // I don't think I should scale the yeasts.
   
   // Let the user know what happened.
   QMessageBox::information(this, tr("Recipe Scaled"),
             tr("The equipment and mash have been reset due to the fact that mash temperatures do not scale easily. Please re-run the mash wizard.") );
}

// ScaleRecipeIntroPage =======================================================

ScaleRecipeIntroPage::ScaleRecipeIntroPage(QWidget* parent) :
   QWizardPage(parent),
   layout(new QVBoxLayout),
   label(new QLabel) {

   doLayout();
   retranslateUi();
}

void ScaleRecipeIntroPage::doLayout() {
   setPixmap(QWizard::WatermarkPixmap, QPixmap(":images/brewtarget.svg"));

   layout->addWidget(label);
      label->setWordWrap(true);
   setLayout(layout);
}

void ScaleRecipeIntroPage::retranslateUi() {
   setTitle(tr("Scale Recipe"));
   label->setText(tr(
      "This wizard will help you scale a recipe to another size or efficiency."
      "Select another equipment with the new batch size and/or efficiency and"
      "the wizard will scale the recipe ingredients automatically."
   ));
}

// ScaleRecipeEquipmentPage ===================================================

ScaleRecipeEquipmentPage::ScaleRecipeEquipmentPage(QAbstractItemModel* listModel, QWidget* parent) :
   QWizardPage(parent),
   layout(new QFormLayout),
   equipLabel(new QLabel),
   equipComboBox(new QComboBox),
   equipListModel(listModel),
   effLabel(new QLabel),
   effLineEdit(new QLineEdit) {

   doLayout();
   retranslateUi();

   registerField("equipComboBox", equipComboBox);
   registerField("effLineEdit", effLineEdit);
}

void ScaleRecipeEquipmentPage::doLayout() {

   layout->addRow(equipLabel, equipComboBox);
      equipComboBox->setModel(equipListModel);
   layout->addRow(effLabel, effLineEdit);
      effLineEdit->setText("70.0");
   setLayout(layout);
}

void ScaleRecipeEquipmentPage::retranslateUi() {
   setTitle(tr("Select Equipment"));
   setSubTitle(tr("The recipe will be scaled to match the batch size and "
                  "efficiency of the selected equipment"
   ));

   equipLabel->setText(tr("New Equipment"));
   effLabel->setText(tr("New Efficiency (%)"));
}
