import QtQuick 2.12
import QtGraphicalEffects 1.12

Item {
    width: 1150
    height: 100
    property alias character10: character10
    property alias character9: character9
    property alias character8: character8
    property alias character7: character7
    property alias character6: character6
    property alias character5: character5
    property alias character4: character4
    property alias character3: character3
    property alias character2: character2
    property alias character1: character1
    property alias character0: character0

    Gradient {
        id: leftGradient
        orientation: Gradient.Horizontal
        GradientStop {
            position: 1.0
            color: "blue"
        }
        GradientStop {
            position: 0.3
            color: "transparent"
        }
    }

    Gradient {
        id: rightGradient
        orientation: Gradient.Horizontal
        GradientStop {
            position: 0.0
            color: "blue"
        }
        GradientStop {
            position: 0.7
            color: "transparent"
        }
    }

    Flow {
        anchors.fill: parent
        spacing: 5
        Rectangle {
            x: 0
            width: 100
            height: 100
            opacity: 0.6
            gradient: leftGradient
            Text {
                id: character0
                color: "#20ffffff"
                text: "A"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character1
                color: "#ffffff"
                text: "B"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character2
                color: "#ffffff"
                text: "C"
                lineHeight: 1
                style: Text.Normal
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character3
                color: "#ffffff"
                text: "D"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character4
                color: "#ffffff"
                text: "E"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            Text {
                id: character5
                color: "#ffffff"
                text: "F"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 88
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character6
                color: "#ffffff"
                text: "G"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character7
                color: "#ffffff"
                text: "H"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character8
                color: "#ffffff"
                text: "I"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            color: "blue"
            opacity: 0.6
            Text {
                id: character9
                color: "#ffffff"
                text: "J"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }

        Rectangle {
            x: 0
            width: 100
            height: 100
            gradient: rightGradient
            color: "blue"
            opacity: 0.6
            Text {
                id: character10
                color: "#20ffffff"
                text: "K"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 48
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
            }
        }
    }
}
