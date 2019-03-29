import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import Qt.labs.settings 1.0


ApplicationWindow {
    id: appWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Calculator")

    // Save and restore settings on open and close window respectivly
    Settings {
        property alias x: appWindow.x
        property alias y: appWindow.y
        property alias width: appWindow.width
        property alias height: appWindow.height
    }

    Rectangle {
        anchors.fill: parent
        color: "red"

        GridLayout {
            id: grid
            columns: 5
            anchors.margins: 10
            anchors.fill: parent

            TextField {
                id: input
                //TODO style: { background-color: white; }
                verticalAlignment: TextInput.AlignVCenter
                horizontalAlignment: TextInput.AlignRight
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 5
                cursorVisible: false
            }

            // First row
            Button { text: "<-";  font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "C";   font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "+/-"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "=";   font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }

            // Second row
            Button { text: "7"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "8"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "9"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "/"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }

            // Third row
            Button { text: "4"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "5"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "6"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "*"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }

            // Fourth row
            Button { text: "1"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "2"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "3"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "-"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }

            // Fifth row
            Button { text: "0"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true; Layout.columnSpan: 2 }
            Button { text: ","; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }
            Button { text: "+"; font.bold: true; flat: false; Layout.fillHeight: true; Layout.fillWidth: true }

    //         Text { text: "words"; color: "red" }
    //         Text { text: "in"; font.underline: true }
    //         Text { text: "a"; font.pixelSize: 20 }
    //         Text { text: "row"; font.strikeout: true }
        }
    }

//     Action {
//         id: quitAction
//         text: qsTr("Quit");
//         shortcut: "Ctrl+Q"
//         onTriggered: Qt.quit();
//     }
//
//     menuBar: MenuBar {
//         Menu {
//             title: "File"
//             MenuItem { action: quitAction }
//         }
//     }
//
//     ColumnLayout {
//         id: appLayout
//         spacing: 0
//         anchors.fill: parent
//
//         Rectangle {
//             id: redRec
//             color: "red"
//             Layout.fillHeight: true
//             Layout.fillWidth: true
//             Text {
//                 color: "white"
//                 text: "Red Rectangle"
//                 anchors.right: redRec.right
// //                 anchors.verticalCenter: redRec.verticalCenter
//                 anchors.top: redRec.top
//             }
//         }
//
//         Rectangle {
//             color: "blue"
//             Layout.fillHeight: true
//             Layout.fillWidth: true
//         }
//
//         Rectangle {
//             color: "green"
//             Layout.fillHeight: true
//             Layout.fillWidth: true
//         }
//
//         Rectangle {
//             color: "#505050"
//             Layout.fillHeight: true
//             Layout.fillWidth: true
//         }
//
//         Rectangle {
//             color: "yellow"
//             Layout.fillHeight: true
//             Layout.fillWidth: true
//         }
//     }
//
//     Text {
//         color: "white"
//         text: "Window pos: ("
//                     + appWindow.x
//                     + ", "
//                     + appWindow.y + ")\n"
//             + "Window size: ("
//                     + appWindow.width
//                     + ", "
//                     + appWindow.height + ")\n"
//             + "Screen size: ("
//                     + Screen.width
//                     + ", "
//                     + Screen.height + ")\n"
//             + "Desktop size: ("
//                     + Screen.desktopAvailableWidth
//                     + ", "
//                     + Screen.desktopAvailableHeight + ")\n"
//         anchors.horizontalCenter: appLayout.horizontalCenter
//         anchors.verticalCenter: appLayout.verticalCenter
//     }
}
