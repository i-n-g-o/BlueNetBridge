import QtQuick 2.0
//import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Particles 2.0
import QtQuick.Shapes 1.0


ToolBar {

    property bool show_testbutton: dropdown_menu.show_testbutton
    property bool show_statusbar: dropdown_menu.show_statusbar

    height: Screen.pixelDensity * 11

    style: ToolBarStyle {
        background: Rectangle {
            color: "darkgray"
        }
    }

    Label {
        id: header_label
        color: "white"
        text: "BLEBridge"
        font.pixelSize: Screen.pixelDensity * 4
        font.bold: true

        padding: 10

        anchors.verticalCenter: parent.verticalCenter
    }

    RowLayout {

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10

        spacing: 20

        Button {
            id: header_refresh_btn
            width: parent.height+10
            height: parent.height
            clip: true

            text: qsTr("🔄")

            style: ButtonStyleFlat {}

            onClicked: {
                device.startDeviceDiscovery()
            }
        }

        ToggleButton {
            id: tools_button
            width: parent.height+10
            height: parent.height

            text: qsTr("⚙️")

            style: ButtonStyleFlat {}

            // dropdownmenu
            DropdownMenu {
                id: dropdown_menu
            }
        }
    }
}
