#ifndef USERLIST_H
#define USERLIST_H

#include <QDialog>
#include "usertojson.h"

namespace Ui {
class UserList;
}

class UserList : public QDialog
{
    Q_OBJECT

public:
    explicit UserList(QSharedPointer<UserToJson> _userList, QWidget *parent = nullptr);
    void on_addButton_press();
    void on_deleteButton_press();
    void on_saveButton_press();
    ~UserList();

private:
    QSharedPointer<UserToJson> userList;
    Ui::UserList *ui;
};

#endif // USERLIST_H
