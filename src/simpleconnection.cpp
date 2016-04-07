#include "simpleconnection.h"
#include "json.h"
#include "reply.h"
#include "storage.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkReply>
#include <QStringList>
#include <QStringBuilder>
#include <QUrl>
#include <QUrlQuery>

using namespace Vreen;

SimpleConnection::SimpleConnection(QObject *parent) :
    Connection(parent),
    m_connectionState(Client::StateOffline)
{
    m_token = Storage::instance()->getAccessToken();
    m_uid = Storage::instance()->getMyUid();

    connect(Storage::instance().data(), SIGNAL(accessTokenChanged(QString)), this, SLOT(setToken(QString)));
    connect(Storage::instance().data(), SIGNAL(uidChanged(int)), this, SLOT(setUid(int)));
}

void SimpleConnection::connectToHost(const QString &login, const QString &password)
{
    Q_UNUSED(login)
    Q_UNUSED(password)
    setConnectionState(Client::StateOnline);
}

void SimpleConnection::disconnectFromHost()
{
    setConnectionState(Client::StateOffline);
}

QNetworkRequest SimpleConnection::makeRequest(const QString &method, const QVariantMap &args)
{
    QUrlQuery query;
    for (auto it = args.constBegin(); it != args.constEnd(); it++)
        query.addQueryItem(QUrl::toPercentEncoding(it.key()),
                           QUrl::toPercentEncoding(it.value().toString()));
    query.addQueryItem("access_token", QUrl::toPercentEncoding(m_token));

    QUrl url(BaseUrl.url() % QStringLiteral("/") % method);
    url.setQuery(query);

    QNetworkRequest request(url);
    return request;
}

void SimpleConnection::decorateRequest(QNetworkRequest &request)
{
    auto url = request.url();

    QUrlQuery query(url);
    query.addQueryItem("access_token", QUrl::toPercentEncoding(m_token));

    url.setQuery(query);
    request.setUrl(url);
}

Client::State SimpleConnection::connectionState() const
{
    return m_connectionState;
}

int SimpleConnection::uid() const
{
    return m_uid;
}

void SimpleConnection::setConnectionState(Client::State state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        emit connectionStateChanged(state);
    }
}

void SimpleConnection::setToken(const QString &token)
{
    if (m_token != token) {
        m_token = token;
    }
}

void SimpleConnection::setUid(int uid)
{
    if (m_uid != uid) {
        m_uid = uid;
    }
}

void SimpleConnection::onReplyFinished()
{
    //TODO
    qDebug() << "!@# SimpleConnection::onReplyFinished";
}

void SimpleConnection::onReplyError(QNetworkReply::NetworkError error)
{
    //TODO

    qDebug() << "!@# SimpleConnection::onReplyError " << error;
}
