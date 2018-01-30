import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2


ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    minimumHeight: 300
    minimumWidth: 300

    FontLoader { id: iconFont; source: "entypo.ttf" }

    header: AppToolBar {}
    footer: AppStatusBar {}


    MessageDialog {
        id: messageDialog
        visible: false
        modality: Qt.WindowModal //Qt.NonModal
        title: "TITLE"
        text: "TEXT"
        informativeText: "informative text"
        detailedText: "detailed text"

        standardButtons: StandardButton.Ok | StandardButton.Yes | StandardButton.Ignore

        onButtonClicked: console.log("clicked button " + clickedButton)

        onAccepted: console.log("accept")
        onRejected: console.log("reject")
        onHelp: console.log("HELP")
        onYes: console.log("YES")
        onNo: console.log("NO")
        onApply: console.log("Apply")
        onReset: console.log("Reset")
    }

    Dialog {
        id: helloDialog
        modality: Qt.WindowModal //Qt.NonModal
        title: "Hello"

        standardButtons: StandardButton.Ok | StandardButton.Cancel | StandardButton.Reset

        onButtonClicked: console.log("clicked button " + clickedButton)



        onAccepted: console.log("acctpted")
        onRejected: console.log("reject")
        onHelp: console.log("help")
        onYes: console.log("YES")
        onNo: console.log("NO")
        onApply: console.log("APPLY")
        onReset: console.log("reset")

        Label {
            text: "Hello world!"
        }
//        Rectangle {
//            id: inforect
//            height: 100
//            border.width: 1
//            radius: 5

//            anchors.fill: parent



//            RoundButton {
//                text: "bla"
//                onClicked: console.log("CLICK")
//            }
//        }

    }

//    Dialog {
//        id: spinboxDialog
//        modality: Qt.WindowModal //Qt.NonModal
//        title: "Spinbox"
//        onHelp: {
//            lastChosen.text = "No help available, sorry.  Please answer the question."
//            visible = true
//        }
//        onButtonClicked: {
//            if (clickedButton === StandardButton.Ok && answer.value == 11.0)
//                lastChosen.text = "You are correct!"
//            else
//                lastChosen.text = "Having failed to give the correct answer, you shall not pass!"
//        }

//        ColumnLayout {
//            id: column
//            width: parent ? parent.width : 100
//            Label {
//                text: "<b>What</b> is the average airspeed velocity of an unladen European swallow?"
//                Layout.columnSpan: 2
//                Layout.fillWidth: true
//                wrapMode: Text.WordWrap
//            }
//            RowLayout {
//                Layout.alignment: Qt.AlignHCenter
//                SpinBox {
//                    id: answer
//                    onEditingFinished: spinboxDialog.click(StandardButton.Ok)
//                }
//                Label {
//                    text: "m/s"
//                    Layout.alignment: Qt.AlignBaseline | Qt.AlignLeft
//                }
//            }
//        }
//    }

    Rectangle {
        id: rect
        clip: true
        gradient: Gradient {
            GradientStop { position: 0; color: "#efefef"}
            GradientStop {position: 1; color: "#b9b9b9"}
        }
        border.color: "#949494"
        border.width: 1

        anchors.fill: parent

//        RoundButton {
//            width: parent.width / 3
//            height: 100

//            anchors.centerIn: parent
//            visible: list_view.count == 0
//            text: qsTr("Scan Bluetooth...")
//            font.bold: true

//            onClicked: {
//                device.startDeviceDiscovery()
//            }
//        }

        ScrollView {
            id: scrollView
            height: 480
            clip: true

            anchors.fill: parent

            padding: 10

            enabled: !device.scanning

            ListView {
                id: list_view

                width: parent.width
                orientation: ListView.Vertical
                layoutDirection: Qt.LeftToRight
                spacing: 4
                snapMode: ListView.NoSnap

                model: device.devicesList
                delegate: ListViewDelegate {}

                //visible : count > 0

                Label {
                    id: emptyLabel
                    text: qsTr("Nothing to show yet!")
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.bold: true

                    visible: parent.count == 0

                    anchors.centerIn: parent
                }
            }


        }

//        Rectangle {
//            id: thingie
//            width: 30
//            height: 30
//            y: -list_view.contentY
//            color: "blue"



//            property real rot: 0

//            transform: Rotation { origin.x: 15; origin.y: 15; angle: thingie.rot}

//            anchors.horizontalCenter: parent.horizontalCenter

//            Behavior on y {
//                id: behavy
//                enabled: false
//                SpringAnimation {
//                    spring: 8
//                    damping: 0.6
//                    mass: 1
//                }
//            }

//            Behavior on rot {
//                id: behavRot
//                enabled: false
//                SpringAnimation {
//                    spring: 8
//                    damping: 0.6
//                    mass: 1
//                }
//            }

//            //anchors.top: list_view.contentY
//        }

        BusyIndicator {
            anchors.centerIn: parent
            running: device.scanning
        }
    }

}
