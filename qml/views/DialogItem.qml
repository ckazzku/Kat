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
import "../emojione/emojione.js" as EmojiOne

BackgroundItem {
    /*
      Common fields:
          itemId       - id of user or dialog
          isDialog     - true if it is a dialog, false - elsewise
          avatarSource - URL of user avatar
          isOnline     - online status of user
          title        - user name or chat title
          previewText  - the text for preview. It may be last message or user status

      Dialogs' fields:
          isChat       - true if the dialog is chat, false - elsewise
          out          - true if last message is output, false - elsewise
          readState    - true if the dialog was read
    */
    id: dialogItem

    property bool isDialog: model.chatId === 0
    property bool isOut: !model.incoming
    property bool isRead: !model.unread
    property QtObject contact: isOut ? model.to : model.from
    property int itemId: isDialog && contact ? -contact.id : model.chatId
    property string avatarSource: contact ? contact.photoSource : ""
    property bool isOnline: isDialog && contact && contact.status !== Contact.Offline
    property string title: isDialog && contact ? contact.name : model.subject
    property string previewText: model.body

    width: parent.width
    height: Theme.itemSizeMedium
    highlighted: isDialog && !isOut && !isRead

    Image {
        id: avatar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: Theme.paddingSmall
        anchors.bottomMargin: Theme.paddingSmall
        anchors.leftMargin: Theme.paddingLarge
        width: height
        source: avatarSource
    }

    Row {
        anchors.top: avatar.top
        anchors.left: avatar.right
        anchors.leftMargin: Theme.paddingMedium
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingLarge
        spacing: Theme.paddingMedium

        Switch {
            id: onlineStatus
            height: name.height
            width: height
            automaticCheck: false
            checked: isOnline
        }

        Label {
            id: name
            width: parent.width - onlineStatus.width - Theme.paddingMedium
            color: isDialog && !isRead && !isOut ? Theme.highlightColor : Theme.primaryColor
            text: '<b>' + EmojiOne.toImage(title) + '</b>'
            truncationMode: TruncationMode.Fade
        }
    }

    Label {
        id: messagePreview
        anchors.bottom: avatar.bottom
        anchors.left: avatar.right
        anchors.leftMargin: Theme.paddingMedium
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingLarge
        maximumLineCount: 1
        color: !isDialog || isRead ? Theme.secondaryColor : Theme.secondaryHighlightColor
        linkColor: isRead ? Theme.secondaryColor : Theme.secondaryHighlightColor
        text: EmojiOne.toImage(previewText)
        truncationMode: TruncationMode.Fade
    }

    Component.onCompleted: {
        if (contact)
            contact.update()
    }
}
