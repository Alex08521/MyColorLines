import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import GameLogic 1.0

Window {
    id: appWindow
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    maximumWidth: 800
    maximumHeight: 600
    visible: true
    title: qsTr("Color Lines by Alex Pluzhinskyi")
    color: "lightgray"

    GameLogic{
        id: gameLogic
    }

    GridLayout{
        id: gridLayout

        anchors{
            fill: parent
            topMargin: 20
            bottomMargin: 20
            leftMargin: 20
            rightMargin: 20+(parent.width - parent.width/4*3)
        }

        rows: 9
        columns: 9

        Repeater{
            id:playField
            model: 81
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id:field
                    anchors.fill: parent
                    color: "white"
                    border.color: "black"
                    border.width: 4
                    radius: width/10
                }

                Rectangle{
                    id:circle
                    anchors.fill: parent
                    anchors.margins: 5
                    radius: width/2
                    opacity: 0
                    border.color: "black"
                    border.width: 4
                    visible: false
                    color: gameLogic.readField(index) !== "N" ? gameLogic.readField(index) === "R" ? "red" :
                                                                gameLogic.readField(index) === "G" ? "green" : gameLogic.readField(index) === "B" ? "blue" :
                                                                gameLogic.readField(index) === "Y" ? "purple" : "black" : "white";
                }

                MouseArea {
                    id: mousearea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                    onClicked: {
                        if(mouse.button == Qt.LeftButton) gameLogic.ballMoving(index);
                        else if(mouse.button == Qt.RightButton) gameLogic.choiseCancel();
                    }   
                }

                OpacityAnimator {
                  id: opacityAnim
                  target: circle
                  duration: 2000
                }

                NumberAnimation {
                    id: numberAnim
                    targets: circle
                    properties: "x, y"
                    duration: 1000
                }

                Timer {
                       interval: 250
                       repeat: true
                       running: true

                       onTriggered:{
                           circle.color = gameLogic.readField(index) !== "N" ? gameLogic.readField(index) === "R" ? "red" :
                                          gameLogic.readField(index) === "G" ? "green" : gameLogic.readField(index) === "B" ? "blue" :
                                          gameLogic.readField(index) === "Y" ? "purple" : "black" : "white";

                           if(circle.visible && circle.opacity === 1 && index === gameLogic.getActiveIndex()){
                               opacityAnim.from = 1;
                               opacityAnim.to = 0;
                               opacityAnim.duration = 100;
                               opacityAnim.running = true;
                           }

                           circle.visible = gameLogic.readField(index) !== "N";

                           if(circle.visible && circle.opacity < 1){
                               opacityAnim.from = 0;
                               opacityAnim.to = 1;
                               opacityAnim.running = true;
                               opacityAnim.duration = 2000;
                           }

                           scoreText.text = qsTr(gameLogic.getScoresString());
                           gameOverScape.visible = gameLogic.isGameOver();
                           gameOver.visible = gameLogic.isGameOver();
                       }
                }   
            }
        }
    }


    ColumnLayout {
        id: coll
        spacing: 15
        anchors.left: gridLayout.right
        anchors.right: parent.right

        Item {
           height: 60
        }

        Text{
            id: scoreString
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 26
            text: qsTr("Твои очки:")
        }

        Rectangle {
            id: scoreScape
            border.color: "black"
            color: "white"
            radius: width/10
            height: scoreText.height+5
            width: scoreText.width+8
            Layout.alignment: Qt.AlignCenter

            Text {
                id: scoreText
                anchors.centerIn: parent
                font.pixelSize: 26
                text: qsTr(gameLogic.getScoresString())
            }
        }

        Item {
           height: 60
        }

        Button {
            id: bNewGame
            text: qsTr("Новая Игра")
            font.pointSize: 16
            Layout.alignment: Qt.AlignCenter

            background: Rectangle {
                        implicitWidth: 90
                        implicitHeight: 30
                        color: bNewGame.down ? "gray" : "white"
                        border.color: bNewGame.down ? "gray" : "black"
                        border.width: 4
                        radius: 4
            }

            onClicked: {
                gameLogic.newGame();
            }
        }

        Button {
            id: bQuit
            width: bNewGame.width
            text: qsTr("Закрыть")
            font.pointSize: 16
            Layout.alignment: Qt.AlignCenter

            background: Rectangle {
                        implicitWidth: 90
                        implicitHeight: 30
                        color: bQuit.down ? "gray" : "white"
                        border.color: bQuit.down ? "gray" : "black"
                        border.width: 4
                        radius: 4
            }

            onClicked: {
                 appWindow.close()
            }
        }

        Item {
           height: 60
        }

        Text{
            id: header
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 36
            text: qsTr("Color lineS")
        }

        Item {
           Layout.fillHeight: true
        }
    }

    Rectangle {
        id: gameOverScape
        anchors.centerIn: parent
        anchors.leftMargin: 60
        border.color: "black"
        color: "white"
        radius: width/10
        height: gameOver.height+5
        width: gameOver.width+8
        visible: false

        Text {
            id: gameOver
            anchors.centerIn: parent
            visible: false
            font.pixelSize: 98
            color: "black"
            text: qsTr("Игра окончена")
        }
    }
}
