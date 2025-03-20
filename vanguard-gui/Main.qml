import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

ApplicationWindow {
    id: root
    minimumWidth: 240
    minimumHeight: 240
    maximumWidth: 240
    maximumHeight: 240
    visible: true
    title: "vanguard-out"
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint

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
            text: "Warning!\n\n" + "Vanguard state has been changed but not saved." + "\n\n\n\n\n\n\n\n\n\n" // fill height
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
                text: "<font size='+1'><center><b>" + "vanguard-out" + "</b></font></center>" + "<center>Version: 0.3.0</center>"
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
                onClicked: Qt.openUrlExternally("https://github.com/Simon-12/vanguard-out")
            }

            Button {
                text: "Report Issues"
                Layout.fillWidth: true
                onClicked: Qt.openUrlExternally("https://github.com/Simon-12/vanguard-out/issues")
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
