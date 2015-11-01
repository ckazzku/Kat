#include "session.h"

QSharedPointer<Session> Session::instance_ = QSharedPointer<Session>();

Session::Session(QObject *parent) :
    QObject(parent), userId_(0)
{
}

QSharedPointer<Session> Session::instance(QObject *parent)
{
    if (instance_.isNull()){
        instance_ = QSharedPointer<Session>(new Session(parent));
    }
    return instance_;
}

void Session::setUserId(int _userId)
{
    userId_ = _userId;
}

int Session::getUserId()
{
    return userId_;
}
