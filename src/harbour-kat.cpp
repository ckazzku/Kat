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

//#include <sailfishapp.h>
#include <QApplication>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlContext>

#include <QUrl>
#include <QStandardPaths>
#include <QStringList>

#include "filedownloader.h"
#include "notificationhelper.h"
#include "api/photos.h"
#include "session.h"

const QString CURRENT_API_VERSION = "5.37";

int main(int argc, char *argv[])
{
#ifdef OS_SAILFISH
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
#else
    QApplication application(argc, argv);
    QQuickView* view=new QQuickView();
#endif

    QScopedPointer<FileDownloader> fileDownloader(new FileDownloader(view));
    QScopedPointer<NotificationHelper> notificationHelper(new NotificationHelper(view));
    QScopedPointer<Photos> photos(new Photos(view));
    QScopedPointer<ApiRequest> api(new ApiRequest(CURRENT_API_VERSION, view));
    QScopedPointer<Storage> storage(new Storage(view));
    QScopedPointer<Session> session(new Session(view));

    QUrl cachePath;
    QStringList cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    if (cacheLocation.isEmpty()) cachePath = getenv("$XDG_CACHE_HOME/harbour-kat/");
    else cachePath = QString("%1/").arg(cacheLocation.first());

    view->rootContext()->setContextProperty("cachePath", cachePath);
    view->rootContext()->setContextProperty("fileDownloader", fileDownloader.data());
    view->rootContext()->setContextProperty("notificationHelper", notificationHelper.data());
    view->rootContext()->setContextProperty("photos", photos.data());

    view->rootContext()->setContextProperty("Api", api.data());
    view->rootContext()->setContextProperty("Storage", storage.data());
    view->rootContext()->setContextProperty("Session", session.data());
#ifdef OS_SAILFISH
    view->setSource(SailfishApp::pathTo("qml/harbour-kat.qml"));
#else
    view->setSource(QUrl("qrc:/qml/harbour-kat.qml"));
#endif
    view->show();

    return application.exec();
}

