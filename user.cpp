#include "user.h"
#include <QDebug>

User::User(QObject *parent)
    : QObject{parent}
{

}

User::User(QString _FirstName, QString _LastName, QString _Patronymic, QString _CardID)
{
    qDebug()<<"\n----------------------------------------------------------------"
            <<"\n\tCreate User"
            <<"\n----------------------------------------------------------------\n\n\n";

    this->FirstName  = _FirstName;
    this->LastName   = _LastName;
    this->Patronymic = _Patronymic;
    this->CardID     = _CardID;
}

listUsers::listUsers()
{

}

void listUsers::addUser(User *user)
{
    listRegisterUsers.append(user);
    emit listRegisterAdded();
}

void listUsers::printUsers()
{
    qDebug()<<"\n---------\n-Print Users";
    for(auto const i : listRegisterUsers)
    {
        qDebug()<<i->FirstName<< " "<<i->LastName<<" "<<i->Patronymic<<" "<< i->CardID;
    }
}
