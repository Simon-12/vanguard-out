import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

ApplicationWindow {
    id: root
    minimumWidth: 240
    minimumHeight: 270
    maximumWidth: 240
    maximumHeight: 270
    visible: true
    title: "vanguard-out"
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint | Qt.WindowSystemMenuHint
           | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        enabled: !controller.disabled
        Layout.alignment: Qt.AlignTop

        RowLayout {
            spacing: 30
            Image {
                source: "qrc:/icon.png"
                Layout.preferredWidth: 50
                Layout.preferredHeight: 50
            }

            Switch {
                id: switchState
                checked: controller.state
                text: checked ? "Active" : "Inactive"
                font.pointSize: 14
                Layout.alignment: Qt.AlignVCenter
            }
        }

        // Fill space
        Item {
            Layout.fillHeight: true
        }

        Button {
            text: "Save Changes"
            font.pointSize: 14
            enabled: controller.state !== switchState.checked
            Layout.fillWidth: true
            onClicked: controller.changeState(switchState.checked)
        }

        Button {
            id: restartButton
            text: "Restart System"
            font.pointSize: 14
            enabled: controller.restart
            Layout.fillWidth: true
            onClicked: dialogRestart.open()
        }

        Button {
            text: "Add to Autostart"
            font.pointSize: 14
            Layout.fillWidth: true
            onClicked: controller.addAutostart()
        }

        Button {
            text: "Infos"
            font.pointSize: 14
            Layout.fillWidth: true
            onClicked: dialogInfo.open()
        }

        MessageDialog {
            id: dialogSave
            text: "Warning!\n\n" + "Vanguard state has been changed but not saved."
                  + "\n\n\n\n\n\n\n\n\n\n" // fill height
            buttons: MessageDialog.Ok | MessageDialog.Ignore
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
            text: "Restart System Now?" + "\n\n\n\n\n\n\n\n\n\n" // fill height
            buttons: MessageDialog.Ok | MessageDialog.Cancel
            onButtonClicked: function (button, role) {
                switch (button) {
                case MessageDialog.Ok:
                    controller.restartSystem()
                    break
                }
            }
        }
    }

    Popup {
        id: dialogInfo
        width: parent.width - 50
        height: parent.height - 50
        anchors.centerIn: parent
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout {
            anchors.fill: parent

            Label {
                text: "<font size='+1'><center><b>" + "vanguard-out"
                      + "</b></font></center>" + "<center>Version: 0.1.0</center>"
                      + "<br>" + "Tool to temporarily stop and disable Riot Vanguard"
                font.pointSize: 10
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignLeft
                Layout.preferredWidth: dialogInfo.width - 30
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            // Fill space
            Item {
                Layout.fillHeight: true
            }

            Button {
                text: "Website"
                Layout.fillWidth: true
                onClicked: Qt.openUrlExternally(
                               "https://github.com/Simon-12/tidy-images") // TODO link
            }

            Button {
                text: "Report Issues"
                Layout.fillWidth: true
                onClicked: Qt.openUrlExternally(
                               "https://github.com/Simon-12/tidy-images") // TODO link
            }
        }
    }

    // Close window
    property bool forceClose: false
    onClosing: closeEvent => {
                   if (forceClose)
                   return

                   // Check save
                   if (controller.state !== switchState.checked) {
                       dialogSave.open()
                       closeEvent.accepted = false
                   }
               }
}
