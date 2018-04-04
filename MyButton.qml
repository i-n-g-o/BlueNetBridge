import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Button {

    Gradient {
        id: btngradient1
        GradientStop { position: 0.0; color: "white" }
        GradientStop { position: 1.0; color: "lightgray" }
    }
    Gradient {
        id: btngradient2
        GradientStop { position: 0.0; color: "lightgray" }
        GradientStop { position: 1.0; color: "lightgray" }
    }

    style: ButtonStyle {
        background: Rectangle {
            gradient: !pressed ? btngradient1 : btngradient2
            radius: 10
        }
        label: Label {
            text: control.text
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter | Text.AlignVCenter
            //anchors.horizontalCenter: parent.parent.horizontalCenter
        }
    }
}
