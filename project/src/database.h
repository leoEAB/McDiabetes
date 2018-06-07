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
        bool finalResult = false;

        QSqlQuery queryAdmin;

        queryAdmin.prepare("SELECT isAdmin(:username)");
        queryAdmin.bindValue(":username", username);
        queryAdmin.exec();

        while(queryAdmin.next()) {
            finalResult = queryAdmin.record().value(0).toBool();
        }

        return finalResult;
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
        table->resizeRowsToContents();
        table->resizeColumnToContents(2);
        table->setAlternatingRowColors(true);
    }

    void listAllOrders(QTableView *table) {
        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

                                   //ALL ORDERS
        tableViewModel->setQuery("SELECT piece_name, nn, status FROM main WHERE nn !='0' ORDER BY uid");

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Piece Name");
        tableViewModel->setHeaderData(1, Qt::Horizontal, "NN");
        tableViewModel->setHeaderData(2, Qt::Horizontal, "Status");

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

    void listAllUsers(QTableView *table) {

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

        tableViewModel->setQuery("select UD.userName, email, firstName, lastName, street, streetNr, plz, city from user as UI RIGHT OUTER JOIN person as UD ON (UI.userName = UD.userName)");

        table->setModel(tableViewModel);
        table->resizeColumnToContents(1);
        table->resizeColumnToContents(5);
        table->setAlternatingRowColors(true);
    }

    void listMains(QTableView *table) {
        QSqlQuery queryListMains;
        QString foodCategory = "Main";

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

    bool fillCart(QString itemName, QString itemOption, QTableView *table) {

        QSqlQuery queryAddItem;

        queryAddItem.prepare("INSERT INTO cart(type, name, size, price) SELECT type, name, size, price FROM items where items.name = :itemName and items.size = :itemSize");
        queryAddItem.bindValue(":itemName", itemName);
        queryAddItem.bindValue(":itemSize", itemOption);

        QSqlQuery listCart;
        listCart.prepare("SELECT type, name, size, price FROM cart");

        if(queryAddItem.exec() && listCart.exec()) {
            QSqlQueryModel *tableViewModel = new QSqlQueryModel;
            tableViewModel->setQuery(listCart);

            table->setModel(tableViewModel);
            table->resizeColumnToContents(1);
            table->setAlternatingRowColors(true);

            return true;

        } else {
            return false;
        }

    }

    void showFinalOrder(QTableView *table) {
        QSqlQuery showOrder;

        showOrder.prepare("SELECT name, type, size, price FROM userCart");
        showOrder.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

        tableViewModel->setQuery(showOrder);

        table->setModel(tableViewModel);
        table->resizeColumnToContents(0);
        table->setAlternatingRowColors(false);
    }

    void clearCart(QTableView *table) {
        QSqlQuery setSafeUpdates;

        setSafeUpdates.prepare("SET SQL_SAFE_UPDATES = 0");
        setSafeUpdates.exec();

        QSqlQuery clearCartQuery;
        clearCartQuery.prepare("delete from usercart");
        clearCartQuery.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(clearCartQuery);

        table->setModel(tableViewModel);
    }

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
