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
.import "request.js" as RequestAPI


// -------------- API functions --------------

function api_getAudio(oid, aid) {
    RequestAPI.sendRequest("audio.getById",
                           { audios: oid + "_" + aid },
                           callback_getAudio)
}


// -------------- Callbacks --------------

function callback_getAudio(jsonObject) {
    var audioItem = jsonObject.response[0]
    playAudio(audioItem.url, audioItem.duration)
}


// -------------- Other functions --------------
