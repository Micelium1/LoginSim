#include "userlist.h"
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

UserList::UserList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserList)
    , userList(filename)
{

    ui->setupUi(this);

    QSharedPointer<QJsonArray> userTable = userList.getJsonArray();
    int table_iter = 0;
    for (auto iter_ref:*userTable) {
        ui->UserListTable->insertRow(ui->UserListTable->rowCount());
        ui->UserListTable->setItem(table_iter,0,new QTableWidgetItem(QString(iter_ref.toObject()[User::name].toString())));
        addCheckBoxToTable(ui->UserListTable,table_iter,1,iter_ref.toObject()[User::blocked].toBool());
        addCheckBoxToTable(ui->UserListTable,table_iter,2,iter_ref.toObject()[User::limited].toBool());
        ui->UserListTable->item(table_iter,0)->setFlags(Qt::ItemIsEditable);
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

    QLayout* limitLayout = ui->UserListTable->cellWidget(0, 2)->layout();
    if (limitLayout) {
        QCheckBox* limitBox = qobject_cast<QCheckBox*>(limitLayout->itemAt(0)->widget());
        if (limitBox) {
            limitBox->setEnabled(false);
        } else {
            qDebug() << "Error: limitBox is nullptr!";
        }
    } else {
        qDebug() << "Error: limitLayout is nullptr!";
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
    addCheckBoxToTable(ui->UserListTable,ui->UserListTable->rowCount()-1,2);
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
                userList.removeUser(ui->UserListTable->item(begin,0)->text());
                ui->UserListTable->removeRow(begin);

        }
    }
    }
}
void UserList::on_saveButton_press()
{
    for (int firstRow = 0, lastRow = ui->UserListTable->rowCount(); firstRow < lastRow;++firstRow) {
        bool blockChecked;
        bool limitChecked;
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

        QLayout* limitLayout = ui->UserListTable->cellWidget(firstRow, 2)->layout();
        if (limitLayout) {
            QCheckBox* limitBox = qobject_cast<QCheckBox*>(limitLayout->itemAt(0)->widget());
            if (limitBox) {
                limitChecked = limitBox->isChecked();
            } else {
                qDebug() << "Error: limitBox is nullptr!";
            }
        } else {
            qDebug() << "Error: limitLayout is nullptr!";
        }
        QString text;
        QTableWidgetItem* item = ui->UserListTable->item(firstRow, 0);
        if (item) {
            text = item->text();
        } else {
            qDebug() << "Error: Item is nullptr! Returning an empty string.";
            text = "";
        }
        userList.modifyUser(text,blockChecked,limitChecked);
    }
    userList.save_changes();
    close();
}

UserList::~UserList()
{
    delete ui;
}
