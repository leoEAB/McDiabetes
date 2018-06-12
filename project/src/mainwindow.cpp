#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::itemRowString = "";
QString MainWindow::itemOptionsRowString = "";
QString MainWindow::currentUser = "";
QString MainWindow::orderInfoString = "";
QString MainWindow::selectedUser = ""; //this is the selection of the user's username when you click in the table
QString MainWindow::selectedUserUserName = "";
QString MainWindow::selectedUserFirstName = "";
QString MainWindow::selectedUserLastName = "";
QString MainWindow::selectedUserStreet = "";
QString MainWindow::selectedUserStreetNr = "";
QString MainWindow::selectedUserCity = "";
QString MainWindow::selectedUserPlz = "";
QString MainWindow::selectedUserEmail = "";



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set stacked widgets to invisible at first
    ui->stackedWidgetUser->setHidden(true);
    ui->stackedWidgetAdmin->setHidden(true);

    //initialize database first before calling methods!!!!!!!!!!!!!
    db = new Database();

    //countdownConnect = new QTimer();

    updateTimeLabel();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//"global" functions (signals from other buttons call this func)
void MainWindow::on_buttonLogout_clicked()
{
    //close the connection
    db->closeC();

    //disable ui
    ui->labelConnectDB->clear();
    ui->formFrame_2->setEnabled(false);
    ui->buttonNewUser->setEnabled(false);
    ui->buttonLoginStart->setEnabled(false);

    //return to first page
    ui->stackedWidget->setCurrentIndex(0);

    //hide the widgets & set their indexes to 0
    ui->stackedWidgetUser->setHidden(true);
    ui->stackedWidgetUser->setCurrentIndex(0);

    ui->stackedWidgetAdmin->setHidden(true);
    ui->stackedWidgetAdmin->setCurrentIndex(0);

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
    if(isConnected()){
        ui->labelConnectDB->setText("Connection Successful!");

        //enables login form on next page
        ui->formFrame_2->setEnabled(true);
        //enable the two buttons
        ui->buttonNewUser->setEnabled(true);
        ui->buttonLoginStart->setEnabled(true);

    } else {
        ui->labelConnectDB->setText("Connection Unsuccessful!");

        //disables login form
        ui->formFrame_2->setEnabled(false);
        //disables the two buttons
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

        QMessageBox::critical(
          this,
          tr("McFail"),
          tr("Connection to DB-Server unsuccessful!")
        );
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

        //set current user if login is successful
        currentUser = usernameString;

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
    if(ui->stackedWidgetAdmin->isHidden()){
        ui->stackedWidgetAdmin->setHidden(false);
    }

    ui->stackedWidgetAdmin->setCurrentIndex(0);

    db->listAllItems(ui->tableListAllMenuItems);
}

void MainWindow::on_buttonListOrders_clicked()
{
    if(ui->stackedWidgetAdmin->isHidden()){
        ui->stackedWidgetAdmin->setHidden(false);
    }

    ui->stackedWidgetAdmin->setCurrentIndex(1);

   db->listAllOrders(ui->tableAllOrdersTimestampAdmin);
}

void MainWindow::on_tableAllOrdersTimestampAdmin_clicked(const QModelIndex &index)
{
    int row = index.row();
    QDateTime dateTime = index.sibling(row, 1).data().toDateTime();
    db->showOrderContents(dateTime, ui->tableAllOrdersInfoAdmin);
}

void MainWindow::on_buttonListUsers_clicked()
{
    if(ui->stackedWidgetAdmin->isHidden()){
        ui->stackedWidgetAdmin->setHidden(false);
    }

    ui->stackedWidgetAdmin->setCurrentIndex(2);

    db->listAllUsers(ui->tableAllUsersAdmin);
}

void MainWindow::on_buttonEditSelectedUser_clicked()
{
    selectedUserUserName = db->getStringFromQuery(selectedUser, "userName");
    selectedUserFirstName = db->getStringFromQuery(selectedUser, "firstName");
    selectedUserLastName = db->getStringFromQuery(selectedUser, "lastName");
    selectedUserStreet = db->getStringFromQuery(selectedUser, "street");
    selectedUserStreetNr = db->getStringFromQuery(selectedUser, "streetNr");
    selectedUserCity = db->getStringFromQuery(selectedUser, "city");
    selectedUserPlz = db->getStringFromQuery(selectedUser, "plz");
    selectedUserEmail = db->getStringFromQuery(selectedUser, "email");

    ui->lineUsername_3->setText(selectedUserUserName);
    ui->lineNameFirst_3->setText(selectedUserFirstName);
    ui->lineNameLast_3->setText(selectedUserLastName);
    ui->lineStreetName_3->setText(selectedUserStreet);
    ui->lineStreetNumber_3->setText(selectedUserStreetNr);
    ui->lineCityName_3->setText(selectedUserCity);
    ui->lineCityPLZ_3->setText(selectedUserPlz);
    ui->lineEmail_3->setText(selectedUserEmail);

    ui->stackedWidgetAdmin->setCurrentIndex(3);

}

void MainWindow::on_tableAllUsersAdmin_clicked(const QModelIndex &index)
{
    int row = index.row();
    selectedUser = index.sibling(row, 0).data().toString();
}

void MainWindow::on_buttonSubmitChangesAdmin_clicked()
{

    if(db->updateUserData(   selectedUserUserName,
                             ui->lineNameFirst_3->text(),
                             ui->lineNameLast_3->text(),
                             ui->lineStreetName_3->text(),
                             ui->lineStreetNumber_3->text().toInt(),
                             ui->lineCityName_3->text(),
                             ui->lineCityPLZ_3->text(),
                             ui->lineEmail_3->text()
                         )

        ) {
            QMessageBox::information(
              this,
              tr("McSuccess!"),
              tr("User Info changed!")
            );

            //update the user info fields by pulling directly from DB
            on_buttonEditSelectedUser_clicked();
        } else {

            QMessageBox::critical(
              this,
              tr("McFail"),
              tr("Could not update user info!")
            );
        }

}

void MainWindow::on_buttonCancelChangesAdmin_clicked()
{
    ui->lineUsername_3->setText(selectedUserUserName);
    ui->lineNameFirst_3->setText(selectedUserFirstName);
    ui->lineNameLast_3->setText(selectedUserLastName);
    ui->lineStreetName_3->setText(selectedUserStreet);
    ui->lineStreetNumber_3->setText(selectedUserStreetNr);
    ui->lineCityName_3->setText(selectedUserCity);
    ui->lineCityPLZ_3->setText(selectedUserPlz);
    ui->lineEmail_3->setText(selectedUserEmail);
}

void MainWindow::on_buttonDeleteSelectedUser_clicked()
{
    if(db->deleteUser(selectedUser)){
        QMessageBox::information(
          this,
          tr("McSuccess!"),
          tr("GONE!")
        );

        on_buttonListUsers_clicked();

    } else {
        QMessageBox::critical(
          this,
          tr("McFail"),
          tr("STILL HERE...")
        );
    }
    ;
}
//-------------------------------------------------


//USER SCREEN ----------------------------------

void MainWindow::on_buttonNewOrder_clicked()
{
    //firstly, show the widget!
    ui->stackedWidgetUser->setHidden(false);
    //set the page to the new order page
    ui->stackedWidgetUser->setCurrentIndex(0);
    //delete previous cart items (cart is only temporary, orderInfo is the orders database)
    db->clearCart(currentUser, ui->tableViewCartUser);

    db->listMains(ui->tableViewAllMains);
    db->listSides(ui->tableViewAllSides);
    db->listDrinks(ui->tableViewAllDrinks);
    db->listDesserts(ui->tableViewAllDesserts);

}

void MainWindow::on_buttonShowPrevOrders_clicked()
{
    //firstly, show the frame!
    ui->stackedWidgetUser->setHidden(false);
    //set the page to the all previous orders page
    ui->stackedWidgetUser->setCurrentIndex(2);

    db->listAllOrdersUser(currentUser, ui->tableViewPrevOrderTimestamps);

}

void MainWindow::on_tableViewPrevOrderTimestamps_clicked(const QModelIndex &index)
{
    int row = index.row();
    QDateTime dateTime = index.sibling(row, 0).data().toDateTime();
    db->showOrderContents(dateTime, ui->tableViewPrevOrderInfo);
}

void MainWindow::on_buttonUserInfo_clicked()
{
    selectedUserUserName = db->getStringFromQuery(currentUser, "userName");
    selectedUserFirstName = db->getStringFromQuery(currentUser, "firstName");
    selectedUserLastName = db->getStringFromQuery(currentUser, "lastName");
    selectedUserStreet = db->getStringFromQuery(currentUser, "street");
    selectedUserStreetNr = db->getStringFromQuery(currentUser, "streetNr");
    selectedUserCity = db->getStringFromQuery(currentUser, "city");
    selectedUserPlz = db->getStringFromQuery(currentUser, "plz");
    selectedUserEmail = db->getStringFromQuery(currentUser, "email");

    ui->lineUsername_2->setText(selectedUserUserName);
    ui->lineNameFirst_2->setText(selectedUserFirstName);
    ui->lineNameLast_2->setText(selectedUserLastName);
    ui->lineStreetName_2->setText(selectedUserStreet);
    ui->lineStreetNumber_2->setText(selectedUserStreetNr);
    ui->lineCityName_2->setText(selectedUserCity);
    ui->lineCityPLZ_2->setText(selectedUserPlz);
    ui->lineEmail_2->setText(selectedUserEmail);


    //show the frame!
    ui->stackedWidgetUser->setHidden(false);
    //set the page to the user info page
    ui->stackedWidgetUser->setCurrentIndex(3);

}

//check box which enables editing of the lines in the user info section
void MainWindow::on_checkBoxEditUserInfo_stateChanged(int arg1)
{
    if(ui->checkBoxEditUserInfo->isChecked() || arg1) {
        ui->buttonCancelInfoChangeUser->setEnabled(true);
        ui->buttonSubmitInfoChangeUser->setEnabled(true);
        ui->lineCityName_2->setEnabled(true);
        ui->lineCityPLZ_2->setEnabled(true);
        ui->lineEmail_2->setEnabled(true);
        ui->lineNameFirst_2->setEnabled(true);
        ui->lineNameLast_2->setEnabled(true);
        ui->lineStreetName_2->setEnabled(true);
        ui->lineStreetNumber_2->setEnabled(true);
    } else {
        ui->buttonCancelInfoChangeUser->setEnabled(false);
        ui->buttonSubmitInfoChangeUser->setEnabled(false);
        ui->lineCityName_2->setEnabled(false);
        ui->lineCityPLZ_2->setEnabled(false);
        ui->lineEmail_2->setEnabled(false);
        ui->lineNameFirst_2->setEnabled(false);
        ui->lineNameLast_2->setEnabled(false);
        ui->lineStreetName_2->setEnabled(false);
        ui->lineStreetNumber_2->setEnabled(false);

    }
}

void MainWindow::on_buttonCancelInfoChangeUser_clicked()
{
    ui->lineUsername_2->setText(selectedUserUserName);
    ui->lineNameFirst_2->setText(selectedUserFirstName);
    ui->lineNameLast_2->setText(selectedUserLastName);
    ui->lineStreetName_2->setText(selectedUserStreet);
    ui->lineStreetNumber_2->setText(selectedUserStreetNr);
    ui->lineCityName_2->setText(selectedUserCity);
    ui->lineCityPLZ_2->setText(selectedUserPlz);
    ui->lineEmail_2->setText(selectedUserEmail);
}

//---------------------------------------------------------------------------------


//ORDER PAGE -----------------------------------------------------------------------

//updates the time section in the new order page
void MainWindow::updateTimeLabel(){
    QDateTime currentTime;
    currentTime.setTime(currentTime.currentDateTime().time());

    ui->timeEditSetTime->setTime(currentTime.time());
}

//TO-DO
void MainWindow::on_checkBoxSetTime_stateChanged(int arg1)
{
    if(ui->checkBoxEditUserInfo->isChecked() || arg1) {
        ui->labelSetTime->setEnabled(true);
        ui->timeEditSetTime->setEnabled(true);
        ui->pushButtonSetTime->setEnabled(true);
    } else {
        updateTimeLabel();
        ui->labelSetTime->setEnabled(false);
        ui->timeEditSetTime->setEnabled(false);
        ui->pushButtonSetTime->setEnabled(false);
    }
}

void MainWindow::on_tableViewAllMains_clicked(const QModelIndex &index)
{

    int row = index.row();
    itemRowString = index.sibling(row, 0).data().toString();
    db->showItemOptions(itemRowString, ui->tableViewItemOptions);
}

void MainWindow::on_tableViewAllSides_clicked(const QModelIndex &index)
{

    int row = index.row();
    itemRowString = index.sibling(row, 0).data().toString();
    db->showItemOptions(itemRowString, ui->tableViewItemOptions);
}

void MainWindow::on_tableViewAllDrinks_clicked(const QModelIndex &index)
{

    int row = index.row();
    itemRowString = index.sibling(row, 0).data().toString();
    db->showItemOptions(itemRowString, ui->tableViewItemOptions);
}

void MainWindow::on_tableViewAllDesserts_clicked(const QModelIndex &index)
{

    int row = index.row();

    itemRowString = index.sibling(row, 0).data().toString();

    db->showItemOptions(itemRowString, ui->tableViewItemOptions);
}

void MainWindow::on_tableViewItemOptions_clicked(const QModelIndex &index)
{

    int row = index.row();

    itemOptionsRowString = index.sibling(row, 0).data().toString();

    ui->buttonAddToCart->setEnabled(true);
}

void MainWindow::on_buttonAddToCart_clicked()
{
    if (db->fillCart(currentUser, itemRowString, itemOptionsRowString, ui->tableViewCartUser)){
        ui->tableViewCartUser->update();
        ui->buttonClearCart->setEnabled(true);
        ui->buttonToCheckout->setEnabled(true);
    } else {
        QMessageBox::critical(
          this,
          tr("McFail"),
          tr("adding failed miserably")
        );
    }

}

void MainWindow::on_buttonClearCart_clicked()
{
    db->clearCart(currentUser, ui->tableViewCartUser);
}

void MainWindow::on_buttonToCheckout_clicked()
{
    db->showFinalOrder(currentUser, ui->tableViewOrderSummary);
    ui->stackedWidgetUser->setCurrentIndex(1);
}

void MainWindow::on_buttonCancelOrder_clicked()
{
    ui->stackedWidgetUser->setCurrentIndex(0);
}

void MainWindow::on_buttonConfirmOrder_clicked()
{

    if(db->completeOrder(currentUser)){
        QMessageBox::information(
          this,
          tr("McSuccess"),
          tr("Sit back and relax. Your order is on its way!")
        );

        db->listAllOrdersUser(currentUser, ui->tableViewPrevOrderTimestamps);
        ui->stackedWidgetUser->setCurrentIndex(2);

        //db->clearCart(currentUser, ui->tableViewCartUser);

    } else {
        QMessageBox::critical(
          this,
          tr("McFail"),
          tr("Oh nay! Muh carbs!")
        );
    }

}

//-------------------------------------------------


//NEW USER SCREEN-------------------------------
void MainWindow::clearNewUserInput() {
    ui->lineUsername->clear();
    ui->linePassword->clear();
    ui->lineNameFirst->clear();
    ui->lineNameLast->clear();
    ui->lineEmail->clear();
    ui->lineStreetName->clear();
    ui->lineStreetNumber->clear();
    ui->lineCityName->clear();
    ui->lineCityPLZ->clear();
}

void MainWindow::on_buttonClearUserFields_clicked()
{
    clearNewUserInput();
}

void MainWindow::on_buttonSubmitNewUser_clicked()
{
    QString username = ui->lineUsername->text();
    QString password = ui->linePassword->text();
    QString firstName = ui->lineNameFirst->text();
    QString lastName = ui->lineNameLast->text();
    QString email = ui->lineEmail->text();
    QString street = ui->lineStreetName->text();
    int streetNumber = ui->lineStreetNumber->text().toInt();
    QString city = ui->lineCityName->text();
    QString cityPlz = ui->lineCityPLZ->text();

    if(db->newUser(username, password,
                firstName, lastName,
                email,
                street, streetNumber,
                   city, cityPlz))
    {
        clearNewUserInput();

        ui->buttonToLoginNewUserSuccess->setEnabled(true);

        QMessageBox::information(
          this,
          tr("McSuccess!"),
          tr("User created, proceed to login screen!")
        );

    } else {
        QMessageBox::critical(
          this,
          tr("McFail"),
          tr("Could not create user at the moment")
        );
    }


}

void MainWindow::on_buttonToLoginNewUserSuccess_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//----------------------------------------------


void MainWindow::on_buttonSubmitInfoChangeUser_clicked()
{
    if(db->updateUserData(   currentUser,
                             ui->lineNameFirst_2->text(),
                             ui->lineNameLast_2->text(),
                             ui->lineStreetName_2->text(),
                             ui->lineStreetNumber_2->text().toInt(),
                             ui->lineCityName_2->text(),
                             ui->lineCityPLZ_2->text(),
                             ui->lineEmail_2->text()
                         )

        ) {
            QMessageBox::information(
              this,
              tr("McSuccess!"),
              tr("User Info changed!")
            );

            //update the user info fields by pulling directly from DB
            on_buttonUserInfo_clicked();

        } else {

            QMessageBox::critical(
              this,
              tr("McFail"),
              tr("Could not update user info!")
            );
        }
}
