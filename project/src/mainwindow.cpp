#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set newOrderFrame to invisible at first
    ui->stackedWidgetUser->setHidden(true);

    //initialize database first before calling methods!!!!!!!!!!!!!
    db = new Database();

    //countdownConnect = new QTimer();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//"global" functions (signals from other buttons call this func)
void MainWindow::on_buttonLogout_clicked()
{
    /*  maybe this is too radical,
     *  maybe it should return the user only to the login screen
     *  instead of closing the database connection as well
     *
    */


    //close the connection
    db->closeC();

    //disable ui
    ui->labelConnectDB->clear();
    ui->formFrame_2->setEnabled(false);
    ui->buttonNewUser->setEnabled(false);
    ui->buttonLoginStart->setEnabled(false);

    //return to first page
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_buttonPrevious_clicked()
{
    int pageIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(pageIndex - 1);
    ui->stackedWidget->update();

}
//----------------------------------------------


//PAGE DB INFO & CONNECT -----------------------
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked() || arg1) {
        ui->buttonConnectDB->setEnabled(true);
        ui->lineDatenbankName->setEnabled(true);
        ui->lineServerDB->setEnabled(true);
        ui->linePortDB->setEnabled(true);
        ui->lineUserDB->setEnabled(true);
        ui->linePassDB->setEnabled(true);
    } else {
        ui->buttonConnectDB->setEnabled(false);
        ui->lineDatenbankName->setEnabled(false);
        ui->lineServerDB->setEnabled(false);
        ui->linePortDB->setEnabled(false);
        ui->lineUserDB->setEnabled(false);
        ui->linePassDB->setEnabled(false);
    }

}

void MainWindow::on_buttonConnectDB_clicked()
{
    if(MainWindow::isConnected()){
        ui->labelConnectDB->setText("Connection Successful!");

        //enables login form on next page
        ui->formFrame_2->setEnabled(true);
        ui->buttonNewUser->setEnabled(true);
        ui->buttonLoginStart->setEnabled(true);

    } else {
        ui->labelConnectDB->setText("Connection Unsuccessful!");

        //disables login form
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

void MainWindow::on_buttonLoginStart_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_buttonNewUser_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
//----------------------------------------------


//LOGIN SCREEN----------------------------------
void MainWindow::on_buttonLogin_clicked()
{
    QString usernameString = ui->lineUser->text(); //read from username field
    QString passwordString = ui->linePass->text(); //read from password field

    if (db->authorizeUser(usernameString, passwordString)) {

        if(db->isAdmin(usernameString)){
             ui->stackedWidget->setCurrentIndex(2); //admin menu
        } else {
            ui->stackedWidget->setCurrentIndex(3); //user menu
        }

    } else {
        QMessageBox::critical(
          this,
          tr("McFail"),
          tr("Username or Password wrong")
        );
    };

    ui->linePass->clear();
    ui->lineUser->clear();
}
//----------------------------------------------


//ADMIN SCREEN ---------------------------------
void MainWindow::on_buttonListAllMenu_clicked()
{
    if(ui->tableListAll->isHidden()){
        ui->tableListAll->show();
    }

    db->listAllItems(ui->tableListAll);
}

void MainWindow::on_buttonListOrders_clicked()
{
    if(ui->tableListAll->isHidden()){
        ui->tableListAll->show();
    }

    db->listAllOrders(ui->tableListAll);
}

void MainWindow::on_buttonListUsers_clicked()
{
    if(ui->tableListAll->isHidden()){
        ui->tableListAll->show();
    }

    db->listAllUsers(ui->tableListAll);
}
//-------------------------------------------------


//USER SCREEN ----------------------------------
void MainWindow::on_buttonNewOrder_clicked()
{
    //firstly, show the widget!
    ui->stackedWidgetUser->setHidden(false);
    //set the page to the new order page
    ui->stackedWidgetUser->setCurrentIndex(0);
}

void MainWindow::on_buttonShowPrevOrders_clicked()
{
    //firstly, show the frame!
    ui->stackedWidgetUser->setHidden(false);
    //set the page to the all previous orders page
    ui->stackedWidgetUser->setCurrentIndex(1);
}
//-------------------------------------------------


//NEW USER SCREEN-------------------------------
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

void MainWindow::on_buttonSubmitNewUser_clicked()
{

}
//----------------------------------------------
