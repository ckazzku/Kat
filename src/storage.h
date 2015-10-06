#ifndef STORAGE_H
#define STORAGE_H

#include <QDebug>

#include <QDir>
#include <QObject>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class Storage : public QObject
{
    Q_OBJECT
public:
    static const QString DB_NAME = "harbour-kat-db";

    explicit Storage(QObject *parent = 0);
    ~Storage();

    QString getAccessToken() const;

    void    putSettings(const QString &_key, const QString &_value);
    QString getSettings(const QString &_key) const;

    void    putMyName(const QString &_firstName, const QString &_lastName);
    void    putMyAvatar(const QString &_filename);
    void    getMyName() const;
    void    getMyAvatar() const;
    void    putUserInfo(int _userId, const QString &_firstName, const QString &_lastName, const QString &_avatarFilename);



signals:

public slots:

private:
    QSqlDatabase db_;

    QSharedPointer<QSqlQuery> execQuery(const QSqlQuery &_query, const QVariantList &_data) const;
    void    putKeyval(const QString &_table, const QString &_key, const QString &_value);
    QString getKeyval(const QString &_table, const QString &_key, const QString &_default="") const;

    QString getPathToDatabase() const;
};

#endif // STORAGE_H
