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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include <QApplication>
#include <QSharedPointer>
#include <QQuickView>
#include <QQmlContext>

#include <QUrl>
#include <QStandardPaths>
#include <QStringList>

#include "filedownloader.h"
#include "notificationhelper.h"
#include "storage.h"
#include "api/photos.h"
#include "session.h"

#include "models/newsfeedmodel.h"

const QString CURRENT_API_VERSION = "5.37";

int main(int argc, char *argv[])
{
    QSharedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    QQuickView* view = SailfishApp::createView();

    QSharedPointer<FileDownloader> fileDownloader(new FileDownloader(view));
    QSharedPointer<NotificationHelper> notificationHelper(new NotificationHelper(view));
    QSharedPointer<Photos> photos(new Photos(view));

    QSharedPointer<ApiRequest> api(ApiRequest::instance(view));
    QSharedPointer<Storage> storage(Storage::instance(view));
    QSharedPointer<Session> session(Session::instance(view));

    QUrl cachePath;
    QStringList cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    if (cacheLocation.isEmpty()) cachePath = getenv("$XDG_CACHE_HOME/harbour-kat/");
    else cachePath = QString("%1/").arg(cacheLocation.first());

    auto ctx = view->rootContext();
    ctx->setContextProperty("cachePath", cachePath);
    ctx->setContextProperty("fileDownloader", fileDownloader.data());
    ctx->setContextProperty("notificationHelper", notificationHelper.data());
    ctx->setContextProperty("photos", photos.data());

    ctx->setContextProperty("Api", api.data());
    ctx->setContextProperty("Storage", storage.data());
    ctx->setContextProperty("Session", session.data());

    ctx->setContextProperty("newsModel", new NewsFeedModel(view));

    view->setSource(SailfishApp::pathTo("qml/harbour-kat.qml"));

    api->setQuickObject(view->rootObject());
    api->setVersion(CURRENT_API_VERSION);

    view->show();

    return application->exec();
}

