#ifndef NEWSFEEDMODEL_H
#define NEWSFEEDMODEL_H

#include <QAbstractListModel>
#include <QJsonObject>

enum class NewsFeedRoles {
    Post = Qt::UserRole + 1,
    CopyHistory,
    Comments,
    Likes,
    Reposts,
    Attachments,
    Photos,
    PhotoTags,
    Notes,
    Friends
};

class NewsFeedModel : public QAbstractListModel
{
    Q_OBJECT

    QList<QJsonObject> posts_;
public:
    explicit NewsFeedModel(QObject *parent = 0);

    Q_INVOKABLE void    getLastNews();

    virtual QHash<int, QByteArray> roleNames() const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

signals:

public slots:

};


#endif // NEWSFEEDMODEL_H
