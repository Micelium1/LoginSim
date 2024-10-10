#include "userlist.h"
#include "intposdelegate.h"
#include "ui_userlist.h"
#include <QCheckBox>
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"

void addCheckBoxToTable(QTableWidget* table, int row, int column,bool checked = false) {
    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(checked);

    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);

    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);

    layout->setContentsMargins(0, 0, 0, 0);

    table->setCellWidget(row, column, container);
}

UserList::UserList(QSharedPointer<UserToJson> _userList,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserList)
    , userList(_userList)
{

    ui->setupUi(this);

    QSharedPointer<QJsonArray> userTable = userList->getJsonArray();
    int table_iter = 0;
    for (auto iter_ref:*userTable) {
        ui->UserListTable->insertRow(ui->UserListTable->rowCount());

        ui->UserListTable->setItem(table_iter,0,new QTableWidgetItem(QString(iter_ref.toObject()[User::name].toString())));
        ui->UserListTable->item(table_iter,0)->setFlags(Qt::ItemIsEditable);

        addCheckBoxToTable(ui->UserListTable,table_iter,1,iter_ref.toObject()[User::blocked].toBool());

        ui->UserListTable->setItem(table_iter,2,new QTableWidgetItem(QString::number(iter_ref.toObject()[User::limited].toInt())));
        ui->UserListTable->setItemDelegateForColumn(2,new IntPosDelegate);
        ++table_iter;
    }

    QLayout* blocklayout = ui->UserListTable->cellWidget(0, 1)->layout();
    if (blocklayout) {
        QCheckBox* blockBox = qobject_cast<QCheckBox*>(blocklayout->itemAt(0)->widget());
        if (blockBox) {
            blockBox->setEnabled(false);
        } else {
            qDebug() << "Error: blockBox is nullptr!";
        }
    } else {
        qDebug() << "Error: layout is nullptr!";
    }



    connect(ui->cancelButton,&QPushButton::clicked,this,&QWidget::close);
    connect(ui->addButton,&QPushButton::clicked,this,&UserList::on_addButton_press);
    connect(ui->deleteButton,&QPushButton::clicked,this,&UserList::on_deleteButton_press);
    connect(ui->saveButton,&QPushButton::clicked,this,&UserList::on_saveButton_press);
}



void UserList::on_addButton_press()
{
    ui->UserListTable->insertRow(ui->UserListTable->rowCount());

    addCheckBoxToTable(ui->UserListTable,ui->UserListTable->rowCount()-1,1);
}
void UserList::on_deleteButton_press()
{
    QList<QTableWidgetSelectionRange> delete_ranges_list = ui->UserListTable->selectedRanges();
    if (!delete_ranges_list.empty()) {
    for (QTableWidgetSelectionRange& delete_range:delete_ranges_list) {
            for (int begin = delete_range.topRow(), end = delete_range.bottomRow();begin <= end;++begin) {
                if (begin == 0) {
                    break;
                }
                userList->removeUser(ui->UserListTable->item(begin,0)->text());
                ui->UserListTable->removeRow(begin);

        }
    }
    }
}
void UserList::on_saveButton_press()
{
    for (int firstRow = 0, lastRow = ui->UserListTable->rowCount(); firstRow < lastRow;++firstRow) {
        bool blockChecked = false;

        QLayout* blocklayout = ui->UserListTable->cellWidget(firstRow, 1)->layout();
        if (blocklayout) {
            QCheckBox* blockBox = qobject_cast<QCheckBox*>(blocklayout->itemAt(0)->widget());
            if (blockBox) {
                blockChecked = blockBox->isChecked();
            } else {
                qDebug() << "Error: blockBox is nullptr!";
            }
        } else {
            qDebug() << "Error: layout is nullptr!";
        }


        QString name;
        QTableWidgetItem* item_name = ui->UserListTable->item(firstRow, 0);
        if (item_name) {
            name = item_name->text();
        } else {
            qDebug() << "Error: Item is nullptr! Returning an empty string.";
            name = "";
        }

        QString limit;
        QTableWidgetItem* item_limit = ui->UserListTable->item(firstRow, 2);
        if (item_limit) {
            limit = item_limit->text();
        } else {
            qDebug() << "Error: Item is nullptr! Returning an empty string.";
            limit = "0";
        }

        userList->modifyUser(name,blockChecked,limit.toInt());
    }
    userList->save_changes();
    close();
}

UserList::~UserList()
{
    delete ui;
}
