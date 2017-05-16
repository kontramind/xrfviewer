import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("XRF Image Sequence Test")

    function remove_image_prefix(value) {
        return value.toString().substring("image://xrfimage/".length)
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
                property string curr_xrf_loop_url: remove_image_prefix(xrf_img.source)
                text:
                    "<table border='1' align='center'>" +
                       "<tr bgcolor='#9acd32'>" +
                       "<td width='50' min-width='50' align='center'>" + xrfCineLoopManager.loopCurrentFrameNo(curr_xrf_loop_url) + "</td>" +
                       "<td width='50' min-width='50' align='center'>" + xrfCineLoopManager.loopFrameCount(curr_xrf_loop_url) + "</td> </tr>"
                textFormat: Text.RichText
                anchors.centerIn: parent
                color: "red"
                font.bold: true
                font.pointSize: 12
                Keys.enabled: false
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
            text: xrfCineLoopManager.loopDcmTagValuesHtml
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
                Text { color: "white"; text: url; font.pointSize: 12 }
                Text { color: "white"; text: framecount; font.pointSize: 12 }
            }
        }

        ListView {
            id: xrfModelListView
            anchors.fill: parent
            model: xrfCineLoopListModel
            delegate: xrfSimpleDelegate
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
                xrf_img.current_image = 0
                xrfCineLoopManager.addLoopUrl(dlg_open.fileUrl)
                xrf_img.curr_url = dlg_open.fileUrl
            }
            onRejected: {
                main_timer.running = true
            }
        }
        Image {
            id: xrf_img
            anchors.fill: parent
            property string curr_url: ""
            property int current_image: 0
            fillMode: Image.PreserveAspectFit
            source: "image://xrfimage/" + curr_url + "#" + current_image
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
                xrf_img.current_image = xrf_img.current_image + 1 >= xrfCineLoopManager.loopFrameCount(xrf_img.curr_url) ? 0 : xrf_img.current_image + 1
                break;
            case Qt.Key_Left:
                main_timer.running = false
                xrf_img.current_image = xrf_img.current_image - 1 < 0 ? xrfCineLoopManager.loopFrameCount(xrf_img.curr_url) - 1 : xrf_img.current_image - 1
                break;
            case Qt.Key_I:
                rect_hdr.visible = !rect_hdr.visible
                rect_info.visible = !rect_info.visible
                break;
            case Qt.Key_M:
                rect_model.visible = !rect_model.visible
                rect_hdr.visible = false
                rect_info.visible = false
                if(!rect_model.visible)
                    rect_hdr.visible = true
                break;
            default:
                break;
            }
            event.accepted = true
        }

        Timer {
            id: main_timer
            interval: 1000.0/xrfCineLoopManager.frameDisplayRate
            repeat: true
            running: false
            onTriggered: {
                xrf_img.current_image = xrf_img.current_image + 1 >= xrfCineLoopManager.loopFrameCount(xrf_img.curr_url) ? 0 : xrf_img.current_image + 1
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
                xrf_img.current_image = 0
                dlg_open.open()
            }
        }
    }
}
