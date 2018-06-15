#include "ui_mainwindow.h"

#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QMYSQL");
}

//LOGIN FUNCTIONS -------------------------------------------------------------------------
bool Database::authorizeUser(QString username, QString password) {
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

bool Database::isAdmin(QString username){
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
void Database::listAllOrdersUser(QString username, QTableView *table) {
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

void Database::showOrderContents(QDateTime orderDate, QTableView *table){
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

void Database::listMains(QTableView *table, int hour) {
    QSqlQuery queryListMains;
    QString foodType = "Main";

    //size plays no role at the beginning, cause then the user choses the desired size; thus DISTINCT
    queryListMains.prepare("SELECT DISTINCT name FROM items WHERE items.type = :type AND items.category = :category");
    queryListMains.bindValue(":type", foodType);
    if(hour < 10 && hour >= 6) {
        queryListMains.bindValue(":category", "Breakfast");
    } else {
        queryListMains.bindValue(":category", "Lunch");
    }
    queryListMains.exec();

    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
    tableViewModel->setQuery(queryListMains);

    tableViewModel->setHeaderData(0, Qt::Horizontal, "Name");

    table->setModel(tableViewModel);
    table->resizeColumnsToContents();
    table->setAlternatingRowColors(true);

}

void Database::listSides(QTableView *table, int hour) {
    QSqlQuery queryListSides;
    QString foodType = "Side";

    queryListSides.prepare("SELECT DISTINCT name FROM items WHERE items.type = :type AND items.category = :category");
    queryListSides.bindValue(":type", foodType);
    if(hour < 10 && hour >= 6) {
        queryListSides.bindValue(":category", "Breakfast");
    } else {
        queryListSides.bindValue(":category", "Lunch");
    }
    queryListSides.exec();

    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
    tableViewModel->setQuery(queryListSides);

    table->setModel(tableViewModel);
    table->resizeColumnsToContents();
    table->setAlternatingRowColors(true);

}

void Database::listDrinks(QTableView *table, int hour) {
    QSqlQuery queryListDrinks;
    QString foodType = "Beverage";

    queryListDrinks.prepare("SELECT DISTINCT name FROM items WHERE items.type = :type AND items.category = :category");
    queryListDrinks.bindValue(":type", foodType);
    if(hour < 10 && hour >= 6) {
        queryListDrinks.bindValue(":category", "Breakfast");
    } else {
        queryListDrinks.bindValue(":category", "Lunch");
    }

    queryListDrinks.exec();

    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
    tableViewModel->setQuery(queryListDrinks);

    table->setModel(tableViewModel);
    table->resizeColumnsToContents();
    table->setAlternatingRowColors(true);

}

void Database::listDesserts(QTableView *table, int hour) {
    QSqlQuery queryListDesserts;
    QString foodType = "Dessert";

    queryListDesserts.prepare("SELECT DISTINCT name FROM items WHERE items.type = :type AND items.category = :category");
    queryListDesserts.bindValue(":type", foodType);
    if(hour < 10 && hour >= 6) {
        queryListDesserts.bindValue(":category", "Breakfast");
    } else {
        queryListDesserts.bindValue(":category", "Lunch");
    }
    queryListDesserts.exec();

    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
    tableViewModel->setQuery(queryListDesserts);

    table->setModel(tableViewModel);
    table->resizeColumnsToContents();
    table->setAlternatingRowColors(true);

}

void Database::showItemOptions(QString itemName, QTableView *table) {
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

bool Database::fillCart(QString username, QString itemName, QString itemOption, QTableView *table) {

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

void Database::clearCart(QString username, QTableView *table) {
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

void Database::showFinalOrder(QString username, QTableView *table) {
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

bool Database::completeOrder(QString username) {
    QSqlQuery order;

    order.prepare("CALL completeOrder(:username)");
    order.bindValue(":username", username);

    if(order.exec()){
        return true;
    } else {
        return false;
    }

}
QString Database::getStringFromQuery(QString username, QString what) {
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
void Database::listAllUsers(QTableView *table) {

    QSqlQueryModel *tableViewModel = new QSqlQueryModel;

    //there are users with empty fields!! (admins, test users)
    tableViewModel->setQuery("select UD.userName, email, firstName, lastName, street, streetNr, plz, city from user as UI RIGHT OUTER JOIN person as UD ON (UI.userName = UD.userName)");

    table->setModel(tableViewModel);
    //table->resizeColumnsToContents();
    table->resizeColumnToContents(1);
    table->resizeColumnToContents(5);
    table->resizeColumnToContents(6);
    table->setAlternatingRowColors(true);
}

void Database::listAllItems(QTableView *table) {

    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
                               //ALL MENU ITEMS
    tableViewModel->setQuery("SELECT category, type, name, size, price FROM items ORDER BY name");

    tableViewModel->setHeaderData(0, Qt::Horizontal, "Category");
    tableViewModel->setHeaderData(1, Qt::Horizontal, "Type");
    tableViewModel->setHeaderData(2, Qt::Horizontal, "Name");
    tableViewModel->setHeaderData(3, Qt::Horizontal, "Size");
    tableViewModel->setHeaderData(4, Qt::Horizontal, "price");

    table->setModel(tableViewModel);
    table->resizeColumnToContents(2);
    table->resizeRowsToContents();
    table->setAlternatingRowColors(true);
}

void Database::listAllOrders(QTableView *table) {
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

bool Database::updateUserData(QString userName,
                         QString firstName,
                         QString lastName,
                         QString street,
                         int streetNr,
                         QString city,
                         QString plz,
                         QString email) {

    QSqlQuery updateData;

    updateData.prepare("CALL updateUserInfo(:username, "
                       "                    :firstname,"
                       "                    :lastname,"
                       "                    :street,"
                       "                    :streetNr,"
                       "                    :city,"
                       "                    :plz,"
                       "                    :email)");
    updateData.bindValue(":username", userName);
    updateData.bindValue(":firstname", firstName);
    updateData.bindValue(":lastname", lastName);
    updateData.bindValue(":street", street);
    updateData.bindValue(":streetNr", streetNr);
    updateData.bindValue(":city", city);
    updateData.bindValue(":plz", plz);
    updateData.bindValue(":email", email);

    if(updateData.exec()) {
        return true;
    } else {
        return false;
    }

}

bool Database::deleteUser(QString username) {
    QSqlQuery deleteQuery;

    deleteQuery.prepare("CALL deleteUser(:username)");
    deleteQuery.bindValue(":username", username);

    if(deleteQuery.exec()){
        return true;
    } else {
        return false;
    }
}

bool Database::deleteSelectedMenuItem(QString name, QString size) {
    QSqlQuery deleteQuery;

    deleteQuery.prepare("CALL deleteMenuItem(:name, :size)");
    deleteQuery.bindValue(":name", name);
    deleteQuery.bindValue(":size", size);

    if(deleteQuery.exec()){
        return true;
    } else {
        return false;
    }
}

void Database::addNewItem(QString itemCategory,
                QString itemType,
                QString itemName,
                QString itemSize,
                double itemPrice) {

    QSqlQuery newItemQuery;

    newItemQuery.prepare("CALL addNewItem(:category,"
                         "                :type,"
                         "                :name,"
                         "                :size,"
                         "                :price)");
    newItemQuery.bindValue(":category", itemCategory);
    newItemQuery.bindValue(":type", itemType);
    newItemQuery.bindValue(":name", itemName);
    newItemQuery.bindValue(":size", itemSize);
    newItemQuery.bindValue(":price", itemPrice);

    newItemQuery.exec();

}
//---------------------------------------------------------------------------------------------


//NEW USER PAGE -------------------------------------------------------------------------
bool Database::newUser(QString username, QString password,
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

//-----------------------------------------------------------------

