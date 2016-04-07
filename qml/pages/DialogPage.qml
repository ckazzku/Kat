/*
  Copyright (C) 2015 Petr Vytovtov
  Contact: Petr Vytovtov <osanwe@protonmail.ch>
  All rights reserved.

  This file is part of Kat.

  Kat is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kat is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kat.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import Vreen.Base 2.0

import "../views"

Page {
    id: dialogPage

    property int chatId
    property string title

    property int count: 5
    property variant participants: chatModel.participants
    property QtObject interlocutor: participants && participants.length === 1 ? participants[0] : null
    property bool isOnline: interlocutor && interlocutor.online
    property bool isChat: chatId > 0
    property string lastSeenTime

    property Item contextMenu
    property string attachmentsList: ""

    onChatIdChanged: {
        chatModel.setChatId(chatId)
        if (isChat) {
            chatModel.participantsChanged.connect(function(data) {
                if (pageContainer)
                    pageContainer.pushAttached(Qt.resolvedUrl("../pages/ChatUsersPage.qml"),
                                               { "title": dialogPage.title,
                                                 "users": dialogPage.participants })
            })
        }

        getHistory()
        scrollMessagesToBottom()
    }

    function getHistory(prev) {
        prev = prev === true
        var offset = chatModel.count

        loadingIndicator.running = true
        var reply = chatModel.getHistory(count, prev ? -offset : offset)
        if (reply) {
            reply.resultReady.connect(function(data) {
                loadingIndicator.running = false
            });
        }
    }

    function markDialogAsRead() {
        chatModel.markAsRead()
    }

    function scrollMessagesToBottom() {
        messages.positionViewAtEnd()
    }

    function scrollMessagesToTop() {
        messages.positionViewAtIndex(count - 2, ListView.Beginning)
    }

    function sendMessage() {
        chatModel.sendMessage(messageInput.text)

        messageInput.text = ""
        attachmentsList = ""

        markDialogAsRead()
        scrollMessagesToBottom()
    }

    BusyIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: false
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: parent.height

        Label {
            id: dialogTitle
            anchors.top: parent.top
            anchors.right: parent.right
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.highlightColor
            height: Theme.fontSizeLarge + 3 * Theme.paddingLarge
            width: parent.width - Theme.paddingLarge
            verticalAlignment: Text.AlignVCenter
            text: title
            elide: Text.ElideRight
        }

        Switch {
            id: dialogOnlineStatus
            anchors.verticalCenter: dialogTitle.verticalCenter
            anchors.right: dialogTitle.left
            anchors.rightMargin: Theme.paddingMedium
            automaticCheck: false
            height: Theme.fontSizeLarge
            width: Theme.fontSizeLarge
            checked: isOnline
        }

        SilicaListView {
            id: messages
            anchors.fill: parent
            anchors.topMargin: dialogTitle.height
            anchors.bottomMargin: messageInput.height
            clip: true
            model: chatModel

            footer: Label {
                width: parent.width
                height: (isChat || isOnline) ? 0 : Theme.itemSizeSmall
                visible: !(isChat || isOnline)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("Был(a) в сети: %1").arg(lastSeenTime)
            }

            delegate: Item {
                id: messageItem

                property bool menuOpen: contextMenu != null && contextMenu.parent === messageItem

                height: menuOpen ? contextMenu.height + contentItem.height : contentItem.height
                width: parent.width

                MessageItem {
                    id: contentItem
                    width: parent.width

//                    onClicked: {
//                        dialogPage.pageContainer.push(Qt.resolvedUrl("MessagePage.qml"),
//                                                      { "fullname": dialogTitle.text,
//                                                        "isOnline": dialogOnlineStatus.isOnline,
//                                                        "messageText": message,
//                                                        "attachments": attachments })
//                    }
                    onPressAndHold: {
                        console.log(index)
                        if (!contextMenu)
                            contextMenu = contextMenuComponent.createObject(messages)
                        contextMenu.show(messageItem)
                    }
                }

                Component {
                    id: contextMenuComponent

                    ContextMenu {

                        property string message

                        MenuItem {
                            text: qsTr("Копировать текст")
                            onClicked: Clipboard.text = chatModel.getMessageBody(index)
                        }

                        onClosed: contextMenu = null
                    }
                }
            }

            VerticalScrollDecorator {}
        }

        IconButton {
            id: attachmentsButton
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingLarge
            anchors.verticalCenter: messageInput.verticalCenter
            width: Theme.iconSizeSmallPlus
            height: Theme.iconSizeSmallPlus
            icon.width: Theme.iconSizeSmallPlus
            icon.height: Theme.iconSizeSmallPlus
            icon.fillMode: Image.PreserveAspectFit
            icon.source: "image://theme/icon-m-attach"
        }

        Label {
            id: attachmentsCounter
            anchors.verticalCenter: attachmentsButton.top
            anchors.left: attachmentsButton.left
            anchors.leftMargin: text === "10" ? 0 : Theme.paddingSmall
            anchors.verticalCenterOffset: Theme.paddingSmall
            font.bold: true
            font.pixelSize: Theme.fontSizeTiny
            color: Theme.highlightColor
            text: {
                var attachmentsCount = attachmentsList.split(',').length - 1
                return attachmentsCount > 0 ? attachmentsCount : ""
            }
        }

        TextArea {
            id: messageInput
            anchors.bottom: parent.bottom
            anchors.left: attachmentsButton.right
            anchors.right: parent.right
            placeholderText: qsTr("Сообщение:")
            label: qsTr("Сообщение")

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: sendMessage()
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Загрузить больше")
                onClicked: {
                    getHistory(true)
                    scrollMessagesToTop()
                }
            }
        }

        PushUpMenu {

            MenuItem {
                text: qsTr("Обновить")
                onClicked: {
                    markDialogAsRead()
                    getHistory()
                    scrollMessagesToBottom()
                }
            }

            MenuItem {
                text: qsTr("Прикрепить изображение")
                onClicked: {
                    var imagePicker = pageStack.push("Sailfish.Pickers.ImagePickerPage")
                    imagePicker.selectedContentChanged.connect(function () {
//                        loadingIndicator.running = true
                        photos.attachImage(imagePicker.selectedContent, "MESSAGE", 0)
                    })
                }
            }
        }
    }
}
