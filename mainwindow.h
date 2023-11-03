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
    QRegularExpression rex;
    QSerialPort serial;
    listUsers* users;
    RegisterCard* RegCard;

private:
    void importPorts();
    void addUser(User* user);
    void updateLastUsers(QString CardID, QDateTime dateTime);

    bool findCardIDTable(QString ID);


private slots:
    void slotConnectComPort();
    void slotGetDataFromCom();
    void slotRegisterCardID();
    void slotUpdateTableUsers();
};
#endif // MAINWINDOW_H
