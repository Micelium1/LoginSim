#ifndef PASSWORDCHANGEWINDOW_H
#define PASSWORDCHANGEWINDOW_H

#include <QDialog>
#include "usertojson.h"

namespace Ui {
class PasswordChangeWindow;
}

class PasswordChangeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordChangeWindow(QString _name, QSharedPointer<UserToJson> _userList, QWidget *parent = nullptr);
    void on_applyButton_press();
    ~PasswordChangeWindow();

private:
    QString name;
    QSharedPointer<UserToJson> userList;
    Ui::PasswordChangeWindow *ui;
};

#endif // PASSWORDCHANGEWINDOW_H
