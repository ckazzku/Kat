#ifndef NEWSFEEDMODEL_H
#define NEWSFEEDMODEL_H

#include <QAbstractListModel>
#include <QPointer>

#include "newsfeed.h"
#include "utils.h"

namespace Vreen {
class Contact;
} //namespace Vreen

class NewsFeedModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(int count READ count NOTIFY requestFinished)
public:

    enum Roles {
        TypeRole = Qt::UserRole,
        PostIdRole,
        FromRole,
        DateRole,
        BodyRole,
        AttachmentsRole,
        LikesRole,
        RepostsRole,
        CommentsRole,
        OwnerRole
    };

    explicit NewsFeedModel(QObject *parent = 0);
    QObject* client() const;
    void setClient(QObject* arg);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;
    int count() const;
    int findNews(int id);
public slots:
    Vreen::Reply *getNews(int filters = Vreen::NewsFeed::FilterPost | Vreen::NewsFeed::FilterPhoto,
                       quint8 count = 10, int offset = 0);
    void addLike(int index, bool retweet = false, const QString &message = QString());
    void deleteLike(int index);
    void clear();
    void truncate(int count);
signals:
    void clientChanged(QObject* client);
    void requestFinished();
protected:
    void insertNews(int index, const Vreen::NewsItem &data);
    void replaceNews(int index, const Vreen::NewsItem &data);
    inline Vreen::Contact *findContact(int id) const;
private slots:
    void onNewsReceived(const Vreen::NewsItemList &data);
    void onAddLike(const QVariant &response);
    void onDeleteLike(const QVariant &response);
private:
    QPointer<Vreen::Client> m_client;
    QPointer<Vreen::NewsFeed> m_newsFeed;
    Vreen::NewsItemList m_newsList;
    Vreen::Comparator<Vreen::NewsItem, QDateTime> m_newsItemComparator;
};

#endif // NEWSFEEDMODEL_H

