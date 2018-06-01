#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
#include <QSplashScreen>

#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("C:/Users/Micha/Dropbox/HTW/SoSe18/DB/Projekt_DB/qt/ProjektTest/Logo/logo.png"));
    splash->show();

    MainWindow w;

    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));

    return a.exec();
}
