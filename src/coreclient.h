#ifndef CORECLIENT_H
#define CORECLIENT_H

#include "client.h"

using Client = Vreen::Client;

class CoreClient : public Client
{
    Q_OBJECT

public:
    explicit CoreClient(QObject *parent = 0);

    Q_INVOKABLE QObject *request(const QString &method, const QVariantMap &args = QVariantMap());
    Q_INVOKABLE Vreen::Contact *contact(int id);

signals:
    void messageReceived(Vreen::Contact *from);

private slots:
    void onOnlineStateChanged(bool state);
    void setOnline(bool set);
    void onMessageAdded(const Vreen::Message &msg);
    void onReplyCreated(Vreen::Reply *reply);
    void onReplyFinished(const QVariant &);
    void onReplyError(Vreen::Client::Error);
};

#endif // CORECLIENT_H
