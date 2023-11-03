#include "registercard.h"
#include "ui_registercard.h"



RegisterCard::RegisterCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterCard)
{
    ui->setupUi(this);
}

RegisterCard::RegisterCard(QWidget *parent, QString portName):
    QDialog(parent),
    ui(new Ui::RegisterCard)
{
    ui->setupUi(this);
    ui->editCardID->setReadOnly(true);
    rex.setPattern("\\r\\n|\\r|\\n");

    qDebug()<<"Установка Порта "<<portName;
    serialPort.setPort(QSerialPortInfo(portName));
    serialPort.setBaudRate(QSerialPort::Baud9600); // Установите скорость передачи данных
    serialPort.open(QIODevice::ReadWrite); // Откройте последовательный порт

    if(!serialPort.isOpen())
        QMessageBox::warning(this,
                             "Error connect to ComPort",
                             "Не удалось подключиться к Com порту \nОшибка: " + serialPort.errorString(),
                             QMessageBox::Ok);


    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &RegisterCard::acceptDialog);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &RegisterCard::rejectDialog);
    connect(&serialPort,   &QSerialPort::readyRead,     this, &RegisterCard::slotGetCardID);
}

RegisterCard::~RegisterCard()
{
    delete ui;
}

void RegisterCard::slotGetCardID()
{
    QByteArray data = serialPort.readAll();
    QString line = QString::fromStdString(data.toStdString());

    line = line.remove(rex);

    if (!line.isEmpty())
    {
        qDebug()<<"Line "<<line;
        if(line.startsWith("CardID: ")){
            int i = line.indexOf(" ");//Разделитель строки на подстроки
            if (i > 0)
            {
                QString paramString = line.mid(i + 1, line.length());
                ui->editCardID->setText(paramString);
                if(!ui->editCardID->text().isEmpty())
                {
                    ui->editCardID->setEnabled(false);
                    qDebug()<<"Text in editLine "<<ui->editCardID->text();
                }
                else
                {
                    ui->editCardID->setEnabled(true);
                    qDebug()<<"None text in EditLine ";
                }
                return;
                //qDebug()<<"result: "<<paramString;
            }
        }

    }
}

void RegisterCard::acceptDialog()
{
    if(ui->editName->text().isEmpty())
    {
        QMessageBox::warning(this, "ФИО пустое", "Заполните Фамилию Имя и Отчество!",QMessageBox::Ok);
        return;
    }
    else
    {
        QStringList FIO = ui->editName->text().split(" ");
        user = new User(FIO[0],FIO[1],FIO[2], ui->editCardID->text());

        serialPort.close();
        RegisterCard::accept();
    }
}

void RegisterCard::rejectDialog()
{
    serialPort.close();
    RegisterCard::reject();
}

