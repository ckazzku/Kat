#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QHash>
#include <QHashIterator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QMutex>
#include <QQuickItem>

#include "../storage.h"

class ApiRequest : public QObject
{
    Q_OBJECT

    static const QString BASE_URL;
    static int seq_;

    QString version_;
    QMutex  requestMutex_;
    QQuickItem* qml_;

    ApiRequest(const QString &_version, QObject *parent = 0);
    static QSharedPointer<ApiRequest> instance_;
public:
    static QSharedPointer<ApiRequest> instance(const QString &_version, QObject *parent = 0);

    Q_INVOKABLE void call(const QString &_method, const QHash<QString, QString> &_args, const QString &_callback=nullptr);
    Q_INVOKABLE void call(const QString &_method, const QJsonObject &_args, const QString &_callback=nullptr);

    inline void setQuickObject(QQuickItem* _qml){qml_=_qml;}

signals:
    void gotResponse(const QString &_method, int _seq, const QJsonObject &_jsonData);

private slots:
    void onRequestFinished(const QString &_method, int _seq, const QString &_callback, QNetworkReply *_rep);
};

#endif // APIREQUEST_H
