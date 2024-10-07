#include "intposdelegate.h"
#include "qlineedit.h"

IntPosDelegate::IntPosDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *IntPosDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit* line = new QLineEdit(parent);
    line->setValidator(new QIntValidator(0,INT_MAX,line));
    return line;
}
