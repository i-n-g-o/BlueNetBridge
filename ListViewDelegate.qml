import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {

    id: delegate_box

    height: 80
    width: parent.width

    color: enabled ? ( (modelData.connected ? "green" : "lightsteelblue") ) : "lightgray"

    border.width: 1
    border.color: "darkgray"
    radius: 5

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (!modelData.connected) {
                device.connectDevice(modelData.deviceAddress);
            } else {
                device.disconnectDevice(modelData.deviceAddress);
            }
        }
    }

    Text {
        id: deviceName
        text: modelData.deviceName
        font.pointSize: 20
        color: enabled ? "black" : "darkgray"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    Text {
        id: deviceAddress
        text: modelData.deviceAddress
        font.pointSize: deviceName.font.pointSize*0.7
        color: enabled ? "black" : "darkgray"

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
    }


    RowLayout {

        spacing: 50

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10

        BusyIndicator {
            anchors.centerIn: parent
            running: modelData.connecting


            style: BusyIndicatorStyle {
                indicator: Text {
                    width: 40
                    height: 40

                    text: "🔄"
                    font { family: iconFont.name; pixelSize: 24;}

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    visible: control.running

                    RotationAnimator on rotation {
                        running: control.running
                        loops: Animation.Infinite
                        duration: 2000
                        from: 360 ; to: 0
                    }
                }
//                indicator: Image {
//                    visible: control.running
//                    source: "spinner.png"
//                    RotationAnimator on rotation {
//                        running: control.running
//                        loops: Animation.Infinite
//                        duration: 2000
//                        from: 0 ; to: 360
//                    }
//                }
            }
        }

//        Button {
//            text: "Test"
//            height: 30

//            onClicked: {
//                modelData.writeData("test");
//            }
//        }

        Text {
            id: ok
            width: 40
            text: "★"
            color: "#727272"
            font { family: iconFont.name; pixelSize: 40;}
            visible: modelData.connected
        }
    }


}
