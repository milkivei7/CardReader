#ifndef REGISTERCARD_H
#define REGISTERCARD_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDebug>
#include "user.h"

namespace Ui {
class RegisterCard;
}

class RegisterCard : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterCard(QWidget *parent = nullptr);
    RegisterCard(QWidget *parent = nullptr, QString portName = "");
    ~RegisterCard();

    QSerialPort serialPort;
    User* user;
private:
    Ui::RegisterCard *ui;
    QRegularExpression rex;
public slots:
    void slotGetCardID();
private slots:
    void acceptDialog();
    void rejectDialog();

};

#endif // REGISTERCARD_H
