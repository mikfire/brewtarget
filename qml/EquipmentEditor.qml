import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

Item {
   height: 550
   width: 450
   id: equipmenteditor

   ColumnLayout {
      id: mainColumn
      anchors {
         topMargin: 10
         leftMargin: 10
         fill: parent
      }
      spacing: 10

      // Top row including equipment dropdown, delete button and set as default
      RowLayout {
         id: topMostRow
         anchors {
            fill: parent
            leftMargin: 10
         }
         Label {
            id: kitLabel
            text: qsTr("Equipment")
            font.pointSize: 16
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignHCenter
            // implicitHeight: equipmentComboBox
         }
         ComboBox {
            id: equipmentComboBox
            width: 100
            height: 100

            anchors {
               left: kitLabel.right
               leftMargin: 6
            }
            currentIndex: 0
            model: equipmentSortProxyModel
            textRole: "display"
            // This is the signal. Wonder what I do with it
            onCurrentIndexChanged: kitEditor.equipmentSelected(currentIndex)
         }

         Button {
            id: clickMe
            width: 19
            height: 17
            anchors {
               top: parent.top
               topMargin: 4
               left: equipmentComboBox.right
               leftMargin: 10
            }
            iconSource: "qrc:/images/smallMinus.svg"
            checkable: true
         }
         CheckBox {
            id: checkBox_defaultEquipment
            height: 15
            text: qsTr("Set as Default")
            anchors {
               top: parent.top
               topMargin: 10
               right: parent.right
               rightMargin: 26
            }

            onClicked: console.debug("Clicked my checkBox " + checked)
         }
      }
      // Middle panel, which holds the two main columns
      RowLayout {
         id: middlePanel
         width: 220
         anchors {
            topMargin: 30
            top: topMostRow.top
         }
         // left column for required fields and boiling&water
         ColumnLayout {
            id: requiredAndWater
            anchors {
               top: middlePanel.top
            }
            // Required fields
            GroupBox {
               id: requiredFields
               title: qsTr("Required Fields")
               // Is this really necessary?
               GridLayout {
                  id: gridInABox
                  columns: 2
                  anchors.fill: parent

                  Label {
                     id: label_name
                     text: qsTr("Name")
                  }
                  TextField {
                     id: lineEdit_name
                     text: obsEquip.name
                  }

                  Label {
                     id: label_boilSize
                     text: qsTr("Pre-boil Volume")
                  }
                  TextField {
                     id: lineEdit_boilSize
                  }

                  Label {
                     id: label_calcBoilVolume
                     text: qsTr("Calculate pre-boil volume")
                  }
                  CheckBox {
                     id: checkBox_calcBoilVolume
                     // Need behavior here, but I'm just mocking it up for right
                     // now
                  }

                  Label {
                     id: label_batchSize
                     text: qsTr("Batch Size")
                  }
                  TextField {
                     id: lineEdit_batchSize
                  }
               }
            }
            GroupBox {
               id: boilingAndWater
               title: qsTr("Boiling and Water")
               anchors {
                  top: requiredFields.bottom
               }
               GridLayout {
                  id: gridInAnotherBox
                  columns: 2
                  anchors.fill: parent

                  Label {
                     id: label_boilTime
                     text: qsTr("Boil Time");
                  }
                  TextField {
                     id: lineEdit_boilTime
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_evaporationRate
                     text: qsTr("Evaporation rate (per hr)")
                  }
                  TextField {
                     id: lineEdit_evaporationRate
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_topUpKettle
                     text: qsTr("Kettle top-up water")
                  }
                  TextField {
                     id: lineEdit_topUpKettle
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_topUpWater
                     text: qsTr("Final top-up water")
                  }
                  TextField {
                     id: lineEdit_topUpWater
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_grainAbsorption
                     text: qsTr("Grain Absorption (L/kg)")
                  }
                  TextField {
                     id: lineEdit_grainAbsorption
                     Layout.fillWidth: true
                  }

                  Button {
                     id: pushButton_absoprtion
                     Layout.columnSpan: 2
                     width: 20
                     height: 17
                     text: qsTr("Default Absorption") ;
                     //                    iconSource: qsTr("");
                  }

                  Label {
                     id: label_boilingPoint
                     text: qsTr("Boiling Point of Water")
                  }
                  TextField {
                     id: lineEdit_boilingPoint
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_hopUtilization
                     text: qsTr("Hop Utilization")
                  }
                  TextField {
                     id: lineEdit_hopUtilization
                     Layout.fillWidth: true
                  }
               }
            }
         }
         // Right column for mash tun, losses and notes
         ColumnLayout {
            id: mashTunLossesNotes
            width: 220
            anchors {
               left: requiredAndWater.right
               top: parent.top
               leftMargin: 10
               topMargin: 0
            }
            transformOrigin: Item.Center
            // Mashtun
            GroupBox {
               id: mashTun
               title: qsTr("Mash Tun")
               anchors {
                  right: mashTunLossesNotes.right
                  left: mashTunLossesNotes.left
               }
               GridLayout {
                  id: mashTunGrid
                  columns: 2
                  anchors {
                     left: parent.left
                     right: parent.right
                  }
                  height: 50

                  Label {
                     id: label_tunVolume
                     text: qsTr("Volume");
                  }
                  TextField {
                     id: lineEdit_tunVolume
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_tunWeight
                     text: qsTr("Mass")
                  }
                  TextField {
                     id: lineEdit_tunWeight
                     Layout.fillWidth: true
                  }

                  Label {
                     id: label_tunSpecificHeat
                     text: qsTr("Specific Heat (cal/(g*K))")
                  }
                  TextField {
                     id: lineEdit_tunSpecificHeat
                     Layout.fillWidth: true
                  }

               }
            }
            // Losses
            GroupBox {
               id: losses
               title: qsTr("Losses")
               anchors {
                  right: mashTunLossesNotes.right
                  left: mashTunLossesNotes.left
               }

               GridLayout {
                  id: lossesGrid
                  anchors {
                     right: parent.right
                     left: parent.left
                  }
                  columns: 2

                  Label {
                     id: label_trubChillerLoss
                     text: qsTr("Kettle to fermenter");
                  }
                  TextField {
                     id: lineEdit_trubChillerLoss
                     anchors {
                        right: parent.right
                        left: label_trubChillerLoss.right
                        leftMargin: 30
                     }
                  }

                  Label {
                     id: label_lauterDeadspace
                     text: qsTr("Latuer deadspace")
                  }
                  TextField {
                     id: lineEdit_lauterDeadspace
                     anchors {
                        right: parent.right
                        left: label_lauterDeadspace.right
                        leftMargin: 35
                     }
                  }
               }
            }
            // Notes
            GroupBox {
               id: notes
               title: qsTr("Notes")
               anchors {
                  topMargin: 27
                  //                  bottom: middlePanel.bottom
                  bottomMargin: 27
               }
               TextArea {
                  id: textEdit_notes
                  width: parent.width
                  height: parent.height

               }
            }
         }
      }
   }
}

