#ifndef MYSTANDARDITEMMODEL_H
#define MYSTANDARDITEMMODEL_H
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QDebug>

class MyStandardItemModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit MyStandardItemModel(QObject *parent = nullptr) : QStandardItemModel(parent) {}

signals:
    void itemTextChanged(const QModelIndex &index, const QString &oldValue, const QString &newValue);

protected:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};
#endif //MYSTANDARDITEMMODEL_H
