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
#include "chatmodel.h"
#include "chatsession.h"
#include "client.h"
#include "contact.h"
#include "longpoll.h"
#include "groupchatsession.h"
#include <QCoreApplication>
#include <QDebug>

using namespace Vreen;

ChatModel::ChatModel(QObject *parent) :
    MessageListModel(parent)
{
    setSortOrder(Qt::AscendingOrder);
}

void ChatModel::setChatId(int chatId)
{
    if (!m_session.isNull()) {
        clear();
        m_session.data()->deleteLater();
    }
    if (!client()) {
        qWarning("ChatModel: client must be setted!");
        return;
    }

    MessageSession *session;
    if (chatId > 0)
    {
        session = new GroupChatSession(chatId, client());
        auto groupSession = qobject_cast<GroupChatSession*>(session);
        groupSession->getInfo();
        connect(groupSession, SIGNAL(participantAdded(Vreen::Buddy*)),
                        this, SLOT(participantsChanged(Vreen::Buddy*)));
    }
    else
    {
        auto contact = client()->contact(-chatId);
        session = new ChatSession(contact);
        session->setTitle(contact->name());
        emit participantsChanged(contact_cast<Buddy*>(contact));
    }
    setMessageSession(session);
}

QString ChatModel::title() const
{
    return !m_session.isNull() ? m_session->title() : tr("unknown");
}

void ChatModel::setMessageSession(MessageSession *session)
{
    auto longPoll = client()->longPoll();
    connect(session, SIGNAL(messageAdded(Vreen::Message)), SLOT(addMessage(Vreen::Message)));
    connect(session, SIGNAL(messageDeleted(int)), SLOT(removeMessage(int)));
    connect(session, SIGNAL(messageReadStateChanged(int,bool)),
            this, SLOT(messageReadStateChanged(int,bool)));
    connect(session, SIGNAL(titleChanged(QString)), SIGNAL(titleChanged(QString)));
    connect(longPoll, SIGNAL(messageFlagsReplaced(int, int, int)),
            this, SLOT(replaceMessageFlags(int, int, int)));
    connect(longPoll, SIGNAL(messageFlagsReseted(int, int, int)),
            this, SLOT(resetMessageFlags(int, int, int)));

    m_session = session;
    emit titleChanged(session->title());
    emit messageSessionChanged(m_session.data());
}

MessageSession *ChatModel::messageSession() const
{
    return m_session.data();
}

QObjectList ChatModel::participants()
{
    QObjectList participants;
    if (!m_session.isNull())
    {
        if (qobject_cast<GroupChatSession*>(m_session.data()))
        {
            auto session = qobject_cast<GroupChatSession*>(m_session.data());
            for (auto participant : session->participants())
                participants.push_back(participant);
        }
        else
        {
            auto session = qobject_cast<ChatSession*>(m_session.data());
            participants.push_back(session->contact());
        }
    }

    return participants;
}

QString ChatModel::getMessageBody(int index)
{
    if (!m_session.isNull() && count() > index)
    {
        auto message = at(index);
        return message.body();
    }

    return "";
}

void ChatModel::doInsertMessage(int index, const Message &message)
{
    MessageListModel::doInsertMessage(index, message);
}

Reply *ChatModel::getHistory(int count, int offset)
{
    if (!m_session.isNull()) {
        auto reply = m_session->getHistory(count, offset);
        connect(reply, SIGNAL(resultReady(QVariant)), SIGNAL(requestFinished()));
        return reply;
    }
    return nullptr;
}

Reply *ChatModel::markAsRead()
{
    if (!m_session.isNull()) {
        IdList ids;
        for (int i = 0; i < count(); ++i)
        {
            auto message = at(i);
            if (message.isIncoming() && message.isUnread())
                ids.append(message.id());
        }
        if (!ids.empty())
            return m_session->markMessagesAsRead(ids, true);
    }
    return nullptr;
}

Reply *ChatModel::sendMessage(const QString &body, const QString &subject)
{
    if (!m_session.isNull())
        return m_session->sendMessage(body, subject);
    return nullptr;
}

void ChatModel::messageReadStateChanged(int id, bool set)
{
    int index = findMessage(id);
    if (index == -1)
        return;

    auto message = at(index);
    message.setUnread(!set);
    doReplaceMessage(index, message);
}

void ChatModel::participantsChanged(Buddy *)
{
    emit participantsChanged(participants());
}
