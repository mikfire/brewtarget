/*
 * AncestorDialog.h is part of Brewtarget, and is Copyright the following
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

#ifndef _ANCESTORDIALOG_H
#define _ANCESTORDIALOG_H

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

#include "recipe.h"
// Forward declarations.
class MainWindow;

/*!
 * \class AncestorDialog
 * \author Philip G. Lee
 *
 * \brief View/controller dialog for displaying all the yeasts in the database.
 */
class AncestorDialog : public QDialog
{
   Q_OBJECT

public:
   AncestorDialog(MainWindow* parent);
   virtual ~AncestorDialog() {}

   //! \name Public UI Variables
   //! @{
   QVBoxLayout *verticalLayout;
   QWidget *horizontalWidget_descendant;
   QHBoxLayout *horizontalLayout;
   QComboBox *comboBox_ancestor;
   QSpacerItem *horizontalSpacer_2;
   QLabel *label_descendant;
   QWidget *horizontalWidget_ancestor;
   QHBoxLayout *horizontalLayout_2;
   QComboBox *comboBox_descendant;
   QSpacerItem *horizontalSpacer_3;
   QHBoxLayout *horizontalLayout_3;
   QPushButton *pushButton_apply;
   QSpacerItem *horizontalSpacer;
   QPushButton *pushButton_close;
   //! @}

public slots:
   void connectDescendant();
   void activateButton();
   void ancestorSelected(int ndx);

signals:
   void ancestoryChanged(Recipe* ancestor, Recipe* descendant);

protected:

   virtual void changeEvent(QEvent* event)
   {
      if(event->type() == QEvent::LanguageChange)
         retranslateUi();
      QDialog::changeEvent(event);
   }

private:
   MainWindow* mainWindow;

   void doLayout();
   void retranslateUi();
   void buildAncestorBox();
   void buildDescendantBox(Recipe *ignore);
   static bool recipeLessThan(Recipe *right, Recipe *left);
};

#endif   /* _ANCESTORDIALOG_H */

