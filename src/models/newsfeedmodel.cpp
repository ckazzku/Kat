#include "newsfeedmodel.h"
#include "../api/apirequest.h"

NewsFeedModel::NewsFeedModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void NewsFeedModel::getLastNews()
{
    // TODO: pass parameters to call
    ApiRequest::instance()->call("newsgeed.get", QJsonObject());
}

QHash<int, QByteArray> NewsFeedModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[int(NewsFeedRoles::Attachments)] = "attachments";
    roles[int(NewsFeedRoles::Comments)] = "comments";
    roles[int(NewsFeedRoles::CopyHistory)] = "copy_history";
    roles[int(NewsFeedRoles::Friends)] = "friends";
    roles[int(NewsFeedRoles::Likes)] = "likes";
    roles[int(NewsFeedRoles::Notes)] = "notes";
    roles[int(NewsFeedRoles::Photos)] = "photos";
    roles[int(NewsFeedRoles::PhotoTags)] = "photo_tags";
    roles[int(NewsFeedRoles::Post)] = "post";
    roles[int(NewsFeedRoles::Reposts)] = "reposts";

    return roles;
}

int NewsFeedModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant NewsFeedModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
        if (index.row() >= 0 && index.row() < posts_.count()){
            switch (NewsFeedRoles(role)){
                case NewsFeedRoles::Post:
                    result = QVariant(this->posts_.at(index.row()));
                    break;
            }
        }
        return result;
}
