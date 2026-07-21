import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 1000
    height: 700
    title: "Шахматы (Сетевая игра)"
    
    property var selectedCell: null
    property var cachedBoardState: []
    property bool isNetworkMode: false

    Connections {
        target: gameWindow
        function onMessageShown(message) {
            messageLabel.text = message
            messageTimer.start()
        }
        function onBoardUpdated() {
            cachedBoardState = gameWindow.getBoardState()
        }
        function onConnectionStatusChanged(status) {
            networkStatusLabel.text = status
            if (status !== "") {
                messageLabel.text = status
                messageTimer.start()
            }
        }
        function onNetworkGameChanged() {
            isNetworkMode = gameWindow.isNetworkGame
        }
    }

    Timer {
        id: messageTimer
        interval: 3000
        running: false
        onTriggered: {
            messageLabel.text = ""
        }
    }

    Component.onCompleted: {
        gameWindow.startNewGame(0, 0)
        cachedBoardState = gameWindow.getBoardState()
        isNetworkMode = false
    }

    MenuBar {
        id: menuBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        Menu {
            title: "Игра"
            MenuItem {
                text: "Новая игра (Игрок vs Игрок)"
                onTriggered: {
                    gameWindow.startNewGame(0, 0)
                    statusLabel.text = "Режим: Игрок vs Игрок"
                    cachedBoardState = gameWindow.getBoardState()
                    selectedCell = null
                    isNetworkMode = false
                }
            }
            MenuItem {
                text: "Новая игра (Вы играете белыми vs ИИ)"
                onTriggered: {
                    gameWindow.startNewGame(1, 0)
                    statusLabel.text = "Режим: Вы играете белыми"
                    cachedBoardState = gameWindow.getBoardState()
                    selectedCell = null
                    isNetworkMode = false
                }
            }
            MenuItem {
                text: "Новая игра (Вы играете чёрными vs ИИ)"
                onTriggered: {
                    gameWindow.startNewGame(1, 1)
                    statusLabel.text = "Режим: Вы играете чёрными"
                    cachedBoardState = gameWindow.getBoardState()
                    selectedCell = null
                    isNetworkMode = false
                }
            }
            MenuItem {
                text: "Новая игра (Сетевая)"
                onTriggered: {
                    gameWindow.startNewGame(2, 0)
                    statusLabel.text = "Режим: Сетевая игра"
                    cachedBoardState = gameWindow.getBoardState()
                    selectedCell = null
                    isNetworkMode = true
                }
            }
        }
        Menu {
            title: "Партия"
            MenuItem {
                text: "Сохранить историю в файл"
                onTriggered: gameWindow.saveGame("chess_history.txt")
            }
            MenuSeparator {}
            MenuItem {
                text: "Выход"
                onTriggered: Qt.quit()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.topMargin: menuBar.height
        anchors.margins: 15
        spacing: 20

        Rectangle {
            Layout.alignment: Qt.AlignCenter
            width: 560
            height: 560
            color: "#b58863"
            border.color: "#4a3018"
            border.width: 6
            
            Grid {
                id: boardGrid
                anchors.centerIn: parent
                rows: 8
                columns: 8
                spacing: 0
                Repeater {
                    model: 64
                    delegate: Rectangle {
                        width: 70
                        height: 70
                        property int row: Math.floor(index / 8)
                        property int col: index % 8
                        property var cellData: (cachedBoardState && cachedBoardState.length > index) ? cachedBoardState[index] : null
                        
                        color: {
                            if (!cellData) return (row + col) % 2 === 0 ? "#f0d9b5" : "#b58863"
                            var isLight = (row + col) % 2 === 0
                            var baseColor = isLight ? "#f0d9b5" : "#b58863"
                            return cellData.isSelected === true ? "#e6c229" : baseColor
                        }

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 50
                            font.family: "Segoe UI Symbol"
                            visible: cellData ? cellData.hasPiece === true : false
                            text: {
                                if (!cellData || cellData.hasPiece !== true) return ""
                                var symbols = {
                                    "white_king": "♔", "white_queen": "♕", "white_rook": "♖",
                                    "white_bishop": "♗", "white_knight": "♘", "white_pawn": "♙",
                                    "black_king": "♚", "black_queen": "♛", "black_rook": "♜",
                                    "black_bishop": "♝", "black_knight": "♞", "black_pawn": "♟"
                                }
                                return symbols[cellData.color + "_" + cellData.type] || "?"
                            }
                        }

                        Rectangle {
                            anchors.centerIn: parent
                            width: 22; height: 22; radius: 11
                            color: "#40000000"
                            visible: cellData ? (cellData.isLegalMove === true && cellData.hasPiece !== true) : false
                        }

                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 3
                            color: "transparent"
                            border.color: "#99dc3232"
                            border.width: 4
                            radius: 4
                            visible: cellData ? (cellData.isLegalMove === true && cellData.hasPiece === true) : false
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            enabled: gameWindow.canMakeMove
                            onClicked: handleCellClick(row, col, index)
                        }
                    }
                }
            }

            Rectangle {
                anchors.fill: parent
                color: "#80000000"
                visible: isNetworkMode && !gameWindow.canMakeMove
                z: 10
                Column {
                    anchors.centerIn: parent
                    spacing: 10
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "⏳ Ожидание противника..."
                        font.pixelSize: 24
                        font.bold: true
                        color: "white"
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: gameWindow.isServer ? "Ожидание подключения игрока" : "Подключение к серверу..."
                        font.pixelSize: 14
                        color: "#cccccc"
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.preferredWidth: 320
            spacing: 15

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                color: "#f5f5f5"
                border.color: "#dcdcdc"
                border.width: 1
                radius: 4
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 8
                    Text {
                        id: statusLabel
                        text: "Режим: Игрок vs Игрок"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#333333"
                    }
                    Text {
                        id: currentPlayerLabel
                        text: {
                            try { return gameWindow.currentPlayer === 0 ? "Ход: Белые" : "Ход: Чёрные" } 
                            catch (e) { return "Ход: Белые" }
                        }
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#e3f2fd"
                border.color: "#90caf9"
                border.width: 1
                radius: 4
                visible: isNetworkMode
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8
                    
                    Text {
                        text: "Сетевая игра"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#1565c0"
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        Text { text: "IP:"; font.pixelSize: 12; width: 30 }
                        TextField {
                            id: ipField
                            Layout.fillWidth: true
                            text: "127.0.0.1"
                            font.pixelSize: 12
                            padding: 4
                            // ИСПРАВЛЕНО: надежная блокировка во время подключения или игры
                            enabled: !gameWindow.isConnecting && !gameWindow.isServer && !gameWindow.isConnected
                        }
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        Text { text: "Порт:"; font.pixelSize: 12; width: 30 }
                        TextField {
                            id: portField
                            Layout.fillWidth: true
                            text: "5555"
                            validator: IntValidator { bottom: 1024; top: 65535 }
                            font.pixelSize: 12
                            padding: 4
                            // ИСПРАВЛЕНО: надежная блокировка во время подключения или игры
                            enabled: !gameWindow.isConnecting && !gameWindow.isServer && !gameWindow.isConnected
                        }
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        Button {
                            text: "Создать"
                            Layout.fillWidth: true
                            // ИСПРАВЛЕНО: кнопка активна только если мы не подключаемся, не сервер и еще не подключены
                            enabled: !gameWindow.isConnecting && !gameWindow.isServer && !gameWindow.isConnected
                            onClicked: {
                                gameWindow.startServer(parseInt(portField.text))
                                statusLabel.text = "Режим: Сетевая игра (Сервер)"
                            }
                        }
                        Button {
                            text: "Подключиться"
                            Layout.fillWidth: true
                            // ИСПРАВЛЕНО: кнопка активна только если мы не подключаемся, не сервер и еще не подключены
                            enabled: !gameWindow.isConnecting && !gameWindow.isServer && !gameWindow.isConnected
                            onClicked: {
                                gameWindow.connectToServer(ipField.text, parseInt(portField.text))
                                statusLabel.text = "Режим: Сетевая игра (Клиент)"
                            }
                        }
                    }
                    
                    Text {
                        id: networkStatusLabel
                        text: ""
                        font.pixelSize: 11
                        color: "#d32f2f"
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: "#fff8e1"
                border.color: "#ffb300"
                border.width: 1
                radius: 4
                Text {
                    id: messageLabel
                    anchors.centerIn: parent
                    text: ""
                    font.bold: true
                    font.pixelSize: 16
                    color: "#d32f2f"
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#ffffff"
                border.color: "#dcdcdc"
                border.width: 1
                radius: 4
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0
                    Text {
                        text: "История партии"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#555555"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 8
                        Layout.bottomMargin: 4
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: "#eeeeee"
                    }
                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        ListView {
                            id: listView
                            anchors.fill: parent
                            model: gameWindow.moveHistory
                            delegate: RowLayout {
                                width: listView.width
                                height: 30
                                spacing: 5
                                Text {
                                    text: modelData.number + ". "
                                    width: 35
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 13
                                    color: "#666666"
                                }
                                Text {
                                    text: modelData.white
                                    width: 120
                                    font.pixelSize: 13
                                    font.family: "Courier New"
                                }
                                Text {
                                    text: modelData.black
                                    width: 120
                                    font.pixelSize: 13
                                    font.family: "Courier New"
                                }
                            }
                            onCountChanged: {
                                if (count > 0) positionViewAtIndex(count - 1, ListView.End)
                            }
                        }
                    }
                }
            }
        }
    }

    function handleCellClick(row, col, index) {
        if (!gameWindow.canMakeMove) return
        if (!cachedBoardState || cachedBoardState.length <= index) return
        
        var cellData = cachedBoardState[index]
        if (selectedCell) {
            if (selectedCell.row === row && selectedCell.col === col) {
                selectedCell = null
                gameWindow.clearSelection()
                cachedBoardState = gameWindow.getBoardState()
                return
            }
            gameWindow.makeMove(selectedCell.row, selectedCell.col, row, col)
            selectedCell = null
        } else if (cellData && cellData.hasPiece === true) {
            selectedCell = {row: row, col: col}
            gameWindow.selectPiece(row, col)
            cachedBoardState = gameWindow.getBoardState()
        }
    }
}