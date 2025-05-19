#include "mystandarditemmodel.h"


bool MyStandardItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        // 获取旧值
        QString oldValue = this->data(index).toString();

        QString newText = value.toString();
        // 调用基类的 setData 方法更新数据
        bool result = QStandardItemModel::setData(index, value, role);
        if(result && oldValue != newText)
        {
            // 如果更新成功，则发出信号
            emit itemTextChanged(index, oldValue, value.toString());
            qDebug()<<"setData信号发出";
        }
        return result;
    }
    return QStandardItemModel::setData(index, value, role);
}
