import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

StatusBar {

    id: statusbar
    height: Screen.pixelDensity * 15


    MyButton {
        id: roundButton
        width: parent.height
        height: parent.height

        text: "+"

        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        anchors.verticalCenter: parent.verticalCenter

        onClicked: {
            console.log("aaaa");
            device.addSomething();
        }
    }


    MyButton {
        id: dialogopen
        width: parent.height
        height: parent.height

        text: "open"

        anchors.left: roundButton.right
        anchors.leftMargin: 10
        anchors.top: roundButton.top
        anchors.verticalCenter: parent.verticalCenter

        onClicked: {
            messageDialog.open()
        }
    }
}

