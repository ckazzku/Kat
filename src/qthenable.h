#ifndef QTHENABLE_H
#define QTHENABLE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

class QThenable : public QObject
{
    Q_OBJECT

    std::function<void(QVariant)> resolve_;

    explicit QThenable(QObject *parent = 0);
public:    
    inline static QSharedPointer<QThenable> create() {
        return QSharedPointer<QThenable>(new QThenable());
    }

    void then(std::function<void(QVariant)> _f);
    void fulfill(const QVariant& _res);

signals:    

public slots:

};

#endif // QTHENABLE_H
