import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

Rectangle {

    id: delegate_box

    height: Screen.pixelDensity * 20
    width: parent.width

    property bool show_testbutton: false

    border.width: 1
    border.color: "darkgray"
    radius: 5

    Rectangle {
        anchors.fill: parent
        color: enabled ? ( (modelData.connected ? "yellowgreen" : "lightsteelblue") ) : "lightgray"
        opacity: 0.5
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (!modelData.connected) {
                info_text.text = "connecting..."
                device.connectDevice(modelData.deviceAddress);
            } else {
                info_text.text = "disconnecting..."
                device.disconnectDevice(modelData.deviceAddress);
            }
        }
    }


    Item {
        id: device_info
        width: 10 + (deviceName.width > deviceAddress.width ? deviceName.width : deviceAddress.width)
        height: parent.height

        Text {

            id: deviceName

            Layout.alignment: Qt.AlignLeft

            text: modelData.deviceName
            font.pointSize: 20
            color: enabled ? "black" : "darkgray"

            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -deviceAddress.height+5
            //anchors.top: parent.top
            anchors.topMargin: 5

            Text {
                id: deviceAddress
                text: modelData.deviceAddress
                font.pointSize: parent.font.pointSize*0.5
                color: enabled ? "black" : "darkgray"

                anchors.top: parent.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
            }
        }
    }


    Text {

        id: info_text

        // dynamic width
        anchors.left: device_info.right
        anchors.leftMargin: 10
        anchors.right: item_box_right.left
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        height: parent.height / 2
        clip: true

        visible: width > 20 ? true : false

        text: ""

        font.family: iconFont.name
        font.pixelSize: 20

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WrapAnywhere
//        elide: Text.ElideMiddle

        OpacityAnimator on opacity {
            id: coloranim;
            from: 1.0;
            to: 0.0;
            duration: 1000;
            running: false

            onRunningChanged: {
                if (!running) {
                    console.log("stopped");
                    info_text.text = "";
                }
            }
        }

        onTextChanged: {
            // fade-out
            if (text != "") {
                coloranim.restart();
            }
        }
    }


    RowLayout {

        id: item_box_right

        spacing: 10

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10


        Button {
            width: 100
            text: "Test"
            visible: show_testbutton

            onClicked: {
                modelData.testDevice();
            }
        }

        Item {
            id: item_busy_indicator
            width: 40
            height: 40

            BusyIndicator {

                anchors.centerIn: parent
                running: modelData.connecting


                style: BusyIndicatorStyle {
                    indicator: Text {

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

}
