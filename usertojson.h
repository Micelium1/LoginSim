#ifndef USERTOJSON_H
#define USERTOJSON_H
#include <QtCore>

extern QString filename;//по заданию нужно было сохранять всё в файл на машине симулирующий базу, думаю глобальная переменная всё же лучше как идея чем прописывать название файла ручками каждый раз

namespace User {

    const QString name = "Username";
    const QString passwd = "Password";
    const QString blocked = "Blocked";
    const QString limited = "Limited";
};
enum PasswordChangeCodes {
    Okay = 0,
    WrongPassword = 1,
    ViolatedLimitations = 2
};
class UserToJson
{
public:
    UserToJson(QString filename);
    void addNewUser(QString name, bool blocked, int limited);
    void modifyUser(QString name,bool blocked,int limited);
    void removeUser(QString name);
    QJsonObject getUser(QString name);
    PasswordChangeCodes changePassword(QString name, QString old_passwd, QString new_passwd);
    void save_changes();
    QSharedPointer<QJsonArray> getJsonArray() const;

private:
    QFile JsonFile;
    QJsonObject JsonArray;
};

#endif // USERTOJSON_H
