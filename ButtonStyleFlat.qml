import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

ButtonStyle {

    property int fontsize: 6
    property string fontfamily: iconFont.name
    property int textHAlignment: Text.AlignHCenter
    property string colorOn: "lightgray"
    property string colorOff: "white"
    property string colorDisabled: "darkgray"
    property real bgOpacity: 0.

    label: Text {
        color: control.enabled ? (control.pressed | control.checked ? colorOn : colorOff) : colorDisabled
        text: control.text
        font { family: fontfamily; pixelSize: Screen.pixelDensity * fontsize;}

        horizontalAlignment: textHAlignment
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        opacity: bgOpacity
        radius: 5
    }
}
