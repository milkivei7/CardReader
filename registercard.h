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

    QSerialPort serialPort; //COM port for new open
    User* user; // potentiality new user
private:
    Ui::RegisterCard *ui;
    QRegularExpression rex; // regular for delete \n\r
public slots:
    void slotGetCardID(); //get CardID for registered
private slots:
    void acceptDialog(); //override
    void rejectDialog();    //funcstions

};

#endif // REGISTERCARD_H
