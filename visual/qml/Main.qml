import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 700
    title: qsTr("PathFinding Visualizer")

    property int cellSize: 25
    property int editMode: 0
    property bool isDrawing: false
    property bool drawAsWall: true
    property int lastCellX: -1
    property int lastCellY: -1
    property var pathPoints: []

    function getCellFromPosition(x, y) {
        var gridPos = gridView.mapFromItem(mouseOverlay, x, y)
        var cellX = Math.floor(gridPos.x / cellSize)
        var cellY = Math.floor(gridPos.y / cellSize)
        if (cellX >= 0 && cellX < gridModel.width && cellY >= 0 && cellY < gridModel.height) {
            return { x: cellX, y: cellY, valid: true }
        }
        return { x: -1, y: -1, valid: false }
    }

    function applyDrawAtCell(cellX, cellY) {
        if (editMode === 0) {
            var idx = cellY * gridModel.width + cellX
            var cellType = gridModel.data(gridModel.index(idx, 0), 257)
            if (drawAsWall && cellType === 0) {
                gridModel.setWall(cellX, cellY, true)
            } else if (!drawAsWall && cellType === 1) {
                gridModel.setWall(cellX, cellY, false)
            }
        } else if (editMode === 1) {
            gridModel.setStart(cellX, cellY)
        } else if (editMode === 2) {
            gridModel.setEnd(cellX, cellY)
        }
    }

    function updatePath(path) {
        pathPoints = path
        pathCanvas.requestPaint()
    }

    RowLayout {
        anchors.fill: parent
        spacing: 10

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"

            GridView {
                id: gridView
                anchors.centerIn: parent
                width: cellSize * gridModel.width
                height: cellSize * gridModel.height
                cellWidth: cellSize
                cellHeight: cellSize
                model: gridModel
                interactive: false

                delegate: Rectangle {
                    id: cellDelegate
                    width: gridView.cellWidth
                    height: gridView.cellHeight
                    color: {
                        switch (cellType) {
                        case 0: return "#ffffff"
                        case 1: return "#333333"
                        case 2: return "#4CAF50"
                        case 3: return "#F44336"
                        default: return "#ffffff"
                        }
                    }
                    border.color: "#e0e0e0"
                    border.width: 1
                }

                Canvas {
                    id: pathCanvas
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        if (pathPoints.length < 2) return

                        ctx.strokeStyle = "#FFD700"
                        ctx.lineWidth = 3
                        ctx.lineCap = "round"
                        ctx.lineJoin = "round"

                        ctx.beginPath()
                        var startX = pathPoints[0].x * cellSize + cellSize / 2
                        var startY = pathPoints[0].y * cellSize + cellSize / 2
                        ctx.moveTo(startX, startY)

                        for (var i = 1; i < pathPoints.length; i++) {
                            var px = pathPoints[i].x * cellSize + cellSize / 2
                            var py = pathPoints[i].y * cellSize + cellSize / 2
                            ctx.lineTo(px, py)
                        }
                        ctx.stroke()
                    }
                }

                MouseArea {
                    id: mouseOverlay
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                    onPressed: function(mouse) {
                        if (pathFinderController.isSearching) return
                        var cell = getCellFromPosition(mouse.x, mouse.y)
                        if (!cell.valid) return

                        isDrawing = true
                        lastCellX = cell.x
                        lastCellY = cell.y

                        if (editMode === 0) {
                            var idx = cell.y * gridModel.width + cell.x
                            var cellType = gridModel.data(gridModel.index(idx, 0), 257)
                            drawAsWall = (cellType !== 1)
                        }
                        applyDrawAtCell(cell.x, cell.y)
                    }

                    onReleased: {
                        isDrawing = false
                        lastCellX = -1
                        lastCellY = -1
                    }

                    onPositionChanged: function(mouse) {
                        if (!isDrawing) return
                        var cell = getCellFromPosition(mouse.x, mouse.y)
                        if (!cell.valid) return
                        if (cell.x !== lastCellX || cell.y !== lastCellY) {
                            lastCellX = cell.x
                            lastCellY = cell.y
                            applyDrawAtCell(cell.x, cell.y)
                        }
                    }
                }
            }
        }

        Pane {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            padding: 15

            ColumnLayout {
                anchors.fill: parent
                spacing: 12

                Label {
                    text: qsTr("Algorithm")
                    font.bold: true
                }

                ComboBox {
                    id: algorithmCombo
                    Layout.fillWidth: true
                    model: pathFinderController.algorithms
                    currentIndex: pathFinderController.currentAlgorithm
                    onCurrentIndexChanged: pathFinderController.currentAlgorithm = currentIndex
                }

                Label {
                    text: qsTr("Heuristic")
                    font.bold: true
                    visible: pathFinderController.showHeuristic
                }

                ComboBox {
                    id: heuristicCombo
                    Layout.fillWidth: true
                    model: pathFinderController.heuristicOptions
                    currentIndex: pathFinderController.heuristic
                    onCurrentIndexChanged: pathFinderController.heuristic = currentIndex
                    visible: pathFinderController.showHeuristic
                }

                Label {
                    text: qsTr("Options")
                    font.bold: true
                    visible: pathFinderController.showOptions
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: pathFinderController.showOptions

                    CheckBox {
                        text: qsTr("Allow Diagonal")
                        checked: pathFinderController.allowDiagonal
                        onCheckedChanged: pathFinderController.allowDiagonal = checked
                        visible: pathFinderController.showAllowDiagonal
                    }

                    CheckBox {
                        text: qsTr("Bi-directional")
                        checked: pathFinderController.biDirectional
                        onCheckedChanged: pathFinderController.biDirectional = checked
                        visible: pathFinderController.showBiDirectional
                    }

                    CheckBox {
                        text: qsTr("Don't Cross Corners")
                        checked: pathFinderController.dontCrossCorners
                        onCheckedChanged: pathFinderController.dontCrossCorners = checked
                        visible: pathFinderController.showDontCrossCorners
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        visible: pathFinderController.showWeight

                        Label { text: qsTr("Weight:") }
                        SpinBox {
                            from: 1
                            to: 100
                            value: pathFinderController.weight * 10
                            onValueChanged: pathFinderController.weight = value / 10
                            stepSize: 1
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        visible: pathFinderController.showTimeLimit

                        Label { text: qsTr("Time Limit (s):") }
                        SpinBox {
                            from: 0
                            to: 60
                            value: pathFinderController.timeLimit
                            onValueChanged: pathFinderController.timeLimit = value
                            Layout.fillWidth: true
                        }
                    }
                }

                Label {
                    text: qsTr("Edit Mode")
                    font.bold: true
                }

                RowLayout {
                    Layout.fillWidth: true

                    Button {
                        text: qsTr("Wall")
                        checkable: true
                        checked: editMode === 0
                        onClicked: editMode = 0
                        Layout.fillWidth: true
                    }
                    Button {
                        text: qsTr("Start")
                        checkable: true
                        checked: editMode === 1
                        onClicked: editMode = 1
                        Layout.fillWidth: true
                    }
                    Button {
                        text: qsTr("End")
                        checkable: true
                        checked: editMode === 2
                        onClicked: editMode = 2
                        Layout.fillWidth: true
                    }
                }

                Label {
                    text: qsTr("Grid Size")
                    font.bold: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    SpinBox {
                        id: widthSpin
                        from: 5
                        to: 100
                        value: gridModel.width
                        onValueChanged: gridModel.width = value
                        Layout.fillWidth: true
                    }
                    Label { text: "x" }
                    SpinBox {
                        id: heightSpin
                        from: 5
                        to: 100
                        value: gridModel.height
                        onValueChanged: gridModel.height = value
                        Layout.fillWidth: true
                    }
                }

                Item { Layout.fillHeight: true }

                Label {
                    text: pathFinderController.statusMessage
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 5

                    Button {
                        text: qsTr("Find Path")
                        Layout.fillWidth: true
                        highlighted: true
                        enabled: !pathFinderController.isSearching
                        onClicked: pathFinderController.findPath()
                    }
                    Button {
                        text: qsTr("Clear Path")
                        Layout.fillWidth: true
                        onClicked: {
                            pathFinderController.clearPath()
                            pathPoints = []
                            pathCanvas.requestPaint()
                        }
                    }
                }

                Button {
                    text: qsTr("Clear All")
                    Layout.fillWidth: true
                    onClicked: {
                        gridModel.clearAll()
                        pathPoints = []
                        pathCanvas.requestPaint()
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: qsTr("Language:") }
                    Button {
                        text: "EN"
                        highlighted: languageManager.currentLanguage === "en"
                        onClicked: languageManager.switchLanguage("en")
                    }
                    Button {
                        text: "中文"
                        highlighted: languageManager.currentLanguage === "zh"
                        onClicked: languageManager.switchLanguage("zh")
                    }
                }
            }
        }
    }

    Connections {
        target: pathFinderController
        function onPathUpdated() {
            pathPoints = gridModel.getPathPoints()
            pathCanvas.requestPaint()
        }
    }
}
