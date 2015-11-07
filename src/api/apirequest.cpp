#include "apirequest.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <functional>

const QString ApiRequest::BASE_URL = "https://api.vk.com/method/";
int ApiRequest::seq_ = 0;
QSharedPointer<ApiRequest> ApiRequest::instance_ = QSharedPointer<ApiRequest>();

QSharedPointer<ApiRequest> ApiRequest::instance(QObject *parent)
{
    if (instance_.isNull()){
        instance_ = QSharedPointer<ApiRequest>(new ApiRequest(parent));
    }
    return instance_;
}

ApiRequest::ApiRequest(QObject *parent) :
    QObject(parent)
{
}

QSharedPointer<QThenable> ApiRequest::call(const QString &_method, const QHash<QString, QString> &_args) {
    QString url = "";
    url.append(BASE_URL).append(_method).append("?v="+version_)
       .append("&access_token=").append(Storage::instance()->getAccessToken());

    auto end = _args.cend();
    for (auto iterator=_args.cbegin();iterator!=end; ++iterator) {
        url.append("&").append(iterator.key()).append("=").append(iterator.value());
    }

    qDebug() << "Request: " << url;

    requestMutex_.lock();
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    auto thenable = QThenable::create();
    connect(mgr, &QNetworkAccessManager::finished, std::bind(&ApiRequest::onRequestFinished, this, _method, ApiRequest::seq_, thenable, std::placeholders::_1));
    connect(mgr, &QNetworkAccessManager::finished, mgr, &QNetworkAccessManager::deleteLater);

    mgr->get(QNetworkRequest(QUrl(url)));
    ApiRequest::seq_++;
    requestMutex_.unlock();
    return thenable;
}

/*std::future<QJsonObject> ApiRequest::call(const QString &_method, const QJsonObject &_args)
{
    QHash<QString, QString> args;

    for (auto &key: _args.keys()) {
        args[key] = _args[key].toString();
    }

    return this->call(_method, args);
}*/

void ApiRequest::onRequestFinished(const QString &_method, int _seq, QSharedPointer<QThenable> _thenable, QNetworkReply *_rep) {
    if (_rep->error() == QNetworkReply::NoError) {

        QJsonDocument document = QJsonDocument::fromJson(_rep->readAll());
        QJsonObject object = document.object();

        if (_thenable) {
            _thenable->fulfill(object);
        }
        // TODO: store cookies
        //emit gotResponse(_method, _seq, object);
    } else {
        qDebug() << "Failure:" << _rep->errorString();
    }
}
