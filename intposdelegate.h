#ifndef INTPOSDELEGATE_H
#define INTPOSDELEGATE_H

#include <QStyledItemDelegate>

class IntPosDelegate : public QStyledItemDelegate
{
public:
    explicit IntPosDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // INTPOSDELEGATE_H
