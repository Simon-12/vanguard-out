import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: root

    property bool forceClose: false

    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowCloseButtonHint
    maximumHeight: 270
    maximumWidth: 240
    minimumHeight: 270
    minimumWidth: 240
    title: "vanguard-out"
    visible: true

    onClosing: closeEvent => {
        if (forceClose)
            return

        // Check save
        if (_controller.state !== switchState.checked) {
            dialogSave.open()
            closeEvent.accepted = false
        }
    }

    // Main controls
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        enabled: !_controller.disabled
        spacing: 10

        // Icon and state toggle
        RowLayout {
            spacing: 30

            Image {
                Layout.preferredHeight: 50
                Layout.preferredWidth: 50
                source: "qrc:/icon.png"
            }

            Switch {
                id: switchState

                Layout.alignment: Qt.AlignVCenter
                checked: _controller.state
                font.pointSize: 14
                text: checked ? "Active" : "Inactive"
            }
        }

        // Fill space
        Item {
            Layout.fillHeight: true
        }

        Button {
            Layout.fillWidth: true
            enabled: _controller.state !== switchState.checked
            font.pointSize: 14
            text: "Save Changes"

            onClicked: _controller.changeState(switchState.checked)
        }

        Button {
            Layout.fillWidth: true
            enabled: _controller.restart
            font.pointSize: 14
            text: "Restart System"

            onClicked: dialogRestart.open()
        }

        Button {
            Layout.fillWidth: true
            font.pointSize: 14
            text: "Add to Autostart"

            onClicked: _controller.addAutostart()
        }

        Button {
            Layout.fillWidth: true
            font.pointSize: 14
            text: "Infos"

            onClicked: dialogInfo.open()
        }

        MessageDialog {
            id: dialogSave

            buttons: MessageDialog.Ok | MessageDialog.Ignore
            popupType: Popup.Item
            text: "Warning!\n\n" + "Vanguard state has been changed but not saved."
                  + "\n\n\n\n\n\n\n\n\n\n" // fill height

            onButtonClicked: function (button, role) {
                switch (button) {
                case MessageDialog.Ignore:
                    root.forceClose = true
                    root.close()
                    break
                }
            }
        }

        MessageDialog {
            id: dialogRestart

            buttons: MessageDialog.Ok | MessageDialog.Cancel
            popupType: Popup.Item
            text: "Restart System Now?" + "\n\n\n\n\n\n\n\n\n\n" // fill height

            onButtonClicked: function (button, role) {
                switch (button) {
                case MessageDialog.Ok:
                    _controller.restartSystem()
                    break
                }
            }
        }
    }

    Popup {
        id: dialogInfo

        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        height: parent.height - 50
        modal: true
        width: parent.width - 50

        // Info dialog content
        ColumnLayout {
            anchors.fill: parent

            Label {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.preferredWidth: dialogInfo.width - 30
                font.pointSize: 10
                horizontalAlignment: Text.AlignLeft
                text: "<font size='+1'><center><b>" + "vanguard-out" + "</b></font></center>"
                      + "<center>Version: 0.4.0</center>" + "<br>"
                      + "Tool to temporarily stop and disable Riot Vanguard"
                textFormat: Text.RichText
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
            }

            // Fill space
            Item {
                Layout.fillHeight: true
            }

            Button {
                Layout.fillWidth: true
                text: "Website"

                onClicked: Qt.openUrlExternally("https://github.com/Simon-12/vanguard-out")
            }

            Button {
                Layout.fillWidth: true
                text: "Report Issues"

                onClicked: Qt.openUrlExternally("https://github.com/Simon-12/vanguard-out/issues")
            }
        }
    }
}
