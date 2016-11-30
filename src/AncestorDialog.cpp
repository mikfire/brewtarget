/*
 * AncestorDialog.cpp is part of Brewtarget, and is Copyright the following
 * authors 2016 - 2019
 * - Mik Firestone <mikfire@fastmail.com>
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

#include <QDebug>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <algorithm>

#include "AncestorDialog.h"
#include "MainWindow.h"
#include "BeerXMLElement.h"
#include "recipe.h"
#include "database.h"

AncestorDialog::AncestorDialog(MainWindow *parent)
   : QDialog(parent), mainWindow(parent)
{

   doLayout();
   pushButton_apply->setEnabled(false);
   comboBox_descendant->setEnabled(false);

   buildAncestorBox();

   // this does the dirty
   connect(pushButton_apply, SIGNAL(clicked()), this, SLOT(connectDescendant()));
   connect(pushButton_close, SIGNAL(clicked()), this, SLOT(reject()));

   // just some nice things
   connect( comboBox_ancestor,   SIGNAL(activated(int)), this, SLOT(ancestorSelected(int)));
   // connect( comboBox_descendant, SIGNAL(activated(int)), this, SLOT(activateButton()));
}

bool AncestorDialog::recipeLessThan(Recipe *right, Recipe *left)
{
   if ( right->name() == left->name() )
      return right->key() < left->key();

   return right->name() < left->name();
}

void AncestorDialog::buildAncestorBox()
{
   QList<Recipe*> recipes = Database::instance().recipes();
   std::sort(recipes.begin(),recipes.end(),AncestorDialog::recipeLessThan);

   foreach (Recipe* recipe, recipes) {
      if ( recipe->display() ) {
         comboBox_ancestor->addItem(recipe->name(), recipe->key());
      }
   }
}

void AncestorDialog::buildDescendantBox(Recipe *ignore)
{
   QList<Recipe*> recipes = Database::instance().recipes();
   std::sort(recipes.begin(),recipes.end(),recipeLessThan);

   //  The rules of what can be a target are complex
   foreach (Recipe* recipe, recipes) {
      // if we are ignoring the recipe, skip
      if ( recipe == ignore )
         continue;
      // if the recipe is not being displayed, skip
      if ( ! recipe->display())
         continue;
      // if the recipe already has ancestors, skip
      if ( recipe->hasAncestors() )
         continue;
      comboBox_descendant->addItem(recipe->name(), recipe->key());
   }
}

void AncestorDialog::connectDescendant()
{
   Recipe *ancestor, *descendant;

   ancestor   = Database::instance().recipe( comboBox_ancestor->currentData().toInt() );
   descendant = Database::instance().recipe( comboBox_descendant->currentData().toInt() );

   // No loops in the inheritance
   if ( ! descendant->isMyAncestor(ancestor) )
   {
      descendant->setAncestor(ancestor);

      emit ancestoryChanged(ancestor,descendant);
   }

   // disable the apply button
   pushButton_apply->setEnabled(false);

   // reset the descendant box
   comboBox_descendant->setEnabled(false);
   comboBox_descendant->clear();

   // and rebuild the ancestors box
   comboBox_ancestor->clear();
   buildAncestorBox();
}

void AncestorDialog::ancestorSelected(int ndx)
{
   Recipe *ancestor = Database::instance().recipe( comboBox_ancestor->currentData().toInt() );
   comboBox_descendant->setEnabled(true);

   buildDescendantBox(ancestor);

   activateButton();
}

void AncestorDialog::activateButton()
{
   if ( ! pushButton_apply->isEnabled() )
      pushButton_apply->setEnabled(true);
}

// Boiler plate things to build the dialog
void AncestorDialog::doLayout()
{
   if (objectName().isEmpty())
      setObjectName(QStringLiteral("Dialog"));
   resize(300, 154);
   verticalLayout = new QVBoxLayout(this);
   verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
   horizontalWidget_descendant = new QWidget(this);
   horizontalWidget_descendant->setObjectName(QStringLiteral("horizontalWidget_descendant"));
   horizontalLayout = new QHBoxLayout(horizontalWidget_descendant);
   horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
   comboBox_ancestor = new QComboBox(horizontalWidget_descendant);
   comboBox_ancestor->setObjectName(QStringLiteral("comboBox_ancestor"));
   comboBox_ancestor->setSizeAdjustPolicy(QComboBox::AdjustToContents);

   horizontalLayout->addWidget(comboBox_ancestor);

   horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   horizontalLayout->addItem(horizontalSpacer_2);


   verticalLayout->addWidget(horizontalWidget_descendant);

   label_descendant = new QLabel(this);
   label_descendant->setObjectName(QStringLiteral("label_descendant"));

   verticalLayout->addWidget(label_descendant);

   horizontalWidget_ancestor = new QWidget(this);
   horizontalWidget_ancestor->setObjectName(QStringLiteral("horizontalWidget_ancestor"));
   horizontalLayout_2 = new QHBoxLayout(horizontalWidget_ancestor);
   horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
   comboBox_descendant = new QComboBox(horizontalWidget_ancestor);
   comboBox_descendant->setObjectName(QStringLiteral("comboBox_descendant"));
   comboBox_descendant->setSizeAdjustPolicy(QComboBox::AdjustToContents);

   horizontalLayout_2->addWidget(comboBox_descendant);

   horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   horizontalLayout_2->addItem(horizontalSpacer_3);


   verticalLayout->addWidget(horizontalWidget_ancestor);

   horizontalLayout_3 = new QHBoxLayout();
   horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
   pushButton_apply = new QPushButton(this);
   pushButton_apply->setObjectName(QStringLiteral("pushButton_apply"));

   horizontalLayout_3->addWidget(pushButton_apply);

   horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   horizontalLayout_3->addItem(horizontalSpacer);

   pushButton_close = new QPushButton(this);
   pushButton_close->setObjectName(QStringLiteral("pushButton_close"));

   horizontalLayout_3->addWidget(pushButton_close);


   verticalLayout->addLayout(horizontalLayout_3);
   retranslateUi();

   QMetaObject::connectSlotsByName(this);
}

void AncestorDialog::retranslateUi()
{
   setWindowTitle( tr("Ancestory") );

   label_descendant->setText(tr("begat"));

   pushButton_apply->setText(tr("Apply"));
   pushButton_close->setText(tr("Close"));
} // retranslateUi

