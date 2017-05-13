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

    SwipeView {
        id: swipeView
        currentIndex: 0
        anchors.fill: parent

        Page {
            id: pg_xrfimage
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
                    text:
                        "<table border='1' align='center'>" +
                           "<tr bgcolor='#9acd32'>" +
                           "<td width='50' min-width='50' align='center'>" + xrf_img.current_image + "</td>" +
                           "<td width='50' min-width='50' align='center'>" + xrfCineLoopManager.frameCount + "</td> </tr>"
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
                property int current_image: 0
                fillMode: Image.PreserveAspectFit
                property string curr_url: ""
                source: "image://xrfimage/" + curr_url + "#" + current_image
                //source: "image://xrfimage/" + xrfCineLoopManager.loopUrl + "#" + current_image
            }

            Keys.onSpacePressed: {
                main_timer.running = !main_timer.running
                event.accepted = true
                console.log("space pressed ...")
            }
            Keys.onPressed: {
                switch(event.key) {
                case Qt.Key_O:
                    main_timer.running = false
                    dlg_open.open()
                    break;
                case Qt.Key_Right:
                    main_timer.running = false
                    xrf_img.current_image = xrf_img.current_image + 1 >= xrfCineLoopManager.frameCount ? 0 : xrf_img.current_image + 1
                    break;
                case Qt.Key_Left:
                    main_timer.running = false
                    if(xrfCineLoopManager.frameCount > 0)
                        xrf_img.current_image = xrf_img.current_image - 1 < 0 ? xrfCineLoopManager.frameCount - 1 : xrf_img.current_image - 1
                    break;
                case Qt.Key_I:
                    rect_hdr.visible = !rect_hdr.visible
                    rect_info.visible = !rect_info.visible
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
                    xrf_img.current_image = xrf_img.current_image + 1 >= xrfCineLoopManager.frameCount ? 0 : xrf_img.current_image + 1
                }
            }
        }
        }

        Page {
            id: pg_xrfmodel
            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }


    PageIndicator {
        id: indicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
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
