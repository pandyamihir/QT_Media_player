import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 800
    height: 400
    visible: true
    title: qsTr("Media Player")
    property int flag: 0

    onActiveFocusItemChanged: {
        if (activeFocusItem !== keyboardHandler) {
            keyboardHandler.forceActiveFocus()
        }
    }

    onActiveChanged: {
        if (active) {
            keyboardHandler.forceActiveFocus()
        }
    }

    // Keyboard event handler
    Item {
        id: keyboardHandler
        anchors.fill: parent
        focus: true

        Keys.onUpPressed: {
            if (playlistView.currentIndex > 0) {
                playlistView.currentIndex--
                //playlistmodel.setCurrentIndex(playlistView.currentIndex)
            }
            event.accepted = true
        }

        Keys.onDownPressed: {
            if (playlistView.currentIndex < playlistmodel.songCount() - 1) {
                playlistView.currentIndex++
                //playlistmodel.setCurrentIndex(playlistView.currentIndex)
            }
            event.accepted = true
        }

        Keys.onReturnPressed: {
            playlistmodel.setCurrentIndex(playlistView.currentIndex)
            mediaController.play()
            flag = 1
            event.accepted = true
        }

        Keys.onSpacePressed: {
            if (flag == 0) {
                mediaController.play()
                flag = 1
            } else {
                mediaController.pause()
                flag = 0
            }
            event.accepted = true
        }
    }

    // Background Image
    Image {
        anchors.fill: parent
        source: "qrc:/Icons/Bg_Image_1.jpg"
        fillMode: Image.PreserveAspectCrop
        z: -1
    }

    // Main container with proper spacing
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Left sidebar - Playlist
        Rectangle {
            Layout.preferredWidth: parent.width * 0.25
            Layout.fillHeight: true
            color: "#00000000"  // Semi-transparent black

            ListView {
                id: playlistView
                anchors.fill: parent
                anchors.margins: 10
                model: playlistmodel
                clip: true
                focus: true
                spacing: 5
                currentIndex: playlistmodel.getCurrentIndex()
                highlightFollowsCurrentItem: true

                Connections {
                        target: playlistmanager
                        function onCurrentSongIndexChanged(index) {
                            playlistView.currentIndex = index
                            playlistView.positionViewAtIndex(index, ListView.Contain)
                        }
                    }

                onCurrentIndexChanged: {
                    positionViewAtIndex(currentIndex, ListView.Contain)
                }

                highlightMoveDuration: 200
                keyNavigationEnabled: true
                keyNavigationWraps: true

                delegate: Rectangle {
                width: parent.width
                height: 40
                radius: 4
                color: ListView.isCurrentItem ? "#c0000000" : "#40000000"
                border.color: ListView.isCurrentItem ? "#4169E1" : "transparent"
                border.width: 2

                Text {
                    text: fileName
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    color: ListView.isCurrentItem ? "white" : "#82C8E5"
                    elide: Text.ElideRight
                    width: parent.width - 20
                    font.pixelSize: 18
                    font.family: "Arial"
                    // Bold for current item
                    font.bold: ListView.isCurrentItem
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        playlistView.currentIndex = index
                        playlistmodel.setCurrentIndex(index)
                        mediaController.play()
                        flag = 1
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            highlight: Rectangle {
                color: "#60ffffff"
                radius: 4
            }
            }
        }

        // Right side - Player controls
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // Album art and metadata
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.6
                color: "#40000000"  // Semi-transparent black

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 20

                    // Album art
                    Rectangle {
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 200
                        color: "#40000000"  // Darker semi-transparent background
                        radius: 5

                        Image {
                            id: albumImage
                            anchors.fill: parent
                            anchors.margins: 5
                            source: metadataManager.albumArt
                            fillMode: Image.PreserveAspectFit
                            sourceSize: Qt.size(200, 200)
                        }
                    }

                    // Song info
                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 10

                        Label {
                            text: metadataManager.songTitle !== "" ? metadataManager.songTitle : "Unknown Title"
                            font.bold: true
                            font.pixelSize: 24
                            color: "white"
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        Label {
                            text: metadataManager.songArtist !== "" ? metadataManager.songArtist : "Unknown Artist"
                            font.pixelSize: 18
                            color: "white"
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        Label {
                            text: metadataManager.songAlbum !== "" ? metadataManager.songAlbum : "Unknown Album"
                            font.pixelSize: 16
                            color: "#aaa"
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                    }
                }
            }

            // Progress bar
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                color: "#40000000"  // Slightly darker background

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 5

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Text {
                            id: curPos
                            text: Qt.formatTime(new Date(0,0,0,0, mediaController.currentPos/1000/60, mediaController.currentPos/1000%60), "mm:ss")
                            color: "white"
                            font.pixelSize: 14
                            Layout.preferredWidth: 50
                        }

                        Slider {
                            Layout.fillWidth: true
                            from: 0
                            to: mediaController.totalDuration
                            value: mediaController.currentPos
                            onMoved: mediaController.setCurrentPos(value)

                            background: Rectangle {
                                implicitHeight: 6
                                color: "#555"
                                radius: 3

                                Rectangle {
                                    width: parent.width * (mediaController.currentPos / mediaController.totalDuration)
                                    height: parent.height
                                    color: "#4169E1"
                                    radius: 3
                                }
                            }

                            handle: Rectangle {
                                x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                                implicitWidth: 16
                                implicitHeight: 16
                                radius: 8
                                color: parent.pressed ? "#0000FF" : "white"
                                border.color: parent.pressed ? "white" : "#0000FF"
                                border.width: 2
                            }
                        }

                        Text {
                            id: totPos
                            text: Qt.formatTime(new Date(0,0,0,0, mediaController.totalDuration/1000/60, mediaController.totalDuration/1000%60), "mm:ss")
                            color: "white"
                            font.pixelSize: 14
                            Layout.preferredWidth: 50
                        }
                    }
                }
            }

            // Control buttons
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 80  // Fixed height as requested
                color: "#c0000000"

                RowLayout {
                    anchors.fill: parent
                    spacing: {
                        // Responsive spacing based on width but with limits
                        var baseSpacing = parent.width * 0.02
                        return Math.max(5, Math.min(20, baseSpacing))
                    }

                    // Left side controls
                    RowLayout {
                        spacing: parent.spacing
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                        Button {
                            id: muteButton
                            icon.source: mediaController.muted ? "qrc:/Icons/Mute.png" : "qrc:/Icons/Unmute.png"
                            icon.color: "transparent"
                            icon.width: 40
                            icon.height: 40
                            onClicked: mediaController.toggleMute()
                            background: Rectangle { color: "transparent" }
                        }

                        Button {
                            id: shuffleButton
                            icon.source: "qrc:/Icons/Shuffle.png"
                            icon.color: playlistmanager.shuffleMode ? "blue" : "transparent"
                            icon.width: 40
                            icon.height: 40
                            background: Rectangle {
                                color: playlistmanager.shuffleMode ? "#ABC9E6" : "transparent"
                                radius: 4
                            }
                            onClicked: playlistmanager.toggleShuffle()
                        }
                    }

                    // Center controls
                    RowLayout {
                        spacing: parent.spacing
                        Layout.alignment: Qt.AlignCenter

                        Button {
                            id: prevButton
                            icon.source: "qrc:/Icons/Prev.png"
                            icon.color: "transparent"
                            icon.width: 40
                            icon.height: 40
                            onClicked: {
                                        playlistmanager.previousSong();
                                        flag = 1;  // Reset to show Play icon for new track
                                    }
                            // onClicked: playlistmanager.previousSong()
                            background: Rectangle { color: "transparent" }
                        }

                        Button {
                            id: stopButton
                            icon.source: "qrc:/Icons/Stop.png"
                            icon.color: "transparent"
                            icon.width: 40
                            icon.height: 40
                            onClicked: {
                                mediaController.stop()
                                flag = 0
                            }
                            background: Rectangle { color: "transparent" }
                        }

                        Button {
                            id: playPauseButton
                            icon.source: flag == 1 ? "qrc:/Icons/Pause.png" : "qrc:/Icons/Play.png"
                            icon.color: "transparent"
                            icon.width: 60  // Slightly larger than other buttons
                            icon.height: 60
                            onClicked: {
                                if (flag == 0) {
                                    mediaController.play()
                                    flag = 1
                                }
                                else {
                                    mediaController.pause()
                                    flag = 0
                                }
                            }
                            background: Rectangle {
                                color: "transparent"
                                radius: width / 2
                            }
                        }

                        Button {
                            id: nextButton
                            icon.source: "qrc:/Icons/Next.png"
                            icon.color: "transparent"
                            icon.width: 40
                            icon.height: 40
                            onClicked: {
                                        playlistmanager.nextSong();
                                        flag = 1;  // Reset to show Play icon for new track
                                    }
                            // onClicked: playlistmanager.nextSong()
                            background: Rectangle { color: "transparent" }
                        }
                    }

                    // Right side controls
                    RowLayout {
                        spacing: parent.spacing
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        Button {
                            id: volDownButton
                            icon.source: "qrc:/Icons/Vol_down.png"
                            icon.color: "transparent"
                            icon.width: 40
                            icon.height: 40
                            onClicked: mediaController.volumeDown()
                            background: Rectangle { color: "transparent" }
                        }

                        Slider {
                            id: volumeSlider
                            Layout.preferredWidth: {
                                // Responsive width with limits
                                var baseWidth = parent.width * 0.09
                                return Math.max(80, Math.min(150, baseWidth))
                            }
                            from: 0
                            to: 100
                            value: mediaController.volume
                            onMoved: mediaController.setVolume(value)

                            background: Rectangle {
                                implicitHeight: 4
                                color: "#555"
                                radius: 2
                                Rectangle {
                                    width: parent.width * (volumeSlider.value / 100)
                                    height: parent.height
                                    color: "#4169E1"
                                    radius: 2
                                }
                            }

                            handle: Rectangle {
                                x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                                y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                                implicitWidth: 12
                                implicitHeight: 12
                                radius: 6
                                color: volumeSlider.pressed ? "#0000FF" : "white"
                                border.color: volumeSlider.pressed ? "white" : "#0000FF"
                                border.width: 2
                            }
                        }

                        Button {
                            id: volUpButton
                            icon.source: "qrc:/Icons/Vol_up.png"
                            icon.color: "transparent"
                            icon.width: 40
                            icon.height: 40
                            onClicked: mediaController.volumeUp()
                            background: Rectangle { color: "transparent" }
                        }
                    }
                }
            }
        }
    }
}
