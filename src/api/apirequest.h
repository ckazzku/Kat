#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QHash>
#include <QHashIterator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

#include "../storage.h"

class ApiRequest : public QObject
{
    Q_OBJECT

    static const QString BASE_URL;

    QString version_;

public:
    ApiRequest(QObject *parent = 0) = delete;
    ApiRequest(const QString &_version, QObject *parent = 0);
    void call(const QString &_method, const QHash<QString, QString> &args);

signals:
    void gotResponse(const QJsonObject &_jsonData);

private slots:
    void onRequestFinished(QNetworkReply *rep);
};

#endif // APIREQUEST_H
