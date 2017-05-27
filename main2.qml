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

        // Define a delegate component. A component will be
        // instantiated for each visible item in the list.
        Component {
            id: xrfSimpleDelegate
            Item {
                id: wrapper; width: xrfGridView.cellWidth; height: xrfGridView.cellHeight;
                Column {
                    id: row_wrapper; anchors.fill: wrapper; spacing:5
                    Text { id:col_frmno; color: "white"; text: currentframeno+"/"+framecount; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter; font.pointSize: 12; anchors.horizontalCenter: parent.horizontalCenter; }
                    Rectangle { id:col_rect_img; anchors.horizontalCenter: parent.horizontalCenter; width:250; height:250; color: "transparent"; border.color: "transparent";
                        Image { id:xrfthumbnail; width:parent.width-8; height:parent.height-8; anchors.centerIn:col_rect_img; source: "image://xrfimage/" + currentframeimage;  }
                    }
                    Text { id:col_url; color: "white"; text: url; font.italic: true; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter; font.pointSize: 12; elide: Text.ElideMiddle; anchors.horizontalCenter: parent.horizontalCenter; width: parent.width}
                }
                states: State {
                    name: "Current"
                    when: wrapper.GridView.isCurrentItem
                    PropertyChanges { target:col_rect_img; border.color: "red"; border.width: 4 }
                    PropertyChanges { target:col_url; color: "black" }
                    PropertyChanges { target:col_frmno; color: "black" }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        wrapper.GridView.view.currentIndex = index
                        curr_loop_url = col_url.text
                        curr_frame_no = parseInt(col_frmno.text)
                        xrfCineLoopManager.addLoopUrl(col_url.text)
                        xrfCineLoopManager.setCurrentFrameNo(curr_loop_url, curr_frame_no)
                    }
                }
            }
        }
        Component {
            id: highlight
            Rectangle {
                width: xrfGridView.cellWidth; height: xrfGridView.cellHeight
                color: "yellow"; radius: 5
                // check if currentindex exist at all
                x: xrfGridView.currentIndex != -1 ? xrfGridView.currentItem.x : 0
                y: xrfGridView.currentIndex != -1 ? xrfGridView.currentItem.y : 0
            }
        }

        GridView {
            id: xrfGridView; anchors.fill: parent; cellWidth: 300; cellHeight: 300;
            model: xrfCineLoopListModel; delegate: xrfSimpleDelegate; highlight: highlight
            highlightFollowsCurrentItem: false
        }
    }

    Rectangle {
        id: main_rect
        focus: true
        anchors.fill: parent
        FileDialog {
            id: dlg_open
            selectFolder: false
            title: "Select loop"
            nameFilters: ["*.dcm"]
            onAccepted: {
                xrfCineLoopManager.addLoopUrl(dlg_open.fileUrl)
                curr_loop_url = dlg_open.fileUrl
                curr_frame_no = 0
                xrfCineLoopManager.setCurrentFrameNo(curr_loop_url, curr_frame_no)
                xrfGridView.currentIndex = xrfCineLoopManager.getModelIndex(curr_loop_url)
            }
            onRejected: {
                main_timer.running = true
            }
        }
        Image {
            id: xrf_img
            cache: true
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "image://xrfimage/" + curr_loop_url + "#" + curr_frame_no
        }
        Image {
            id: xrf_img_rcv
            cache: true
            width: 64*2
            height: 64*2
            visible: false
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            fillMode: Image.PreserveAspectFit
            source: "image://xrfimage/" + curr_loop_url + "#" + 0
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
                if(rect_model.visible) return
                main_timer.running = false
                nextimage()
                break;
            case Qt.Key_Left:
                if(rect_model.visible) return
                main_timer.running = false
                previmage()
                break;
            case Qt.Key_I:
                if(rect_model.visible) return
                rect_hdr.visible = !rect_hdr.visible
                rect_info.visible = !rect_info.visible
                break;
            case Qt.Key_M:
                main_timer.running = false;
                rect_model.visible = !rect_model.visible
                rect_hdr.visible = !rect_model.visible
                rect_info.visible = false
                if(rect_model.visible) {
                    xrf_img_rcv.visible = false
                    xrfGridView.currentIndex = xrfCineLoopManager.getModelIndex(curr_loop_url)
                } else if(xrfGridView.currentIndex != -1) {
                        curr_loop_url = xrfCineLoopListModel.get(xrfGridView.currentIndex).url
                        curr_frame_no = xrfCineLoopListModel.get(xrfGridView.currentIndex).currentframeno
                        xrfCineLoopManager.setCurrentFrameNo(curr_loop_url, curr_frame_no)
                } else {
                    curr_loop_url = "";
                    curr_frame_no = 0;
                }
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

    Connections {
        target: xrfCineLoopRcv
        onCineLoopReceived: {
            if(!xrfCineLoopManager.contains("file:///"+fullpath)) {
                xrfCineLoopManager.addLoopUrl("file:///"+fullpath)
                xrf_img_rcv.source = "image://xrfimage/" + "file:///"+fullpath + "#" + 0
                xrf_img_rcv.visible = true
            }
        }
    }
    Component.onCompleted: {
        xrfCineLoopRcv.init();
        xrfCineLoopRcv.start();
    }
    Component.onDestruction: {
        xrfCineLoopRcv.stop();
        xrfCineLoopRcv.wait(10*10000)
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
