import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1


ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("XRF Viewer")

    FileDialog {
        id: fileDialog
        title: "Select an image"
        nameFilters: imageNameFilters
        selectFolder: false
        folder: picturesLocation
    }

    Image {
        id: xrfimg
        parent: ApplicationWindow.contentItem
        cache: false
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "image://xrfimage/" + fileDialog.fileUrl + "#" + frameFetchDirection
        focus: false
        Keys.enabled: false
        MouseArea {
            anchors.fill: parent
            onClicked: cmdbuttons.focus = true
        }
    }

    Rectangle {
        parent: ApplicationWindow.overlay
        height: fileButton.height
        width: ApplicationWindow.contentItem.width
        color: "lightgray"
        opacity: 0.5

        Text{
            id: headeretxt
            text:
                "<table border='1'>" +
                   "<tr bgcolor='#9acd32'>" +
                   "<td width='50' align='center'>" + root.frameCurrentNo + "</td>" +
                   "<td width='50' align='center'>" + root.frameTotalCount + "</td> </tr>"
            anchors.fill: parent
            color: "red"
            font.bold: true
            font.pointSize: 12
            Keys.enabled: false
            textFormat: Text.RichText
//            verticalAlignment: Text.AlignVCenter
//            horizontalAlignment: Text.AlignHCenter
        }
    }

    footer: RowLayout  {
        id: cmdbuttons
        width: parent.width
        Button {
            id: fileButton
            text: "File"
            onClicked: menu.open()
            width: parent.width
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Keys.enabled: false

            Menu {
                id: menu
                MenuItem {
                    text: "Open..."
                    onTriggered: {
                        fileDialog.open()
                        frameTimer.running = false;
                        root.frameCurrentNo = -1
                        root.frameTotalCount = -1
                        root.frameFetchDirection = "curr"
                        root.framePlayPauseAction = "play"
                        cmdbuttons.focus = true;
                    }
                }
            }
        }
        Button {
            id: prev
            Image {
                id: iconprev
                anchors.centerIn: parent
                source: "icons/movie-prev.svg"
                Keys.enabled: false;
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Keys.enabled: false
            onClicked: {
                cmdbuttons.focus = true
                root.frameFetchDirection = "curr"
                root.frameFetchDirection = "prev"
            }
        }
        Button {
            id: next
            Image {
                id: iconnext
                anchors.centerIn: parent
                source: "icons/movie-next.svg"
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                cmdbuttons.focus = true
                root.frameFetchDirection = "curr"
                root.frameFetchDirection = "next"
            }
        }
        Button {
            id: play
            Image {
                id: icon_play_pause
                anchors.centerIn: parent
                source: root.framePlayPauseAction === "play" ? "icons/movie-play.svg" : "icons/movie-pause.svg"
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Keys.enabled: false
            onClicked: {
                cmdbuttons.focus = true
                root.framePlayPauseAction = root.framePlayPauseAction === "play" ? "pause" : "play"
            }
        }
        Button {
            id: info
            Image {
                id: icon_info
                anchors.centerIn: parent
                source: "icons/info.svg"
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Keys.enabled: false
            onClicked: {
                cmdbuttons.focus = true
                console.log("info pressed ...")
            }
        }

        Keys.onSpacePressed: {
            root.framePlayPauseAction = root.framePlayPauseAction === "play" ? "pause" : "play"
            frameTimer.running = root.framePlayPauseAction === "play" ? false : true
            event.accpted = true;
        }
        Keys.onLeftPressed: {
            root.frameFetchDirection = "curr"
            root.frameFetchDirection = "prev"
            event.accpted = true;
        }
        Keys.onRightPressed: {
            root.frameFetchDirection = "curr"
            root.frameFetchDirection = "next"
            event.accpted = true;
        }
    }


    Timer {
        id: frameTimer
        running: false
        interval: 1000.0/frameRecommendedDisplayRate
        repeat: true

        onTriggered: {
            next.clicked()
        }
    }

    Component.onCompleted: {
        cmdbuttons.focus = true;
        picturesLocation = contextPicturesLocation;
        console.log("contextPicturesLocation : " + contextPicturesLocation);
    }

    property url selectImage : "";
    property string picturesLocation : "";
    property var imageNameFilters : ["*.dcm"];

    property int frameCurrentNo: -1
    property int frameTotalCount: -1
    property int frameRecommendedDisplayRate: 10
    property string frameDcmKeyTagValues : ""
    property string frameFetchDirection: "curr"
    property string framePlayPauseAction: "play"

//    toolBar: ToolBar {
//        RowLayout {
//            anchors.fill: parent
//            ToolButton {...}
//        }
//    }

//    TabView {
//        id: myContent
//        anchors.fill: parent
//        ...
//    }
}

