#ifndef DATABASE_H
#define DATABASE_H

//mySql includes
#include <QtSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

//show tables
#include <QTableView>

//throw messageBox if login is unsuccessful
#include <QMessageBox>

class Database
{
public:
    Database() {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }

    ~Database();

    //funcNameC "C" means - Custom; Qtsql has same function names
    void setHostnameC(QString host) {
        db.setHostName(host);
    }

    void setPortC(QString port) {
        db.setPort(port.toInt());
    }

    void setUserNameC(QString username) {
        db.setUserName(username);
    }

    void setPasswordC(QString password) {
        db.setPassword(password);
    }

    void setDatabaseNameC(QString databaseName) {
        db.setDatabaseName(databaseName);
    }

    bool openC() {
        return db.open();
    }

    void closeC() {
        db.close();
    }

//LOGIN FUNCTIONS -------------------------------------------------------------------------
    bool authorizeUser(QString username, QString password) {
            //by default you are not authorized
            bool authorized = false;

            QSqlQuery queryLogin;
            queryLogin.prepare("SELECT authorize(:user, :userPassword)");
            queryLogin.bindValue(":user", username);
            queryLogin.bindValue(":userPassword", password);
            queryLogin.exec();

            while(queryLogin.next()) {
                authorized = queryLogin.record().value(0).toBool();
            }

            return authorized;

    }

    bool isAdmin(QString username){
        //prove me wrong
        bool isAdmin = false;

        QSqlQuery queryAdmin;

        //call function isAdmin
        queryAdmin.prepare("SELECT isAdmin(:username)");
        queryAdmin.bindValue(":username", username);
        queryAdmin.exec();

        //position on 0th row
        while(queryAdmin.next()) {
            //the value at column 0, converted to boolean
            isAdmin = queryAdmin.record().value(0).toBool();
        }

        return isAdmin;
    }

//------------------------------------------------------------------------------------------



//USER FUNCTIONS ---------------------------------------------------------------------
    void listAllOrdersUser(QString username, QTableView *table) {
        QSqlQuery queryOrderInfo;

        queryOrderInfo.prepare("SELECT orderDate AS Date, total AS Total,"
                               "        firstName AS `Ordered by`,"
                               "        lastName AS `Family Name`,"
                               "        email AS Email,"
                               "        street AS Street,"
                               "        streetNr AS `No.`,"
                               "        plz AS PLZ,"
                               "        city AS City FROM orderInfo AS O WHERE O.userName = :username");
        queryOrderInfo.bindValue(":username", username);
        queryOrderInfo.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryOrderInfo);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void showOrderContents(QDateTime orderDate, QTableView *table){
        QSqlQuery queryOrderContents;

        queryOrderContents.prepare("SELECT type AS Type, name AS Name, size AS Size, price AS Price FROM orderContents WHERE orderContents.orderDate = :date");
        queryOrderContents.bindValue(":date", orderDate);
        queryOrderContents.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryOrderContents);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

    void listMains(QTableView *table) {
        QSqlQuery queryListMains;
        QString foodCategory = "Main";

        //size plays no role at the beginning, cause then the user choses the desired size; thus DISTINCT
        queryListMains.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListMains.bindValue(":category", foodCategory);
        queryListMains.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListMains);

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Name");

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void listSides(QTableView *table) {
        QSqlQuery queryListSides;
        QString foodCategory = "Side";

        queryListSides.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListSides.bindValue(":category", foodCategory);
        queryListSides.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListSides);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void listDrinks(QTableView *table) {
        QSqlQuery queryListDrinks;
        QString foodCategory = "Beverage";

        queryListDrinks.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListDrinks.bindValue(":category", foodCategory);
        queryListDrinks.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListDrinks);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void listDesserts(QTableView *table) {
        QSqlQuery queryListDesserts;
        QString foodCategory = "Dessert";

        queryListDesserts.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListDesserts.bindValue(":category", foodCategory);
        queryListDesserts.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListDesserts);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void showItemOptions(QString itemName, QTableView *table) {
        QSqlQuery queryItemOptions;

        queryItemOptions.prepare("SELECT size, price FROM items WHERE items.name = :itemName");
        queryItemOptions.bindValue(":itemName", itemName);
        queryItemOptions.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryItemOptions);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

    bool fillCart(QString username, QString itemName, QString itemOption, QTableView *table) {

        //create a query that calls a procedure which fills the cart table
        QSqlQuery queryAddItem;
        queryAddItem.prepare("CALL addToCart(:username, :itemName, :itemSize)");
        queryAddItem.bindValue(":username", username);
        queryAddItem.bindValue(":itemName", itemName);
        queryAddItem.bindValue(":itemSize", itemOption);

        //this query is only to list the cart items ONLY for the current user
        QSqlQuery listCart;
        listCart.prepare("SELECT type, name, size, price FROM cart WHERE cart.userName = :username");
        listCart.bindValue(":username", username);

        //define a table view model which fills up the QTableView
        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

        table->setModel(tableViewModel);
        table->resizeColumnToContents(1);
        table->setAlternatingRowColors(true);

        //.exec() functions return true if query is successful
        if(queryAddItem.exec() && listCart.exec()) {

            //SetQuery resets the model and updates the data
            tableViewModel->setQuery(listCart);

            //scroll to bottom, obviously
            table->scrollToBottom();

            return true;
        } else {
            return false;
        }

    }

    void clearCart(QString username, QTableView *table) {
        QSqlQuery setSafeUpdates;

        //
        setSafeUpdates.prepare("SET SQL_SAFE_UPDATES = 0");
        setSafeUpdates.exec();

        QSqlQuery clearCartQuery;
        clearCartQuery.prepare("DELETE FROM cart WHERE cart.userName = :username");
        clearCartQuery.bindValue(":username", username);
        clearCartQuery.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(clearCartQuery);

        table->setModel(tableViewModel);
    }

    void showFinalOrder(QString username, QTableView *table) {
        QSqlQuery showOrder;

        showOrder.prepare("SELECT type, name, size, price FROM cart WHERE cart.userName = :username");
        showOrder.bindValue(":username", username);
        showOrder.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

        tableViewModel->setQuery(showOrder);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->resizeColumnToContents(1);
        table->setAlternatingRowColors(false);
    }

    bool completeOrder(QString username) {
        QSqlQuery order;

        order.prepare("CALL completeOrder(:username)");
        order.bindValue(":username", username);

        if(order.exec()){
            return true;
        } else {
            return false;
        }

    }

    QString getStringFromQuery(QString username, QString what) {
        QString result;

        QSqlQuery query;

        query.prepare("SELECT getUserData(:username, :what)");
        query.bindValue(":username", username);
        query.bindValue(":what", what);
        query.exec();

        while(query.next()) {
            result = query.record().value(0).toString();
        }

        return result;


    }
//---------------------------------------------------------------------------------------------

//ADMIN FUNCTIONALITIES -----------------------------------------------------------------------
    void listAllUsers(QTableView *table) {

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

        //there are users with empty fields!! (admins, test users)
        tableViewModel->setQuery("select UD.userName, email, firstName, lastName, street, streetNr, plz, city from user as UI RIGHT OUTER JOIN person as UD ON (UI.userName = UD.userName)");

        table->setModel(tableViewModel);
        table->resizeColumnToContents(1);
        table->resizeColumnToContents(5);
        table->setAlternatingRowColors(true);
    }

    void listAllItems(QTableView *table) {

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
                                   //ALL MENU ITEMS
        tableViewModel->setQuery("SELECT category, type, name, size, price FROM items ORDER BY name");

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Category");
        tableViewModel->setHeaderData(1, Qt::Horizontal, "Type");
        tableViewModel->setHeaderData(2, Qt::Horizontal, "Name");
        tableViewModel->setHeaderData(3, Qt::Horizontal, "Size");
        tableViewModel->setHeaderData(4, Qt::Horizontal, "price");

        table->setModel(tableViewModel);
        table->resizeRowsToContents();
        table->resizeColumnToContents(2);
        table->setAlternatingRowColors(true);
    }

    void listAllOrders(QTableView *table) {
        QSqlQuery queryOrders;

        queryOrders.prepare("SELECT  userName AS Username,"
                               "        orderDate AS Date, "
                               "        total AS Total,"
                               "        firstName AS `Ordered by`,"
                               "        lastName AS `Family Name`,"
                               "        email AS Email,"
                               "        street AS Street,"
                               "        streetNr AS `No.`,"
                               "        plz AS PLZ,"
                               "        city AS City FROM orderInfo AS O");
        queryOrders.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryOrders);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

//---------------------------------------------------------------------------------------------

//NEW USER PAGE -------------------------------------------------------------------------
    bool newUser(QString username, QString password,
                 QString firstName, QString lastName,
                 QString email, QString street,
                 int streetNumber, QString city,
                 QString cityPlz) {

        QSqlQuery newUser;
        newUser.prepare("CALL addUser(:username, :password, :firstname, :lastname, :email, :street, :streetNr, :plz, :city)");
        newUser.bindValue(":username", username);
        newUser.bindValue(":password", password);
        newUser.bindValue(":firstname", firstName);
        newUser.bindValue(":lastname", lastName);
        newUser.bindValue(":email", email);
        newUser.bindValue(":street", street);
        newUser.bindValue(":streetNr", streetNumber);
        newUser.bindValue(":plz", cityPlz);
        newUser.bindValue(":city", city);

        if(newUser.exec()) {
            return true;
        } else {
            return false;
        }

    }


private:

    //create Database
    QSqlDatabase db;

};

#endif // DATABASE_H
