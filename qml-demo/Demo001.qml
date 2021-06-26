import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

ApplicationWindow {
    id: appWindow
    visible: true

    Image {
        id: bgImage
        source: "Demo001/background.png"
    }

    minimumWidth: bgImage.width
    maximumWidth: bgImage.width
    minimumHeight: bgImage.height
    maximumHeight: bgImage.height

    Text {
        text: qsTr("Click left mouse button to rotate the wheel")
    }

    Image {
        id: pole
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        source: "Demo001/pole.png"
    }

    Image {
        id: wheel
        anchors.centerIn: parent
        source: "Demo001/pinwheel.png"

        Behavior on rotation {
            NumberAnimation {
                duration: 150
            }
        }
    }

    Image {
        id: wheelNegativeOffset
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -wheel.width
        source: "Demo001/pinwheel.png"
    }

    Image {
        id: wheelPositiveOffset
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: +wheel.width
        source: "Demo001/pinwheel.png"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: wheel.rotation += 45
    }
}
