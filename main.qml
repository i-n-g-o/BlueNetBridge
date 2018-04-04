import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Particles 2.0
import QtQuick.Shapes 1.0

ApplicationWindow {
    id: applicationWindow
    title: qsTr("BLEBridge")

    width: 640
    height: 480

    minimumHeight: 300
    minimumWidth: 300

    visible: true

    FontLoader { id: iconFont; source: "entypo.ttf" }

    // Controls 1.4
//    toolBar: AppToolBar {}
    header: AppToolBar {
        id: tb
    }

    footer: AppStatusBar {
        id: statusbar
        visible: tb.show_statusbar
    }

    // main view
    Rectangle {

        clip: true

        gradient: Gradient {
            GradientStop { position: 0; color: "#efefef"}
            GradientStop {position: 1; color: "#b9b9b9"}
        }
        border.color: "#949494"
        border.width: 1

        anchors.fill: parent

        // scrolll view
        ScrollView {
            id: scrollView            

            anchors.fill: parent
            enabled: !device.scanning
            clip: true

            ListView {
                id: list_view

                width: parent.width
                orientation: ListView.Vertical
                layoutDirection: Qt.LeftToRight
                spacing: 4
                snapMode: ListView.NoSnap

                model: device.devicesList
                delegate: ListViewDelegate {
                    show_testbutton: tb.show_testbutton
                }

                EmptyIndicator {
                    id: emptyLabel
                    visible: parent.count == 0
                }
            }
        }


        BusyIndicator {
            anchors.centerIn: parent
//            anchors.verticalCenterOffset: -100
            scale: 2.0
            running: device.scanning
        }
    }

}
