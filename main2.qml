import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root
    visible: true
    width: 640*2
    height: 480*2
    title: qsTr("XRF Cine Loop Viewer")

    property int commonDisplayFrameRate: 10
    property int curr_frame_no: 0
    property string curr_loop_url: ""
    function nextimage() {
        if(xrfCineLoopManager.loopFrameCount(curr_loop_url) === 0)
             curr_frame_no = 0
        else if(curr_frame_no + 1 === xrfCineLoopManager.loopFrameCount(curr_loop_url))
            curr_frame_no = 0
        else
            ++curr_frame_no
        xrfCineLoopManager.setCurrentFrameNo(curr_loop_url, curr_frame_no)
    }
    function previmage() {
        if(xrfCineLoopManager.loopFrameCount(curr_loop_url) === 0)
            curr_frame_no = 0
        else if (curr_frame_no - 1 < 0)
            curr_frame_no = xrfCineLoopManager.loopFrameCount(curr_loop_url) - 1
        else
            --curr_frame_no;
        xrfCineLoopManager.setCurrentFrameNo(curr_loop_url, curr_frame_no)
    }

    Rectangle {
        id: rect_hdr
        visible: true
        parent: ApplicationWindow.overlay
        anchors.top: parent.top
        anchors.left: parent.left
        width: ApplicationWindow.contentItem.width
        height: 30
        color: "lightgray"
        opacity: 0.5

        Text{
            id: txt_header
            text: ""
            textFormat: Text.RichText
            anchors.centerIn: parent
            color: "red"
            font.bold: true
            font.pointSize: 12
            Keys.enabled: false

            Connections {
                target: xrfCineLoopManager
                onDataChanged: {
                    txt_header.text =
                            "<table border='1' align='center'>" +
                            "<tr bgcolor='#9acd32'>" +
                            "<td width='50' min-width='50' align='center'>" + xrfCineLoopManager.loopCurrentFrameNo(curr_loop_url) + "</td>" +
                            "<td width='50' min-width='50' align='center'>" + xrfCineLoopManager.loopFrameCount(curr_loop_url) + "</td> </tr>"
                }
            }
        }
    }

    Rectangle {
        id: rect_info
        visible: false
        parent: ApplicationWindow.overlay
        anchors.top: parent.top
        anchors.left: parent.left
        width: ApplicationWindow.overlay.width
        height: ApplicationWindow.contentItem.height
        color: "lightgrey"
        opacity: 0.5
        property bool toggle: false

        Text {
            id: txt_info
            text: xrfCineLoopManager.loopDcmTagValuesHtml(curr_loop_url)
            textFormat: Text.RichText
            anchors.centerIn: parent
            color: "red"
            font.bold: true
            font.pointSize: 12
            Keys.enabled: false
        }
    }

    Rectangle {
        id: rect_model
        visible: false
        parent: ApplicationWindow.overlay
        anchors.top: parent.top
        anchors.left: parent.left
        width: ApplicationWindow.overlay.width
        height: ApplicationWindow.contentItem.height
        color: "blue"

        Component {
            id: xrfSimpleDelegate
            Row {
                spacing: 10
                Image {
                    id:xrfthumbnail; width: 256; height: 256; fillMode: Image.PreserveAspectFit; source: "image://xrfimage/" + currentframeimage
                }
                Text { color: "white"; text: url; font.italic: true; font.pointSize: 12; elide: Text.ElideMiddle; anchors.verticalCenter: xrfthumbnail.verticalCenter }
                Text { color: "white"; text: currentframeno; style: Text.Outline; font.pointSize: 12; anchors.verticalCenter: xrfthumbnail.verticalCenter }
                Text { color: "white"; text: "/"+framecount; font.pointSize: 12; anchors.verticalCenter: xrfthumbnail.verticalCenter }
            }
        }

        ListView {
            id: xrfModelListView
            anchors.fill: parent
            model: xrfCineLoopListModel
            delegate: xrfSimpleDelegate
            spacing: 10
            orientation: ListView.Vertical
            verticalLayoutDirection: ListView.TopToBottom
        }
    }

    Rectangle {
        id: main_rect
        focus: true
        anchors.fill: parent
        FileDialog {
            id: dlg_open
            selectFolder: false
            title: "Select an image"
            nameFilters: ["*.dcm"]
            onAccepted: {
                xrfCineLoopManager.addLoopUrl(dlg_open.fileUrl)
                curr_loop_url = dlg_open.fileUrl
                curr_frame_no = 0
                xrfCineLoopManager.setCurrentFrameNo(curr_loop_url, curr_frame_no)
            }
            onRejected: {
                main_timer.running = true
            }
        }
        Image {
            id: xrf_img
            cache: false
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "image://xrfimage/" + curr_loop_url + "#" + curr_frame_no
        }

        Keys.onSpacePressed: {
            main_timer.running = !main_timer.running
            event.accepted = true
        }
        Keys.onPressed: {
            switch(event.key) {
            case Qt.Key_O:
                main_timer.running = false
                dlg_open.open()
                break;
            case Qt.Key_Right:
                main_timer.running = false
                nextimage()
                break;
            case Qt.Key_Left:
                main_timer.running = false
                previmage()
                break;
            case Qt.Key_I:
                rect_hdr.visible = !rect_hdr.visible
                rect_info.visible = !rect_info.visible
                break;
            case Qt.Key_M:
                main_timer.running = false;
                rect_model.visible = !rect_model.visible
                rect_hdr.visible = !rect_model.visible
                rect_info.visible = false
                break;
            default:
                break;
            }
            event.accepted = true
        }

        Timer {
            id: main_timer
            interval: 1000.0/xrfCineLoopManager.loopFrameDisplayRate(curr_loop_url)
            repeat: true
            running: false
            onTriggered: {
                if(!rect_model.visible) {
                    main_timer.interval = 1000.0/xrfCineLoopManager.loopFrameDisplayRate(curr_loop_url)
                    nextimage()
                } else {
                    main_timer.interval = 1000.0/root.commonDisplayFrameRate
                    xrfCineLoopManager.incrementCurrentFrameNoForAllLoops()
                }
            }
        }
    }

    footer: Button {
        id: bt_open
        text: "Open"
        MouseArea {
            anchors.fill : parent
            onClicked: {
                main_timer.running = false
                dlg_open.open()
            }
        }
    }
}
