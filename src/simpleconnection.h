#ifndef SIMPLECONNECTION_H
#define SIMPLECONNECTION_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include "connection.h"

class SimpleConnection : public Vreen::Connection
{
    Q_OBJECT
public:
    explicit SimpleConnection(QObject *parent = 0);

    void connectToHost(const QString &login, const QString &password);
    void disconnectFromHost();
    Vreen::Client::State connectionState() const;
    int uid() const;

    const QUrl BaseUrl = QUrl(QLatin1String("https://api.vk.com/method"));
protected:
    void setConnectionState(Vreen::Client::State connectionState);
    QNetworkRequest makeRequest(const QString &method, const QVariantMap &args);
    void decorateRequest(QNetworkRequest &request);
protected slots:
    void setToken(const QString &token);
    void setUid(int uid);
    void onReplyFinished();
    void onReplyError(QNetworkReply::NetworkError error);
private:
    Vreen::Client::State m_connectionState;
    QString m_token;
    int m_uid;
};

#endif // SIMPLECONNECTION_H

