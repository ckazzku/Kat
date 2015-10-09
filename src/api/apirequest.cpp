#include "apirequest.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

const QString ApiRequest::BASE_URL = "https://api.vk.com/method/";
int ApiRequest::seq_ = 0;

ApiRequest::ApiRequest(const QString &_version, QObject *parent) :
    QObject(parent),
    version_(_version)
{
}

void ApiRequest::call(const QString &_method, const QHash<QString, QString> &_args) {
    QString url = "";
    url.append(BASE_URL).append(_method).append("?v="+version_)
       .append("&access_token=").append(Storage().getAccessToken());

    auto end = _args.cend();
    for (auto iterator=_args.cbegin();iterator!=end; ++iterator) {
        url.append("&").append(iterator.key()).append("=").append(iterator.value());
    }

    qDebug() << "Request: " << url;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, &QNetworkAccessManager::finished, std::bind(&ApiRequest::onRequestFinished, this, _method, ApiRequest::seq_, std::placeholders::_1));
    connect(mgr, &QNetworkAccessManager::finished, mgr, &QNetworkAccessManager::deleteLater);

    mgr->get(QNetworkRequest(QUrl(url)));
    ApiRequest::seq_++;
}

void ApiRequest::call(const QString &_method, const QJsonObject &_args)
{
    QHash<QString, QString> args;

    for (auto &key: _args.keys()) {
        args[key] = _args[key].toString();
    }

    this->call(_method, args);
}

void ApiRequest::onRequestFinished(const QString &_method, int _seq, QNetworkReply *_rep) {
    if (_rep->error() == QNetworkReply::NoError) {

        QJsonDocument document = QJsonDocument::fromJson(_rep->readAll());
        QJsonObject object = document.object();
        // TODO: store cookies
        emit gotResponse(_method, _seq, object);
    } else {
        qDebug() << "Failure:" << _rep->errorString();
    }
}
