#ifndef MYSTANDARDiTEMmODEL_H
#define MYSTANDARDiTEMmODEL_H
#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include <QDebug>

class MyStandardItemModel : public QStandardItemModel {
    Q_OBJECT

public:
    explicit MyStandardItemModel(QObject *parent = nullptr)
        : QStandardItemModel(parent) {}

signals:
    void itemTextChanged(const QModelIndex &index, const QString &oldText, const QString &newText);

public:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};
#endif
