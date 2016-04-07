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
    id: dialogsListPage

    property int count: 5
    property int offset: 0
    property int previewLength: 160

    function getDialogs() {
        loadingIndicator.running = true

        var reply = dialogsModel.getDialogs(count, offset, previewLength)
        if (reply) {
            reply.resultReady.connect(function(data) {
                if (data) {
                    offset += data.length - 1
                }
                loadingIndicator.running = false
            });
        }
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

        SilicaListView {
            id: dialogsList
            anchors.fill: parent
            anchors.bottomMargin: Theme.paddingMedium

            header: PageHeader {
                title: qsTr("Сообщения")
            }

            model: dialogsModel

            delegate: DialogItem {
                id: item

                onClicked: {
                    console.log("dialogId = " + item.itemId)
                    pageContainer.push(Qt.resolvedUrl("../pages/DialogPage.qml"),
                                       { "title": item.title,
                                        "chatId": item.itemId })
                }
            }

            VerticalScrollDecorator {}
        }

        PullDownMenu {

            MenuItem {
                id: newMessageItem
                text: qsTr("Новое сообщение")
                onClicked: pageContainer.push(Qt.resolvedUrl("NewMessagePage.qml"))
            }

            MenuItem {
                id: mainMenuItem
                text: qsTr("Обновить")
                onClicked: {
                    offset = 0
                    getDialogs()
                }
            }
        }

        PushUpMenu {

            MenuItem {
                text: qsTr("Загрузить больше")
                onClicked: getDialogs()
            }
        }
    }

    Component.onCompleted: {
        getDialogs()
    }
}
