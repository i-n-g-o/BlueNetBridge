import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.3

ToolBar {

    id: tb
    height: Screen.pixelDensity * 10

    style: ToolBarStyle {
        background: Rectangle {
            color: "darkgray"
        }
    }

    Label {
        id: header_label
        color: "white"
        text: "BLEBridge"
        font.pointSize: 14
        font.bold: true

        padding: 10

        anchors.verticalCenter: parent.verticalCenter
    }

    Button {
        id: header_refresh_btn
        width: parent.height+10
        height: parent.height
        clip: true

        style: ButtonStyle {
            label: Text {
                color: header_refresh_btn.pressed ? "lightgray" : "white"
                text: qsTr("🔄")
                font { family: iconFont.name; pixelSize: 24;}

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                opacity: 0
            }
        }

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10

        onClicked: {
            device.startDeviceDiscovery()
        }
    }
}
