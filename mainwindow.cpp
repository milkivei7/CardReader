#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user.h"
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug()<<"___________________________________"
            <<"\n\t-Load mainWindow"
            <<"\n___________________________________\n\n\n";
    ui->setupUi(this);
    rex.setPattern("\\r\\n|\\r|\\n");
    //list with all users
    users = new listUsers();

    // Load available Ports and add to comboBox with ports
    importPorts();

    //connect
    connect(ui->butConnect,  &QPushButton::clicked, this, &MainWindow::slotConnectComPort);
    connect(ui->butRegister, &QPushButton::clicked, this, &MainWindow::slotRegisterCardID);


}

MainWindow::~MainWindow()
{
    delete ui;
}
// ..................Methods.................
//-------------------------------------------
// Import ports from usb
void MainWindow::importPorts()
{
    qDebug()<<"-------------"
            <<"\n-importPorts";

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : serialPortInfos)
    {
        qDebug()<< "Port:" << portInfo.portName() << "\n";
        ui->cbComPorts->addItem(portInfo.portName());

    }

    if(ui->cbComPorts->currentText().isEmpty())
        qWarning()<<"\n-----COM PORT IS EMPTY!----\n";


    qDebug()<<"\n-importPorts - Complete"
            << "\n-------------\n\n\n";

}
//-----------------------------------------------
// add new User
void MainWindow::addUser(User* user)
{
    qDebug()<<"-------------"
            <<"\n-addUser";
        int row = ui->tableUsersWidget->rowCount();
        ui->tableUsersWidget->insertRow(ui->tableUsersWidget->rowCount());
        ui->tableUsersWidget->setRowCount(row+1);

        QTableWidgetItem *userItem;
        userItem = new QTableWidgetItem(user->FirstName);
        ui->tableUsersWidget->setItem(row, 0, userItem);
        userItem = new QTableWidgetItem(user->LastName);
        ui->tableUsersWidget->setItem(row, 1, userItem);
        userItem = new QTableWidgetItem(user->Patronymic);
        ui->tableUsersWidget->setItem(row, 2, userItem);
        userItem = new QTableWidgetItem(user->CardID);
        ui->tableUsersWidget->setItem(row, 3, userItem);

        users->addUser(user);

        qDebug()<<"\nFirst Name: "<<user->FirstName
                <<"\nLast Name:  "<<user->LastName
                <<"\nPatronymic: "<<user->Patronymic
                << "\nCardID:    "<<user->CardID;

        qDebug()<<"\n-addUser - Complete"
                << "\n-------------\n\n\n";

}
//-----------------------------------------------
// check last openned users
void MainWindow::updateLastUsers(QString CardID, QDateTime dateTime)
{
    qDebug()<<"-------------"
            <<"\n-updateLastUsers"
            <<"\n-CardId: "<< CardID
            <<"\n-Date: "<<dateTime;

    int rows = ui->tableUsersWidget->rowCount();
    QString fullName = "";

    for(int i = 0; i < rows; ++i)
    {
        if(ui->tableUsersWidget->item(i, 3)->text() == CardID)
        {
            //we have found our value so we can update 'i' row
            fullName = ui->tableUsersWidget->item(i,0)->text() +" "
                       +ui->tableUsersWidget->item(i,1)->text()+" "
                       +ui->tableUsersWidget->item(i,2)->text();
            break;
        }
    }
    if(!fullName.isEmpty())
    {
        int row = ui->lastUserTable->rowCount();
        ui->lastUserTable->insertRow(ui->lastUserTable->rowCount());
        ui->lastUserTable->setRowCount(row+1);

        QTableWidgetItem *userItem;
        userItem = new QTableWidgetItem(fullName);
        ui->lastUserTable->setItem(row, 0, userItem);
        userItem = new QTableWidgetItem(dateTime.date().toString("dd.MM.yyyy"));
        ui->lastUserTable->setItem(row, 1, userItem);
        userItem = new QTableWidgetItem(dateTime.time().toString("hh:mm"));
        ui->lastUserTable->setItem(row, 2, userItem);

    }
    qDebug()
            <<"\n-updateLastUsers - Complete"
            <<"\n-------------\n\n\n";

}
//_______________________________________________
// find cardId in UsersTable
bool MainWindow::findCardIDTable(QString ID)
{
    qDebug()<<"-------------"
            <<"\n-findCardIDTable"
            <<"\n-CardId: "<< ID;

    int rows = ui->tableUsersWidget->rowCount();
    bool found = false;
    for(int i = 0; i < rows; ++i)
    {
        if(ui->tableUsersWidget->item(i, 3)->text() == ID)
        {
            //we have found our value so we can update 'i' row
            found = true;
            break;
        }
    }
    qDebug()<<"\n-isFind?: "
            <<found
            <<"\n-findCardIDTable - Complete"
            <<"\n-------------\n\n\n";
    return found;
}
//_______________________________________________



// ..................Slots.......................
//-----------------------------------------------
// Connect to ports
void MainWindow::slotConnectComPort()
{
    qDebug()<<"-------------"
            <<"\n-slotConnectComPort";

    // ---Settings SerialPort
    serial.setPort(QSerialPortInfo(ui->cbComPorts->currentText())); //Set name Port
    serial.setBaudRate(QSerialPort::Baud9600); // Set speed data
    serial.open(QIODevice::ReadWrite); // Open Port
    if(!serial.isOpen())
    {
         QMessageBox::warning(this, "Error Connect", "Error connect to COM Port" + serial.errorString(),QMessageBox::Ok);
         qWarning()<<"\nqWarning(): "<<"Error connect to COM Port:\n"<<serial.errorString();
         return;
    }
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::slotGetDataFromCom);

    ui->cbComPorts->setEnabled(false);
    ui->butConnect->setText(tr("Disconnect"));
    qDebug()<<"\n+Complete"
            <<"\n-------------\n\n\n";
}


//-----------------------------------------------
// Get data from Com port
void MainWindow::slotGetDataFromCom()
{
    qDebug()<<"-------------"
            <<"\n-slotGetDataFromCom";
    QByteArray data = serial.readAll();
    QString line = QString::fromStdString(data.toStdString());

    line = line.remove(rex);

    if (!line.isEmpty())
    {
        //qDebug()<<line;
        if(line.startsWith(MainWindow::CardIdPreffix)){
            int i = line.indexOf(" ");//Разделитель строки на подстроки
            if (i > 0)
            {
                QString paramString = line.mid(i + 1, line.length());
                listCardID.append(paramString);
                if(findCardIDTable(paramString))
                {
                    qDebug()<<"+CardId found";
                    ui->statusLabel->setText("Открыто!");
                    updateLastUsers(paramString, QDateTime::currentDateTime());
                    QTimer::singleShot(3000,this,[=]{ui->statusLabel->setText("Закрыто");});
                }

                qDebug()<<"-Arduino Message: "<<paramString;
            }
        }
    }
    qDebug()<<"\n+slotGetDataFromCom - Complete"
            <<"\n-------------\n\n\n";
}
//-----------------------------------------------
// Register CardID User
void MainWindow::slotRegisterCardID()
{
    qDebug()<<"-------------"
            <<"\n-slotRegisterCardID";
    if(!serial.isOpen())
        QMessageBox::warning(this, "Error Connect", "Error connect to COM Port" + serial.errorString(),QMessageBox::Ok);
    else
    {
        serial.close();
        RegCard = new RegisterCard(this, ui->cbComPorts->currentText());
        RegCard->show();
        connect(RegCard, &RegisterCard::finished, this, [this]()
        {
            if(RegCard->user)
            {
                addUser(RegCard->user);
                qDebug()<<"RegCard has user \n-printUsers";
                users->printUsers();
            }
            else
                qWarning()<<"\n---user in RegCard == nullptr!";
            this->slotConnectComPort();
        }
        );
        //RegCard->serialPort.setPort(QSerialPortInfo(ui->cbComPorts->currentText()));
    }
    qDebug()<<"\n-slotRegisterCardID - Complete"
            <<"\n-------------\n\n\n";

    // RegCard->exec();
}

void MainWindow::slotUpdateTableUsers()
{
    //ui->tableUsersWidget->;


}
//-----------------------------------------------

