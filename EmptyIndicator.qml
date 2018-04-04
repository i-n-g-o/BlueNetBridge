import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

Label {

    anchors.centerIn: parent
    anchors.verticalCenterOffset: -big_scan_button.height/2

    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter

    text: qsTr("No BLE Serial devices found.")
    font.bold: true

    color: "black"

    // button to refresh
    Button {
        id: big_scan_button

        width: Screen.pixelDensity * 13
        height: Screen.pixelDensity * 13

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        anchors.topMargin: 10


        clip: true

        text: qsTr("🔄")

        style: ButtonStyleFlat {
            fontsize: 12
            colorOn: "darkgray"
            colorOff: "black"
            colorDisabled: "gray"
            bgOpacity: 0.3
        }

        onClicked: {
            device.startDeviceDiscovery()
        }
    }
}
