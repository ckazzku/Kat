# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-kat

QT += dbus sql

CONFIG += link_pkgconfig sailfishapp c++11
PKGCONFIG += mlite5

HEADERS += \
    src/filedownloader.h \
    src/notificationhelper.h \
    src/api/apirequest.h \
    src/storage.h \
    src/api/photos.h \
    src/coreclient.h \
    src/simpleconnection.h \
    src/models/buddymodel.h \
    src/models/chatmodel.h \
    src/models/dialogsmodel.h \
    src/models/newsfeedmodel.h

SOURCES += \
    src/harbour-kat.cpp \
    src/filedownloader.cpp \
    src/notificationhelper.cpp \
    src/api/apirequest.cpp \
    src/storage.cpp \
    src/api/photos.cpp \
    src/coreclient.cpp \
    src/simpleconnection.cpp \
    src/models/buddymodel.cpp \
    src/models/chatmodel.cpp \
    src/models/dialogsmodel.cpp \
    src/models/newsfeedmodel.cpp

OTHER_FILES += \
    qml/harbour-kat.qml \
    qml/cover/*.qml \
    qml/pages/*.qml \
    qml/views/*.qml \
    qml/js/*.js \
    qml/js/api/*.js \
    qml/emojione/*.js \
    qml/emojione/png/*.png \
    qml/fonts/OpenSansEmoji.ttf \
    translations/*.ts \
    rpm/harbour-kat.yaml \
    rpm/harbour-kat.spec \
    rpm/harbour-kat.changes.in \
    LICENSE \
    README.md \
    .gitignore \
    qml/pages/NewWallPostPage.qml \
    qml/js/api/groups.js \
    qml/js/types.js \
    qml/js/api/account.js \
    qml/js/signals.js

CONFIG += sailfishapp_i18n
TRANSLATIONS += \
    translations/harbour-kat-en.ts

# using vreen

VKIT_SRC_ROOT_DIR = 3rdparty/vreen/src

INCLUDEPATH += \
    $$VKIT_SRC_ROOT_DIR/api \
    $$VKIT_SRC_ROOT_DIR/qml/src

SOURCES += \
    $$VKIT_SRC_ROOT_DIR/api/abstractlistmodel.cpp \
    $$VKIT_SRC_ROOT_DIR/api/attachment.cpp \
    $$VKIT_SRC_ROOT_DIR/api/audio.cpp \
    $$VKIT_SRC_ROOT_DIR/api/audioitem.cpp \
    $$VKIT_SRC_ROOT_DIR/api/chatsession.cpp \
    $$VKIT_SRC_ROOT_DIR/api/client.cpp \
    $$VKIT_SRC_ROOT_DIR/api/commentssession.cpp \
    $$VKIT_SRC_ROOT_DIR/api/connection.cpp \
    $$VKIT_SRC_ROOT_DIR/api/contact.cpp \
    $$VKIT_SRC_ROOT_DIR/api/contentdownloader.cpp \
    $$VKIT_SRC_ROOT_DIR/api/dynamicpropertydata.cpp \
    $$VKIT_SRC_ROOT_DIR/api/friendrequest.cpp \
    $$VKIT_SRC_ROOT_DIR/api/groupchatsession.cpp \
    $$VKIT_SRC_ROOT_DIR/api/groupmanager.cpp \
    $$VKIT_SRC_ROOT_DIR/api/json.cpp \
    $$VKIT_SRC_ROOT_DIR/api/localstorage.cpp \
    $$VKIT_SRC_ROOT_DIR/api/longpoll.cpp \
    $$VKIT_SRC_ROOT_DIR/api/message.cpp \
    $$VKIT_SRC_ROOT_DIR/api/messagemodel.cpp \
    $$VKIT_SRC_ROOT_DIR/api/messagesession.cpp \
    $$VKIT_SRC_ROOT_DIR/api/newsfeed.cpp \
    $$VKIT_SRC_ROOT_DIR/api/newsitem.cpp \
    $$VKIT_SRC_ROOT_DIR/api/pollitem.cpp \
    $$VKIT_SRC_ROOT_DIR/api/pollprovider.cpp \
    $$VKIT_SRC_ROOT_DIR/api/reply.cpp \
    $$VKIT_SRC_ROOT_DIR/api/roster.cpp \
    $$VKIT_SRC_ROOT_DIR/api/utils.cpp \
    $$VKIT_SRC_ROOT_DIR/api/wallpost.cpp \
    $$VKIT_SRC_ROOT_DIR/api/wallsession.cpp

HEADERS += \
    $$VKIT_SRC_ROOT_DIR/api/abstractlistmodel.h \
    $$VKIT_SRC_ROOT_DIR/api/attachment.h \
    $$VKIT_SRC_ROOT_DIR/api/audio.h \
    $$VKIT_SRC_ROOT_DIR/api/audioitem.h \
    $$VKIT_SRC_ROOT_DIR/api/chatsession.h \
    $$VKIT_SRC_ROOT_DIR/api/client.h \
    $$VKIT_SRC_ROOT_DIR/api/client_p.h \
    $$VKIT_SRC_ROOT_DIR/api/commentssession.h \
    $$VKIT_SRC_ROOT_DIR/api/connection.h \
    $$VKIT_SRC_ROOT_DIR/api/connection_p.h \
    $$VKIT_SRC_ROOT_DIR/api/contact.h \
    $$VKIT_SRC_ROOT_DIR/api/contact_p.h \
    $$VKIT_SRC_ROOT_DIR/api/contentdownloader.h \
    $$VKIT_SRC_ROOT_DIR/api/contentdownloader_p.h \
    $$VKIT_SRC_ROOT_DIR/api/dynamicpropertydata_p.h \
    $$VKIT_SRC_ROOT_DIR/api/friendrequest.h \
    $$VKIT_SRC_ROOT_DIR/api/groupchatsession.h \
    $$VKIT_SRC_ROOT_DIR/api/groupmanager.h \
    $$VKIT_SRC_ROOT_DIR/api/groupmanager_p.h \
    $$VKIT_SRC_ROOT_DIR/api/json.h \
    $$VKIT_SRC_ROOT_DIR/api/localstorage.h \
    $$VKIT_SRC_ROOT_DIR/api/longpoll.h \
    $$VKIT_SRC_ROOT_DIR/api/longpoll_p.h \
    $$VKIT_SRC_ROOT_DIR/api/message.h \
    $$VKIT_SRC_ROOT_DIR/api/messagemodel.h \
    $$VKIT_SRC_ROOT_DIR/api/messagesession.h \
    $$VKIT_SRC_ROOT_DIR/api/messagesession_p.h \
    $$VKIT_SRC_ROOT_DIR/api/newsfeed.h \
    $$VKIT_SRC_ROOT_DIR/api/newsitem.h \
    $$VKIT_SRC_ROOT_DIR/api/pollitem.h \
    $$VKIT_SRC_ROOT_DIR/api/pollprovider.h \
    $$VKIT_SRC_ROOT_DIR/api/reply.h \
    $$VKIT_SRC_ROOT_DIR/api/reply_p.h \
    $$VKIT_SRC_ROOT_DIR/api/roster.h \
    $$VKIT_SRC_ROOT_DIR/api/roster_p.h \
    $$VKIT_SRC_ROOT_DIR/api/utils.h \
    $$VKIT_SRC_ROOT_DIR/api/utils_p.h \
    $$VKIT_SRC_ROOT_DIR/api/vk_global.h \
    $$VKIT_SRC_ROOT_DIR/api/wallpost.h \
    $$VKIT_SRC_ROOT_DIR/api/wallsession.h
