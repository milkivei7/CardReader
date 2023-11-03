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
    QString FirstName, LastName, Patronymic, CardID;

};


class listUsers : public QObject
{
    Q_OBJECT
public:
    listUsers();
    QList<User*> listRegisterUsers;

public:
    void addUser(User*);
    void printUsers();
signals:
    void listRegisterAdded();
};

#endif // USER_H
