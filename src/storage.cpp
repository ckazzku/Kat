#include "storage.h"
#include "session.h"
#include <QException>

Storage::Storage(QObject *parent) :
    QObject(parent)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(getPathToDatabase());
    if (!db_.open()) {
        qDebug() << "failed to open database";
        throw new QException(); // TODO: use more specific exception
    }

    // TODO: handle errors
    execQuery("CREATE TABLE IF NOT EXISTS settings (key TEXT UNIQUE, value TEXT)");
    execQuery("CREATE TABLE IF NOT EXISTS messages (id           INTEGER UNIQUE, \
                                                    chat_id      INTEGER,  \
                                                    user_id      INTEGER, \
                                                    from_id      INTEGER,  \
                                                    date         INTEGER,  \
                                                    is_read      INTEGER,  \
                                                    is_out       INTEGER,  \
                                                    title        TEXT,  \
                                                    body         TEXT,  \
                                                    geo          TEXT,  \
                                                    attachments  TEXT,  \
                                                    fwd_messages TEXT)");
    execQuery("CREATE TABLE IF NOT EXISTS users (id         INTEGER UNIQUE,  \
                                                    first_name TEXT, \
                                                    last_name  TEXT, \
                                                    avatar     TEXT)");
}

Storage::~Storage() {
    db_.close();
}

QSharedPointer<QSqlQuery> Storage::execQuery(const QString &_query, const QVariantList &_data) {
    QSharedPointer<QSqlQuery> query(new QSqlQuery());
    query->prepare(_query);
    for (const auto &val: _data) {
        query->addBindValue(val);
    }
    if (query->exec()) {
        return query;
    } else {
        qDebug() << query->lastError();
        return nullptr;
    }
}

void Storage::putKeyval(const QString &_table, const QString &_key, const QString &_value) {
    execQuery("INSERT OR REPLACE INTO ? VALUES (?, ?)", {_table, _key, _value});
}

QString Storage::getKeyval(const QString &_table, const QString &_key, const QString &_default="") const {
    auto query = execQuery("SELECT value FROM ? WHERE key=?", {_table, _key});
    if (query && query->next()) {
        return query->value(0).toString();
    } else {
        return _default;
    }
}

QString Storage::getAccessToken() {
    return getSettings("access_token", "");
}

void Storage::putSettings(const QString &_key, const QString &_value) {
    putKeyval("settings", _key, _value);
}

void Storage::putMyName(const QString &_firstName, const QString &_lastName) {
    execQuery("UPDATE OR IGNORE users set first_name=?, last_name=? where id=?", {_firstName, _lastName, Session::getUserId()});
    execQuery("INSERT OR IGNORE INTO users (id, first_name, last_name) VALUES (?, ?, ?)", {Session::getUserId(), _firstName, _lastName});
}

void Storage::putMyAvatar(const QString &_filename) {
    execQuery("UPDATE OR IGNORE users set user_avatar=? where id=?", {_filename, Session::getUserId()});
    execQuery("INSERT OR IGNORE INTO users (id, user_avatar) VALUES (?, ?)", {user_id, _filename});
}

void Storage::getMyName() const {
    // TODO: ability to change first/last name order
    return execQuery("SELECT last_name||' '||first_name from users where id=?", {Session::getUserId()});
}

void Storage::getMyAvatar() const {
    return execQuery("SELECT user_avatar from users where id=?", {Session::getUserId()});
}

void Storage::putUserInfo(int _userId, const QString &_firstName, const QString &_lastName, const QString &_avatarFilename)
{
    execQuery("INSERT OR REPLACE INTO users (id, first_name, last_name, user_avatar) VALUES (?, ?, ?, ?)", {_userId, _firstName, _lastName, _avatarFilename});
}


QString Storage::getPathToDatabase() {
    QString pathToDatabase;

    QStringList cacheLocation = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    if (cacheLocation.isEmpty()) pathToDatabase = getenv("$XDG_DATA_HOME/harbour-kat/");
    else pathToDatabase = QString("%1/").arg(cacheLocation.first());
    pathToDatabase.append("QML/OfflineStorage/Databases/");
    pathToDatabase.append(DB_NAME);

    qDebug() << "Path to database:" << pathToDatabase;
    return pathToDatabase;
}
