#ifndef USERTOJSON_H
#define USERTOJSON_H
#include <QtCore>

extern QString filename;//по заданию нужно было сохранять всё в файл на машине симулирующий базу, думаю глобальная переменная всё же лучше как идея чем прописывать название файла ручками каждый раз

struct User {
    // Объявление статических переменных
    const static QString name;
    const static QString passwd;
    const static QString blocked;
    const static QString limited;
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
    int indexFind(QString name) const;
    PasswordChangeCodes changePassword(QString name, QString old_passwd, QString new_passwd);
    std::optional<QJsonValueRef> find(QString name);
    void save_changes();
    QSharedPointer<QJsonArray> getJsonArray() const;

private:
    QFile JsonFile;
    QJsonArray JsonArray;
};

#endif // USERTOJSON_H
