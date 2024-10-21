#include "encryptorpassdialog.h"
#include "ui_encryptorpassdialog.h"

EncryptorPassDialog::EncryptorPassDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EncryptorPassDialog)
{
    ui->setupUi(this);
    connect(ui->AcceptButton,&QPushButton::clicked,this,&EncryptorPassDialog::on_acceptButton_press);
}
void EncryptorPassDialog::on_acceptButton_press()
{
    keyPass = ui->keyPassInput->text();
    this->close();
}

QString EncryptorPassDialog::getKeyPass()
{
    return keyPass;
}

EncryptorPassDialog::~EncryptorPassDialog()
{
    delete ui;
}


