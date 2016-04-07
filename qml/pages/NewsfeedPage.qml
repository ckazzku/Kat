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
    property int count: 10
    property Item contextMenu

    function getNews() {
        if (storage.getMyUid()) {
            loadingIndicator.running = true

            var reply = newsFeedModel.getNews(NewsFeed.FilterPost, count, newsFeedModel.count)
            if (reply) {
                reply.resultReady.connect(function() {
                    loadingIndicator.running = false
                });
            }
        }
    }

    function shownotification(text) {
        notificationHelper.sendNotification("", text, true)
    }

    BusyIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: false // true
    }

    SilicaListView {
        id: newsfeedList
        anchors.fill: parent

        PullDownMenu {

            MenuItem {
                text: "Написать"
                onClicked: pageContainer.push(Qt.resolvedUrl("NewWallPostPage.qml"))
            }

            MenuItem {
                text: qsTr("Обновить")
                onClicked: doStartUpdate()
            }
        }

        model: newsFeedModel

        header: PageHeader { title: qsTr("Новости") }

        delegate: Item {
            id: newsItem

            property bool menuOpen: contextMenu != null && contextMenu.parent === newsItem

            width: parent.width
            height: menuOpen ? contextMenu.height + contentItem.height : contentItem.height

            PostItem {
                id: contentItem
                width: parent.width

                onClicked: pageContainer.push(Qt.resolvedUrl("../pages/OneNewsPage.qml"),
                                              { "datetime":        date,
                                                "textBody":        body,
                                                "postAuthor":      owner,
                                                "itemId":          postId,
                                                "ownerId":         owner ? owner.id : 0,
                                                "commentsCount":   comments.count,
                                                "likesCount":      likes.count,
                                                "repostsCount":    reposts.count,
                                                "isPostLiked":     likes.count > 0,
                                                "isPostReposted":  reposts.count > 0,
                                                "attachmentsData": attachments })
                onPressAndHold: {
                    if (!contextMenu)
                        contextMenu = contextMenuComponent.createObject(newsfeedList)
                    contextMenu.show(newsItem)
                }
            }

            Component {
                id: contextMenuComponent

                ContextMenu {

                    MenuItem {
                        text: qsTr("Мне нравится")
                        onClicked: newsFeedModel.addLike(index)
                    }

                    onClosed: contextMenu = null
                }
            }
        }

        footer: Button {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width / 3 * 2
            text: qsTr("Загрузить больше")
            visible: newsFeedModel.count > 0

            onClicked: {
                getNews()
            }
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: {
        getNews()
    }
}
