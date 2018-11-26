import QtQuick 2.6
import QtQuick.Window 2.2

import BorderlessWindow 1.0

BorderlessWindow {
    id: _borderlessWindow

    width: 500
    visible: true
    height: 500
    color: "aqua"

    Rectangle {
        id: _top

        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width
        height: 20
        color: "purple"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onContainsMouseChanged: parent.opacity = containsMouse ? 0.5 : 1.0
        }
    }

    Rectangle {
        id: _bottom

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        height: 20
        color: "red"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onContainsMouseChanged: parent.opacity = containsMouse ? 0.5 : 1.0
        }
    }

    Rectangle {
        id: _left

        anchors.left: parent.left
        anchors.top: parent.top
        width: 20
        height: parent.height
        color: "#00FF00"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onContainsMouseChanged: parent.opacity = containsMouse ? 0.5 : 1.0
        }
    }

    Rectangle {
        id: _right

        anchors.right: parent.right
        anchors.top: parent.top
        width: 20
        height: parent.height
        color: "#0000FF"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onContainsMouseChanged: parent.opacity = containsMouse ? 0.5 : 1.0
        }
    }

    Rectangle {
        id: _rotatingRect

        property bool animate: true

        anchors.centerIn: parent
        width: 200
        height: width
        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_F10) {
                animate = !animate
            }
        }

        RotationAnimation on rotation {
            running: _rotatingRect.animate
            loops: Animation.Infinite
            from: 0
            to: 360
            duration: 1000
        }
    }
}
