#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

#include "registercard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString CardIdPreffix = "CardID: ";

    QList<QString> listCardID;
private:
    Ui::MainWindow *ui;
    QRegularExpression rex; //regular
    QSerialPort serial; //COM port
    listUsers* users; //all residents of the house
    RegisterCard* RegCard; //intercom card

private:
    void importPorts(); // load COM port
    void addUser(User* user); //add user to listUsers from register
    void updateLastUsers(QString CardID, QDateTime dateTime); // take the last tenant of the house who came in

    bool findCardIDTable(QString ID); //find user by CardID


private slots:
    void slotConnectComPort(); //Connect to COM port
    void slotGetDataFromCom(); //Get string data from Arduino
    void slotRegisterCardID(); //Register new User
    void slotUpdateTableUsers(); //Update tableUsers when new user was registered
};
#endif // MAINWINDOW_H
