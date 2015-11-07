#include "qthenable.h"

QThenable::QThenable(QObject *parent) :
    QObject(parent)
{
}

void QThenable::fulfill(const QVariant &_res)
{
    if (resolve_) {
        resolve_(_res);
    }
}

void QThenable::then(std::function<void (QVariant)> _f)
{
    resolve_ = _f;
}
