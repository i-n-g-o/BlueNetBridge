import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

CheckBox {



    Layout.preferredHeight: 36

    Layout.fillWidth: true
    Layout.leftMargin: 10
    Layout.rightMargin: 10

    style: CheckBoxStyle {
        label: Text {
            color: "white"
            text: control.text
            font { family: iconFont.name; pixelSize: Screen.pixelDensity * 3;}

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        width: parent.width
        height: 1

        anchors.top: parent.bottom

        border.width: 1
        border.color: "darkgrey"

        color: "#00000000"
        opacity: 0.2
    }

    onClicked: {
        tools_button.checked = false;
    }
}
