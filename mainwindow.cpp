#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "loggedwindow.h"

MainWindow::MainWindow(QSharedPointer<UserToJson> userList, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->wrongPasswdLabel->hide();
    QPalette pal = ui->wrongPasswdLabel->palette();
    pal.setColor(QPalette::WindowText,Qt::red);
    ui->wrongPasswdLabel->setPalette(pal);
    connect(ui->enterButton,&QPushButton::clicked,this,[userList, this]{MainWindow::on_EnterButton_press(userList);});
    connect(ui->exitButton,&QPushButton::clicked,this,&QWidget::close);
}

void MainWindow::on_EnterButton_press(QSharedPointer<UserToJson> userList)
{
    QJsonObject userInfo = userList->getUser(ui->loginEdit->text());
    if (userInfo.empty()) {
        ui->passwordEdit->setText("");
        ui->wrongPasswdLabel->show();
        return;
    }
    if (!(userInfo[User::passwd] == ui->passwordEdit->text())) {
        ui->passwordEdit->setText("");
        ui->wrongPasswdLabel->setText("Неверное имя пользователя или пароль");
        ui->wrongPasswdLabel->show();
        return;
    }
    // if (!(userInfo[User::blocked].toBool())) {
    //     ui->wrongPasswdLabel->setText("Пользователь заблокирован");
    //     ui->wrongPasswdLabel->show();
    //     return;
    // }
    this->hide();
    LoggedWindow* w = new LoggedWindow(ui->loginEdit->text(),userList,this);
    w->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}
