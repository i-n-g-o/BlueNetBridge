import QtQuick 2.0
import QtQuick.Dialogs 1.3

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


//Dialog {
//    id: helloDialog
//    modality: Qt.WindowModal //Qt.NonModal
//    title: "Hello"

//    standardButtons: StandardButton.Ok | StandardButton.Cancel | StandardButton.Reset

//    onButtonClicked: console.log("clicked button " + clickedButton)



//    onAccepted: console.log("acctpted")
//    onRejected: console.log("reject")
//    onHelp: console.log("help")
//    onYes: console.log("YES")
//    onNo: console.log("NO")
//    onApply: console.log("APPLY")
//    onReset: console.log("reset")

//    Label {
//        text: "Hello world!"
//    }
////        Rectangle {
////            id: inforect
////            height: 100
////            border.width: 1
////            radius: 5

////            anchors.fill: parent



////            RoundButton {
////                text: "bla"
////                onClicked: console.log("CLICK")
////            }
////        }

//}

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
