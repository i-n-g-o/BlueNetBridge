import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Rectangle {

    property bool show_testbutton: btn_show_testbutton.checked
    property bool show_statusbar: btn_show_statusbar.checked

    anchors.top: parent.bottom
    anchors.right: parent.right

    width: menu_layout.width
    height: Screen.pixelDensity * 35

    visible: parent.checked

    color: "darkgray"
    clip: true

    border.width: 1
    border.color: "dimgray"

    //
    ColumnLayout {

        id: menu_layout

        //width: parent.width
        height: parent.height

        layoutDirection: Qt.RightToLeft

        CheckboxMenuItem {
            id: btn_show_testbutton
            text: "Show Testbutton"
        }

        CheckboxMenuItem {
            id: btn_show_statusbar
            text: "Show Statusbar"
        }

        ButtonMenuItem {
            text: "Clear Settings"
            onClicked: {
                console.log("clear settings");
                device.clearSettings();
                tools_button.checked = false;
            }
        }
    }
}
