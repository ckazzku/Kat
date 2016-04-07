/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
#ifndef CHATMODEL_H
#define CHATMODEL_H
#include "chatmodel.h"
#include "messagemodel.h"
#include <QPointer>

namespace Vreen {
class Buddy;
class MessageSession;
class Reply;
} // namespace Vreen

using MessageListModel = Vreen::MessageListModel;

class ChatModel : public MessageListModel
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(Vreen::MessageSession* session READ messageSession NOTIFY messageSessionChanged)
    Q_PROPERTY(int count READ count NOTIFY requestFinished)
    Q_PROPERTY(QList<QObject*> participants READ participants NOTIFY participantsChanged)
public:
    explicit ChatModel(QObject *parent = 0);
    QString title() const;
    void setMessageSession(Vreen::MessageSession *session);
    Vreen::MessageSession *messageSession() const;
    QList<QObject*> participants();

    Q_INVOKABLE QString getMessageBody(int index);
protected:
    void doInsertMessage(int index, const Vreen::Message &message);
public slots:
    void setChatId(int chatId);
    Vreen::Reply *getHistory(int count = 16, int offset = 0);
    Vreen::Reply *markAsRead();
    Vreen::Reply *sendMessage(const QString &message, const QString &subject = QString());
signals:
    void titleChanged(const QString &title);
    void requestFinished();
    void messageSessionChanged(const Vreen::MessageSession *);
    void participantsChanged(QList<QObject*>);
private slots:
    void messageReadStateChanged(int id, bool set);
    void participantsChanged(Vreen::Buddy*);
private:
    QPointer<Vreen::MessageSession> m_session;
};

#endif // CHATMODEL_H

