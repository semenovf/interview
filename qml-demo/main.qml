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
    title: qsTr("QML Demo")

    // Save and restore settings on open and close window respectivly
    Settings {
        property alias x: appWindow.x
        property alias y: appWindow.y
        property alias width: appWindow.width
        property alias height: appWindow.height
    }

//     Action {
//         id: quitAction
//         text: qsTr("Quit");
//         shortcut: "Ctrl+Q"
//         onTriggered: Qt.quit();
//     }

//     menuBar: MenuBar {
//         Menu {
//             title: "File"
//             MenuItem { action: quitAction }
//         }
//     }

    ColumnLayout {
        id: appLayout
        spacing: 0
        anchors.fill: parent

        Rectangle {
            id: content
            color: "green"
            Layout.fillHeight: true
            Layout.fillWidth: true
            //anchors.fill: parent

            Text {
                id: contentText
                color: "white"
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: controls
            color: "whitesmoke"
//             Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            Layout.minimumHeight: 50

            RowLayout {
                anchors.fill: parent
                spacing: 0

                Button {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    flat: true

                    Text {
                        text: "Previous"
                        color: "black"
                        anchors.centerIn: parent
                    }

                    onClicked: { contentText.text = "Prev" }
                }

                Rectangle {
                    Layout.fillHeight: true
                    width: 1
                    color: "darkgrey"
                }

                Button {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    flat: true

                    Text {
                        text: "Next"
                        color: "black"
                        anchors.centerIn: parent
                    }

                    onClicked: { contentText.text = "Next" }
                }
            }
        }

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
    }
}

