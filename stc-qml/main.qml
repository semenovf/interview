import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

ApplicationWindow {
    id: appWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Minimal Qml")

    Action {
        id: quitAction
        text: qsTr("Quit");
        shortcut: "Ctrl+Q"
        onTriggered: Qt.quit();
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { action: quitAction }
        }
    }

    ColumnLayout {
        id: appLayout
        spacing: 0
        anchors.fill: parent

        Rectangle {
            id: redRec
            color: "red"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Text {
                color: "white"
                text: "Red Rectangle"
                anchors.right: redRec.right
//                 anchors.verticalCenter: redRec.verticalCenter
                anchors.top: redRec.top
            }
        }

        Rectangle {
            color: "blue"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            color: "green"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            color: "#505050"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            color: "yellow"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Text {
        color: "white"
        text: "Window pos: ("
                    + appWindow.x
                    + ", "
                    + appWindow.y + ")\n"
            + "Window size: ("
                    + appWindow.width
                    + ", "
                    + appWindow.height + ")\n"
            + "Screen size: ("
                    + Screen.width
                    + ", "
                    + Screen.height + ")\n"
            + "Desktop size: ("
                    + Screen.desktopAvailableWidth
                    + ", "
                    + Screen.desktopAvailableHeight + ")\n"
        anchors.horizontalCenter: appLayout.horizontalCenter
        anchors.verticalCenter: appLayout.verticalCenter
    }
}
