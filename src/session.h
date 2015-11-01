#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QSharedPointer>

class Session : public QObject
{
    Q_OBJECT

    Session(QObject* parent=0);
    static QSharedPointer<Session> instance_;
public:
    static QSharedPointer<Session> instance(QObject *parent = 0);

    void setUserId(int _userId);
    int getUserId();

signals:

public slots:
private:
    int userId_;
};

#endif // SESSION_H
