#include "coreclient.h"

#include <QSettings>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QThread>
#include <QTextDocument>

#include "message.h"
#include "roster.h"
#include "longpoll.h"
#include "utils.h"

CoreClient::CoreClient(QObject *parent)
    : Vreen::Client(parent)
{
    connect(this, SIGNAL(onlineStateChanged(bool)), this,
                SLOT(onOnlineStateChanged(bool)));

    QSettings settings;
    settings.beginGroup("connection");
//    setLogin(settings.value("login").toString());
//    setPassword(settings.value("password").toString());
    settings.endGroup();

    auto manager = new QNetworkConfigurationManager(this);
    connect(manager, SIGNAL(onlineStateChanged(bool)), this, SLOT(setOnline(bool)));

    connect(longPoll(), SIGNAL(messageAdded(Vreen::Message)), SLOT(onMessageAdded(Vreen::Message)));
    connect(this, SIGNAL(replyCreated(Vreen::Reply*)), SLOT(onReplyCreated(Vreen::Reply*)));
    connect(this, SIGNAL(error(Vreen::Client::Error)), SLOT(onReplyError(Vreen::Client::Error)));
}

QObject *CoreClient::request(const QString &method, const QVariantMap &args)
{
    return Vreen::Client::request(method, args);
}

Vreen::Contact *CoreClient::contact(int id)
{
    return roster()->buddy(id);
}

void CoreClient::onOnlineStateChanged(bool isOnline)
{
    if (isOnline) {
        //save settings (TODO use crypto backend as possible)
        QSettings settings;
        settings.beginGroup("connection");
//        settings.setValue("login", login());
//        settings.setValue("password", password());
        settings.endGroup();
    }
}

void CoreClient::setOnline(bool set)
{
    set ? connectToHost()
        : disconnectFromHost();
}

void CoreClient::onMessageAdded(const Vreen::Message &msg)
{
    if (msg.isIncoming() && msg.isUnread())
        emit messageReceived(contact(msg.fromId()));
}

void CoreClient::onReplyCreated(Vreen::Reply *reply)
{
    qDebug() << "--SendReply:" << reply->networkReply()->url();
    connect(reply, SIGNAL(resultReady(QVariant)),SLOT(onReplyFinished(QVariant)));
}

void CoreClient::onReplyFinished(const QVariant &)
{
    Vreen::Reply *reply = Vreen::sender_cast<Vreen::Reply*>(sender());
    qDebug() << "--Reply finished" << reply->networkReply()->url().toEncoded();
    qDebug() << "--Data" << reply->response();
}

void CoreClient::onReplyError(Error error)
{
    qDebug() << "--Error" <<  error;
}
