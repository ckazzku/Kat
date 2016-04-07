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
          name         - user name
          avatarSource - URL of user avatar
          isOnline     - online status of user
    */
    id: userItem

    property QtObject contact: model.contact

    property string name: contact ? contact.name : ""
    property string avatarSource: contact ? contact.photoSource : ""
    property bool isOnline: contact && contact.status !== Contact.Offline

    width: parent.width
    height: Theme.itemSizeMedium

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
            height: nameLabel.height
            width: height
            automaticCheck: false
            checked: isOnline
        }

        Label {
            id: nameLabel
            width: parent.width - onlineStatus.width - Theme.paddingMedium
            color: Theme.primaryColor
            text: '<b>' + EmojiOne.toImage(name) + '</b>'
            truncationMode: TruncationMode.Fade
        }
    }

    Component.onCompleted: {
        if (contact)
            contact.update()
    }
}
