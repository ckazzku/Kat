#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = 0);

    static void setUserId(int _userId);
    static int getUserId() const;

signals:

public slots:
private:
    static int userId_;
};

#endif // SESSION_H
