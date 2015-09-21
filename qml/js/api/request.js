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

.import "../storage.js" as StorageJS

var API_SERVER = "https://api.vk.com/method/";
var API_VERSION = "v=5.35"

function sendRequestTo(query, callback) {
    console.log(query)

    var request = new XMLHttpRequest()
    request.onreadystatechange = function() {
        if (request.readyState === XMLHttpRequest.DONE) {
            console.log(request.responseText)
            if (typeof callback !== 'undefined' && request.responseText) {
                var callback_args = [JSON.parse(request.responseText)]
                for (var i = 2; i < arguments.length; ++i)
                    callback_args.push(arguments[i])

                callback.apply(null, callback_args)
            }
        }
    }
    request.open("GET", query, true)
    request.send()
}

function sendRequest(method, data, callback, isNew) {
    var query = API_SERVER + method + "?" + API_VERSION +
            "&access_token=" + StorageJS.readSettingsValue("access_token");
    for (var arg in data) if (data[arg] !== "") query += "&" + arg + "=" + data[arg];

    sendRequestTo(query, callback, isNew)
}

function sendLongPollRequest(server, data, callback, warkers) {
    var query = "https://" + server + "?act=a_check";
    for (var arg in data) if (data[arg] !== "") query += "&" + arg + "=" + data[arg];

    sendRequestTo(query, callback, warkers)
}
