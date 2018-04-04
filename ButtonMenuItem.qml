import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Particles 2.0
import QtQuick.Shapes 1.0

Button {
    Layout.fillWidth: true
    Layout.leftMargin: 10
    Layout.rightMargin: 10
    Layout.preferredHeight: 36
    //Layout.alignment: Qt.AlignLeft

    style: ButtonStyleFlat {
        fontsize: 3
        textHAlignment: Text.AlignLeft
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
}
