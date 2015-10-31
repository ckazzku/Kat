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
#include <QSharedPointer>

class Storage : public QObject
{
    Q_OBJECT

    Storage(QObject* parent=0);
    static QSharedPointer<Storage> instance_;
public:
    static const QString DB_NAME;
    static QSharedPointer<Storage> instance(QObject *parent = 0);

    ~Storage();

    Q_INVOKABLE QString getAccessToken() const;

    Q_INVOKABLE void    putSettings(const QString &_key, const QString &_value);
    Q_INVOKABLE QString getSettings(const QString &_key, const QString &_default="") const;

    Q_INVOKABLE void       putMyName(const QString &_firstName, const QString &_lastName);
    Q_INVOKABLE void       putMyAvatar(const QString &_filename);
    Q_INVOKABLE QString    getMyName() const;
    Q_INVOKABLE QString    getMyAvatar() const;
    Q_INVOKABLE void    putUserInfo(int _userId, const QString &_firstName, const QString &_lastName, const QString &_avatarFilename);



signals:

public slots:
    bool clearCache();

private:
    QSqlDatabase db_;

    QSharedPointer<QSqlQuery> execQuery(const QString &_query, const QVariantList &_data) const;
    void    putKeyval(const QString &_table, const QString &_key, const QString &_value);
    QString getKeyval(const QString &_table, const QString &_key, const QString &_default="") const;
    QString fetchFirst(const QSharedPointer<QSqlQuery> &_query, const QString &_default="") const;

    QString getPathToDatabase() const;
};
#endif // STORAGE_H
