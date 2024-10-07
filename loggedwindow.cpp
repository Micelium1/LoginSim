#include "loggedwindow.h"
#include "./ui_loggedwindow.h"
#include "passwordchangewindow.h"
#include "userlist.h"

LoggedWindow::LoggedWindow(QString name, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoggedWindow)
    , parentWindow(parent)
{
    ui->setupUi(this);

    ui->greetingText->setText(QString("<h2>Приветствую %1</h2>").arg(name));
    if (name != "admin") {
        ui->userListButton->hide();
    }
    connect(ui->userListButton,&QPushButton::clicked,this,&LoggedWindow::on_userListButton_press);
    connect(ui->changePasswordButton,&QPushButton::clicked,this,[name,this]{LoggedWindow::on_changePasswordButton_press(name);});
    connect(ui->exitButton,&QPushButton::clicked,this,&QWidget::close);
}

void LoggedWindow::on_userListButton_press()
{
    UserList* wind = new UserList;
    wind->exec();
}
void LoggedWindow::closeEvent(QCloseEvent *event) {
    if (parentWindow) {
        parentWindow->show();
    }
    QMainWindow::closeEvent(event);
}

void LoggedWindow::on_changePasswordButton_press(QString name)
{
    PasswordChangeWindow* wind = new PasswordChangeWindow(name);
    wind->exec();
}

