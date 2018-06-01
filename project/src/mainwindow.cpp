#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize database first before calling methods!!!!!!!!!!!!!
    db = new Database();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonLoginStart_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_buttonLogout_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_buttonLogout_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_buttonLogin_clicked()
{
    QString usernameString = ui->lineUser->text(); //read from username field
    QString passwordString = ui->linePass->text(); //read from password field

    if (db->authorizeUser(usernameString, passwordString)) {
        if(!(db->isAdmin(usernameString))){ //always admin when isadmin returns false //FIX
             ui->stackedWidget->setCurrentIndex(2);
        } else {
            ui->stackedWidget->setCurrentIndex(3);
        }

    };
}

void MainWindow::on_buttonConnectDB_clicked()
{
    if(MainWindow::isConnected()){
        ui->labelConnectDB->setText("Connection Successful!");
        ui->formFrame_2->setEnabled(true);
        ui->buttonNewUser->setEnabled(true);
        ui->buttonLoginStart->setEnabled(true);
    } else {
        ui->labelConnectDB->setText("Connection Unsuccessful!");
        ui->formFrame_2->setEnabled(false);
        ui->buttonNewUser->setEnabled(false);
        ui->buttonLoginStart->setEnabled(false);
    }
}

bool MainWindow::isConnected() {

    try {
        db->setDatabaseNameC(ui->lineDatenbankName->text());
        db->setHostnameC(ui->lineServerDB->text());
        db->setPortC(ui->linePortDB->text());
        db->setUserNameC(ui->lineUserDB->text());
        db->setPasswordC(ui->linePassDB->text());

        bool ok = db->openC();
        if(ok) {
            return true;
        } else {
            return false;
        }

    } catch (...) {

        QMessageBox messageBox;
        messageBox.critical(0,"Error","An error has occured !");
        messageBox.setFixedSize(500,200);

        return false;
    }
}

void MainWindow::on_buttonNewUser_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked() || arg1) {
        ui->lineDatenbankName->setEnabled(true);
        ui->lineServerDB->setEnabled(true);
        ui->linePortDB->setEnabled(true);
        ui->lineUserDB->setEnabled(true);
        ui->linePassDB->setEnabled(true);
    } else {
        ui->lineDatenbankName->setEnabled(false);
        ui->lineServerDB->setEnabled(false);
        ui->linePortDB->setEnabled(false);
        ui->lineUserDB->setEnabled(false);
        ui->linePassDB->setEnabled(false);
    }

}

void MainWindow::on_buttonClearUserFields_clicked()
{
    ui->lineNameFirst->clear();
    ui->lineNameLast->clear();
    ui->lineEmail->clear();
    ui->lineStreetName->clear();
    ui->lineStreetNumber->clear();
    ui->lineCityName->clear();
    ui->lineCityPLZ->clear();

}

/*
void MainWindow::on_buttonPrevious_clicked()
{
    int pageIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageIndex - 1);
    ui->stackedWidget->update();
}
*/
void MainWindow::on_buttonListAll_clicked()
{

}

void MainWindow::on_buttonListOrders_clicked()
{

}

void MainWindow::on_buttonListUsers_clicked()
{

}

void MainWindow::on_buttonNewOrder_clicked()
{

}

void MainWindow::on_buttonShowPrevOrders_clicked()
{

}

void MainWindow::on_buttonSubmitNewUser_clicked()
{

}
