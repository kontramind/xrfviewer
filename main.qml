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
        id: hdrrect
        parent: ApplicationWindow.overlay
        height: fileButton.height
        width: ApplicationWindow.contentItem.width
        color: "lightgray"
        opacity: 0.5

        Text{
            id: headeretxt
            text:
                "<table border='1' align='center'>" +
                   "<tr bgcolor='#9acd32'>" +
                   "<td width='50' min-width='50' align='center'>" + root.frameCurrentNo + "</td>" +
                   "<td width='50' min-width='50' align='center'>" + root.frameTotalCount + "</td> </tr>"
            anchors.fill: parent
            color: "red"
            font.bold: true
            font.pointSize: 12
            Keys.enabled: false
            textFormat: Text.RichText
        }
        onWidthChanged: {
            root.frameCurrentNo = root.frameCurrentNo+1
            root.frameCurrentNo = root.frameCurrentNo-1
        }
    }

    footer: RowLayout  {
        id: cmdbuttons
        width: parent.width
        Button {
            id: fileButton
            text: "Open"
            width: parent.width
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Keys.enabled: false
            onClicked: {
                fileDialog.open()
                frameTimer.running = false;
                root.frameCurrentNo = -1
                root.frameTotalCount = -1
                root.frameFetchDirection = "curr"
                root.framePlayPauseAction = "play"
                cmdbuttons.focus = true;
            }
        }
        Button {
            id: prev
            visible: false
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
            visible: false
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
            visible: false
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
            visible: false
            property bool toggle: true
            Image {
                id: icon_info
                anchors.centerIn: parent
                source: "icons/info.svg"
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Keys.enabled: false

            Rectangle {
                id: inforect
                visible: false
                parent: ApplicationWindow.overlay
                width: ApplicationWindow.overlay.width
                height: ApplicationWindow.contentItem.height
                color: "lightgrey"
                opacity: 0.5
                property bool toggle: false

                Text {
                    id: infotxt
                    text: root.frameDcmKeyTagValues
                    anchors.fill: parent
                    color: "red"
                    font.bold: true
                    font.pointSize: 12
                    Keys.enabled: false
                    textFormat: Text.RichText
                }
                onWidthChanged: {
                    var tmp = root.frameDcmKeyTagValues
                    root.frameDcmKeyTagValues = ""
                    root.frameDcmKeyTagValues = tmp
                }
                onHeightChanged: {
                    var tmp = root.frameDcmKeyTagValues
                    root.frameDcmKeyTagValues = ""
                    root.frameDcmKeyTagValues = tmp
                }
            }

            onClicked: {
                cmdbuttons.focus = true
                if(toggle) {
                    hdrrect.visible = false
                    inforect.visible = true
                    toggle = false
                } else {
                    hdrrect.visible = true
                    inforect.visible = false
                    toggle = true;
                }
            }
        }

        Keys.onSpacePressed: {
            frameTimer.running = !frameTimer.running;
            event.accepted = true;
        }
        Keys.onLeftPressed: { frameTimer.running = false; prev.clicked(); event.accpted = true; }
        Keys.onRightPressed: { frameTimer.running = false; next.clicked(); event.accpted = true; }
        Keys.onPressed: { if (event.key === Qt.Key_I) info.clicked(); event.accepted = true;}
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
        root.frameCurrentNo = 0;
        root.frameTotalCount = 0;
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

