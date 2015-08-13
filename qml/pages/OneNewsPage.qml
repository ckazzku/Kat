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
import QtMultimedia 5.0
import Sailfish.Silica 1.0
import "../views"

import "../js/api/wall.js" as WallAPI
import "../js/storage.js" as StorageJS

Page {

    property string textBody
    property string datetime
    property string postAuthor
    property variant attachmentsData

    function update(postData) {
        textBody = postData[1]
        attachmentsData = postData.slice(3)
        newsContent.attachments = attachmentsData
        console.log(newsContent.attachments)
        newsContent.updateAttachments()
    }

    function playAudio(url) {
        audioPlayer.source = url
        audioPlayer.play()
    }

    function getVideoUrl(urls, quality) {
        var url = ""
        switch (quality) {
        case 0:
            url = urls.mp4_720
            break;

        case 1:
            url = urls.mp4_480
            break;

        case 2:
            url = urls.mp4_360
            break;

        case 3:
            url = urls.mp4_240
            break;
        }
        if (url) {
            return url
        } else if (quality < 3) {
            return getVideoUrl(urls, quality+1)
        } else {
            return
        }
    }

    function openVideoPlayer(urls, duration) {
        console.log(urls)
        console.log(duration)
        var url = getVideoUrl(urls, parseInt(StorageJS.readSettingsValue("video_quality"), 10))
//        console.log(url)

        if (url) {
            pageContainer.push("../pages/VideoPage.qml", { "url": url, "duration": duration })
        } else if (urls.external) {
            Qt.openUrlExternally(urls.external)
        }
    }

    Audio {
        id: audioPlayer
    }

    SilicaFlickable {
        anchors.fill: parent
        anchors.topMargin: Theme.paddingLarge
        anchors.bottomMargin: Theme.paddingLarge
        contentHeight: newsAuthor.height + newsContent.height

        PageHeader {
            id: newsAuthor
            title: postAuthor
        }

        ContentItem {
            id: newsContent
            anchors.top: newsAuthor.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            attachments: attachmentsData
            content: textBody
            dateTime: datetime
            isNews: false
        }
    }

//    Component.onCompleted: WallAPI.api_getPostById(-43948962, 76708)
}
