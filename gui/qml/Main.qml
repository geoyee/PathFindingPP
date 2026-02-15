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
    property int editMode: 0 // 0: wall, 1: start, 2: end

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
                    width: gridView.cellWidth
                    height: gridView.cellHeight
                    color: {
                        switch (cellType) {
                        case 0: return "#ffffff"
                        case 1: return "#333333"
                        case 2: return "#4CAF50"
                        case 3: return "#F44336"
                        case 4: return "#2196F3"
                        case 5: return "#BBDEFB"
                        default: return "#ffffff"
                        }
                    }
                    border.color: "#e0e0e0"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        property bool pressed: false

                        onPressed: function(mouse) {
                            pressed = true
                            handleMouse(mouse)
                        }
                        onReleased: pressed = false
                        onPositionChanged: function(mouse) {
                            if (pressed) handleMouse(mouse)
                        }

                        function handleMouse(mouse) {
                            if (pathFinderController.isSearching) return
                            if (editMode === 0) {
                                if (mouse.buttons & Qt.LeftButton) {
                                    if (cellType === 0) gridModel.setWall(cellX, cellY, true)
                                    else if (cellType === 1) gridModel.setWall(cellX, cellY, false)
                                }
                            } else if (editMode === 1) {
                                if (cellType !== 3) gridModel.setStart(cellX, cellY)
                            } else if (editMode === 2) {
                                if (cellType !== 2) gridModel.setEnd(cellX, cellY)
                            }
                        }
                    }
                }
            }
        }

        Pane {
            Layout.preferredWidth: 280
            Layout.fillHeight: true
            padding: 15

            ColumnLayout {
                anchors.fill: parent
                spacing: 15

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
                    text: qsTr("Diagonal Movement")
                    font.bold: true
                }

                ComboBox {
                    id: diagonalCombo
                    Layout.fillWidth: true
                    model: pathFinderController.diagonalOptions
                    currentIndex: pathFinderController.diagonalMovement
                    onCurrentIndexChanged: pathFinderController.diagonalMovement = currentIndex
                }

                Label {
                    text: qsTr("Weight: %1").arg(weightSlider.value.toFixed(1))
                    font.bold: true
                }

                Slider {
                    id: weightSlider
                    Layout.fillWidth: true
                    from: 0.1
                    to: 5.0
                    value: pathFinderController.weight
                    stepSize: 0.1
                    onValueChanged: pathFinderController.weight = value
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
                        onClicked: pathFinderController.clearPath()
                    }
                }

                Button {
                    text: qsTr("Clear All")
                    Layout.fillWidth: true
                    onClicked: gridModel.clearAll()
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
}
