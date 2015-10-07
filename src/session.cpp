#include "session.h"

int Session::userId_ = 0;

// Make as singleton

Session::Session(QObject *parent) :
    QObject(parent)
{
}

void Session::setUserId(int _userId)
{
    userId_ = _userId;
}

int Session::getUserId()
{
    return userId_;
}
