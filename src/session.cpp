#include "session.h"

Session::Session(QObject *parent) :
    QObject(parent), userId_(0)
{
}

void Session::setUserId(int _userId)
{
    userId_ = _userId;
}

int Session::getUserId() const
{
    return userId_;
}
