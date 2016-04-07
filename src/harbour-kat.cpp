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
#include <QGuiApplication>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlContext>

#include <QUrl>
#include <QStandardPaths>
#include <QStringList>

#include "filedownloader.h"
#include "notificationhelper.h"
#include "storage.h"
#include "api/photos.h"

#include "coreclient.h"
#include "simpleconnection.h"

#include "models/audiomodel.h"
#include "models/buddymodel.h"
#include "models/chatmodel.h"
#include "models/commentsmodel.h"
#include "models/dialogsmodel.h"
#include "models/newsfeedmodel.h"
#include "models/wallmodel.h"

#include "roster.h"
#include "contact.h"
#include "longpoll.h"
#include "attachment.h"
#include "newsfeed.h"
#include "connection.h"
#include "reply.h"
#include "messagemodel.h"
#include "messagesession.h"

void registerAllTypes()
{
    using namespace Vreen;

    qmlRegisterType<BuddyModel>("Vreen.Base", 2, 0, "BuddyModel");
    qmlRegisterType<ChatModel>("Vreen.Base", 2, 0, "ChatModel");
    qmlRegisterType<DialogsModel>("Vreen.Base", 2, 0, "DialogsModel");
    qmlRegisterType<NewsFeedModel>("Vreen.Base", 2, 0, "NewsFeedModel");
    qmlRegisterType<CoreClient>("Vreen.Base", 2, 0, "CoreClient");

    qmlRegisterUncreatableType<Reply>("Vreen.Base", 2, 0, "Reply", QObject::tr("Don't use reply directly"));
    qmlRegisterUncreatableType<Client>("Vreen.Base", 2, 0, "ClientBase", QObject::tr("Use CoreClient instead"));
    qmlRegisterUncreatableType<Roster>("Vreen.Base", 2, 0, "Roster", QObject::tr("Use client.roster instead"));
    qmlRegisterUncreatableType<Connection>("Vreen.Base", 2, 0, "Connection", QObject::tr("Use client.connection instead"));
    qmlRegisterUncreatableType<Contact>("Vreen.Base", 2, 0, "Contact", QObject::tr("Use client.contact(id)"));
    qmlRegisterUncreatableType<Buddy>("Vreen.Base", 2, 0, "Buddy", QObject::tr("Use roster.buddy(id)"));
    qmlRegisterUncreatableType<Message>("Vreen.Base", 2, 0, "Message", QObject::tr("Only flags"));
    qmlRegisterUncreatableType<LongPoll>("Vreen.Base", 2, 0, "LongPoll", QObject::tr("Use client.longPoll instead"));
    qmlRegisterUncreatableType<Attachment>("Vreen.Base", 2, 0, "Attachment", QObject::tr("Attachment enums"));
    qmlRegisterUncreatableType<NewsItem>("Vreen.Base", 2, 0, "NewsItem", QObject::tr("NewsItem enums"));
    qmlRegisterUncreatableType<NewsFeed>("Vreen.Base", 2, 0, "NewsFeed", QObject::tr("NewsFeed enums"));
    qmlRegisterUncreatableType<MessageListModel>("Vreen.Base", 2, 0, "MessageListModel", QObject::tr("Cannot use this class directly"));
    qmlRegisterUncreatableType<MessageSession>("Vreen.Base", 2, 0, "MessageSession", QObject::tr("Cannot use this class directly"));
}

int main(int argc, char *argv[])
{
    QGuiApplication* application = SailfishApp::application(argc, argv);
    QQuickView* view = SailfishApp::createView();

    FileDownloader fileDownloader(view);
    NotificationHelper notificationHelper(view);
    Photos photos(view);

    CoreClient client(view);
    SimpleConnection connection;
    client.setConnection(&connection);
    client.connectToHost();
    client.longPoll()->setPollInterval(1000);
    client.longPoll()->setRunning(true);

    ChatModel chatModel;
    chatModel.setClient(&client);

    DialogsModel dialogsModel;
    dialogsModel.setClient(&client);

    NewsFeedModel newsFeedModel;
    newsFeedModel.setClient(&client);

    registerAllTypes();

    QUrl cachePath;
    QStringList cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    if (cacheLocation.isEmpty()) cachePath = getenv("$XDG_CACHE_HOME/harbour-kat/");
    else cachePath = QString("%1/").arg(cacheLocation.first());

    view->rootContext()->setContextProperty("chatModel", &chatModel);
    view->rootContext()->setContextProperty("dialogsModel", &dialogsModel);
    view->rootContext()->setContextProperty("newsFeedModel", &newsFeedModel);
    view->rootContext()->setContextProperty("client", &client);
    view->rootContext()->setContextProperty("cachePath", cachePath);
    view->rootContext()->setContextProperty("fileDownloader", &fileDownloader);
    view->rootContext()->setContextProperty("notificationHelper", &notificationHelper);
    view->rootContext()->setContextProperty("storage", Storage::instance().data());
    view->rootContext()->setContextProperty("photos", &photos);

    view->setSource(SailfishApp::pathTo("qml/harbour-kat.qml"));
    view->show();

    return application->exec();
}
