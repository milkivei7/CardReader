#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    User(QString _FirstName = "", QString _LastName ="", QString _Patronymic="", QString _CardID="" );

public:
    //Info User
    QString FirstName, LastName, Patronymic, CardID;

};

//List all users at home
class listUsers : public QObject
{
    Q_OBJECT
public:
    listUsers();
    QList<User*> listRegisterUsers;

public:
    void addUser(User*); // add user to list and emit signal????
    void printUsers(); // print all users
signals:
    void listRegisterAdded(); // emit signal when add new user????
};

#endif // USER_H
