#include "apirequest.h"

#include <QDebug>

ApiRequest::ApiRequest(const QString &_version, QObject *parent) :
    QObject(parent),
    version_(_version)
{
}

void ApiRequest::call(const QString &_method, const QHash<QString, QString> &_args) const {
    QString url = "";
    url.append(BASE_URL).append(_method).append("?v="+version_)
       .append("&access_token=").append(Storage().getAccessToken());

    for (auto &iterator: _args) {
        url.append("&").append(iterator.key()).append("=").append(iterator.value());
    }

    qDebug() << "Request: " << url;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, &QNetworkAccessManager::finished, this, &ApiRequest::onRequestFinished);
    connect(mgr, &QNetworkAccessManager::finished, mgr, QNetworkAccessManager::deleteLater);

    mgr->get(QNetworkRequest(QUrl(url)));
}

void ApiRequest::onRequestFinished(QNetworkReply *rep) {
    if (rep->error() == QNetworkReply::NoError) {

        QJsonDocument document = QJsonDocument::fromJson(rep->readAll().toUtf8());
        QJsonObject object = document.object();
        // TODO: store cookies
        emit gotResponse(object);
    } else {
        qDebug() << "Failure:" << rep->errorString();
    }
}
