#ifndef STORAGE_H
#define STORAGE_H

#include <QDebug>
#include <QDir>
#include <QObject>
#include <QSharedPointer>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>

class Storage : public QObject
{
    Q_OBJECT
public:
    Storage(QObject* parent=0);
    ~Storage();

    static QSharedPointer<Storage> instance(QObject *parent = 0);
    static const int DbVersion = 6;
    static const QString DbName;

    Q_INVOKABLE QString getAccessToken() const;

    Q_INVOKABLE void putSettings(const QString &_key, const QString &_value);
    Q_INVOKABLE QString getSettings(const QString &_key, const QString &_default="") const;

    Q_INVOKABLE void putMyName(const QString &_firstName, const QString &_lastName);
    Q_INVOKABLE void putMyAvatar(const QString &_filename);
    Q_INVOKABLE QString getMyName() const;
    Q_INVOKABLE QString getMyAvatar() const;
    Q_INVOKABLE int getMyUid() const;
    Q_INVOKABLE void putUserInfo(int _userId, const QString &_firstName, const QString &_lastName, const QString &_avatarFilename);

signals:
    void accessTokenChanged(const QString &token);
    void uidChanged(int uid);

public slots:
    bool clearCache();

private:
    static QSharedPointer<Storage> m_instance;
    QSqlDatabase m_db;

    QSharedPointer<QSqlQuery> execQuery(const QString &_query, const QVariantList &_data) const;
    void    putKeyval(const QString &_table, const QString &_key, const QString &_value);
    QString getKeyval(const QString &_table, const QString &_key, const QString &_default="") const;
    QString fetchFirst(const QSharedPointer<QSqlQuery> &_query, const QString &_default="") const;

    QString getPathToDatabase() const;
};
#endif // STORAGE_H
