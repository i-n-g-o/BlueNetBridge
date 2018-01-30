import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

StatusBar {

    id: statusbar
    height: Screen.pixelDensity * 15

    Row {

        anchors.fill: parent
        spacing: 10

        MyButton {
            width: parent.height
            height: parent.height

            text: "+"

            onClicked: {
                console.log("aaaa");
                device.addSomething();
            }
        }


        MyButton {
            width: parent.height
            height: parent.height

            text: "open"

            onClicked: {
                messageDialog.open()
            }
        }

        MyButton {
            width: parent.height
            height: parent.height

            text: "clear"

            onClicked: {
                device.clearSettings();
            }
        }
    }
}

