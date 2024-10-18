#include "passwordchangewindow.h"
#include "ui_passwordchangewindow.h"

PasswordChangeWindow::PasswordChangeWindow(QString _name,QSharedPointer<UserToJson> _userList, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswordChangeWindow)
    , name(_name)
    , userList(_userList)

{
    ui->setupUi(this);
    ui->limitationNotify->setText(QString("<i>Минимальная длина вашего пароля: %1</i>").arg(userList->getUser(name)[User::limited].toInt()));
    connect(ui->applyButton,&QPushButton::clicked,this,&PasswordChangeWindow::on_applyButton_press);
    connect(ui->leaveButton,&QPushButton::clicked,this,&QWidget::close);
}

void PasswordChangeWindow::on_applyButton_press()
{

    if(ui->newPasswdInput->text() != ui->RepeatPasswdInput->text())
    {
        ui->changeStatus->setText("Пароль и повтор пароля не совпадают");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::red);
        ui->changeStatus->setPalette(pal);
        return;
    }

    PasswordChangeCodes isSuccesfull = userList->changePassword(name,ui->oldPasswdInput->text(),ui->newPasswdInput->text());


    if (isSuccesfull == PasswordChangeCodes::Okay) {
        ui->changeStatus->setText("Пароль успешно изменён");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::green);
        ui->changeStatus->setPalette(pal);
        userList->save_changes();
    }
    else if (isSuccesfull == PasswordChangeCodes::WrongPassword) {
        ui->changeStatus->setText("Неверный пароль");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::red);
        ui->changeStatus->setPalette(pal);
    }
    else if (isSuccesfull == PasswordChangeCodes::ViolatedLimitations) {
        ui->changeStatus->setText("Пароль не соответствует минимальной длинне");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::red);
        ui->changeStatus->setPalette(pal);
    }
}

PasswordChangeWindow::~PasswordChangeWindow()
{
    delete ui;
}
