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

.import QtQuick.LocalStorage 2.0 as LS


// -------------- Cache functions --------------

function prepareMessagePreview(body, attachments, fwd_messages) {
    if (body) body = body.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
    else body = ""
    if (fwd_messages) body = "[сообщения] " + body
    if (attachments) body = "[вложения] " + body
    return body
}

function getLastDialogs() {
    console.log('getLastDialogs()')
    var db = getDatabase()
    if (!db) return

    var value = []

    db.transaction( function (tx) {
        console.log("... reading ...")
        var result = tx.executeSql('SELECT users.id              AS user_id, ' +
                                          'users.avatar          AS avatar, ' +
                                          'users.first_name      AS first_name, ' +
                                          'users.last_name       AS last_name, ' +
                                          'dialogs.title         AS title, ' +
//                                          'messages.id           AS msg_id, ' +
                                          'messages.chat_id      AS chat_id, ' +
                                          'MAX(messages.date)    AS date, ' +
                                          'messages.is_read      AS is_read, ' +
                                          'messages.is_out       AS is_out, ' +
//                                          'messages.title        AS title, ' +
                                          'messages.body         AS body, ' +
//                                          'messages.geo          AS geo, ' +
                                          'messages.attachments  AS attachments, ' +
                                          'messages.fwd_messages AS fwd_messages ' +
                                   'FROM messages ' +
                                   'LEFT OUTER JOIN users ON users.id = messages.user_id ' +
                                   'LEFT OUTER JOIN dialogs ON dialogs.id = messages.chat_id ' +
                                   'GROUP BY messages.chat_id ' +
                                   'ORDER BY date DESC ' +
                                   'LIMIT 20')
        for (var i = 0; i < result.rows.length; i++) {
            var item = result.rows.item(i)
            value[i] = {
                isDialog:     true,
                out:          item.is_out,
                avatarSource: item.avatar ? cachePath + item.avatar :
                                            "image://theme/icon-cover-message",
                nameOrTitle:  item.chat_id !== item.user_id ? item.title :
                                                              item.first_name + ' ' + item.last_name,
                previewText:  prepareMessagePreview(item.body, item.attachments, item.fwd_messages),
                itemId:       item.chat_id,
                readState:    item.is_read,
                isOnline:     false,
                isChat:       item.chat_id !== item.user_id,
            }
        }
    })

    return value
}

function getLastMessagesForDialog(chatId) {
    console.log("getLastMessagesForDialog(" + chatId + ")")
    var db = getDatabase()
    if (!db) return

    var value = []

    db.transaction( function (tx) {
        console.log('... reading ...')
        var result = tx.executeSql('SELECT messages.id          AS id, ' +
                                          'messages.from_id     AS from_id, ' +
                                          'messages.user_id     AS user_id, ' +
                                          'messages.is_read     AS is_read, ' +
                                          'messages.is_out      AS is_out, ' +
                                          'messages.body        AS body, ' +
                                          'messages.date        AS date, ' +
                                          'messages.attachments AS attachments, ' +
                                          'users.avatar         AS avatar ' +
                                   'FROM messages ' +
                                   'LEFT OUTER JOIN users ON users.id = messages.from_id ' +
                                   'WHERE messages.chat_id = ' + chatId + ' ' +
                                   'ORDER BY date DESC ' +
                                   'LIMIT 50')
        var date = new Date()
        for (var i = 0; i < result.rows.length; i++) {
            var item = result.rows.item(i)
            date.setTime(parseInt(item.date) * 1000)
            value[i] = {
                mid:             item.id,
                fromId:          item.from_id ? item.from_id : item.user_id,
                readState:       item.is_read,
                out:             item.is_out,
                message:         item.body ? item.body.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;') :
                                             "",
//                datetime:        item.date,
                datetime:        ("0" + date.getHours()).slice(-2) + ":" +
                                 ("0" + date.getMinutes()).slice(-2) + ", " +
                                 ("0" + date.getDate()).slice(-2) + "." +
                                 ("0" + (date.getMonth() + 1)).slice(-2) + "." +
                                 ("0" + date.getFullYear()).slice(-2),
                attachmentsData: item.attachments ? JSON.parse(item.attachments) :
                                                    [],
                avatarSource:    item.avatar ? cachePath + item.avatar :
                                               "image://theme/icon-cover-people",
                isNewsContent:   false
            }
        }
    })

    return value
}

function saveAnotherUserInfo(userId, firstName, lastName, avatarName) {
    var db = getDatabase()
    if (!db) return

    db.transaction( function (tx) {
        tx.executeSql('INSERT OR REPLACE INTO users (id, first_name, last_name, avatar) VALUES (' +
                      userId + ', \"' + firstName + '\", \"' + lastName + '\", \"' +
                      avatarName + '\"' + ')')
    })
}

function saveMessage(id, chatId, userId, fromId, date, isRead, isOut, title, body, geo, attachments,
                     fwd_messages) {
    console.log("saveMessage(" + id + ")")
    var db = getDatabase()
    if (!db) return

    var values = [body, JSON.stringify(geo), JSON.stringify(attachments), JSON.stringify(fwd_messages)]

    db.transaction( function (tx) {
        console.log('... saving or updating ...')
        if (typeof title !== 'undefined') {
            tx.executeSql('INSERT OR REPLACE INTO dialogs (id, ' +
                                                          'title) ' +
                                             'VALUES (' + (chatId ? chatId : userId) + ', ' +
                                                    '\"' + title + '\")')
        }
        tx.executeSql('INSERT OR REPLACE INTO messages (id, ' +
                                                       'chat_id, ' +
                                            ( userId ? 'user_id, ' : '' ) +
                                            ( fromId ? 'from_id, ' : '' ) +
                                            ( date   ? 'date, '    : '' ) +
                     ( typeof isRead !== 'undefined' ? 'is_read, ' : '' ) +
                     ( typeof isOut !== 'undefined'  ? 'is_out, '  : '' ) +
                                                       'body, ' +
                                                       'geo, ' +
                                                       'attachments, ' +
                                                       'fwd_messages) ' +
                                           'VALUES (' + id           + ', ' +
                                       (chatId ?        chatId       + ', '   : userId + ', ' ) +
                                       (userId ?        userId       + ', '   : '' ) +
                                       (fromId ?        fromId       + ', '   : '' ) +
                                       (date   ?        date         + ', '   : '' ) +
                      (typeof isRead !== 'undefined' ?  isRead       + ', '   : '' ) +
                      (typeof isOut !== 'undefined'  ?  isOut        + ', '   : '' ) +
                                                       '?, ' +
                                                       '?, ' +
                                                       '?, ' +
                                                       '?)', values)
    })
}

function updateMessage(id, data) {
    console.log("updateMessage(" + id + ")")

    var db = getDatabase()
    if (!db) return

    var values = ''
    for (var key in data) {
        values += key + " = '" + JSON.stringify(data[key]) + "',"
    }

    if (values.length > 0) {
        values = values.substring(0, values.length - 1)

        db.transaction( function (tx) {
            console.log('... updating ...')
            tx.executeSql('UPDATE messages SET ' + values + ' WHERE id = ' + id)
        })
    }
}
