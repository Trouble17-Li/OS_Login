#include "loadingpage.h"
#include "ui_loadingpage.h"

LoadingPage::LoadingPage(MainWindow *win, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingPage)
{
    mainwindow = win;
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);

    QMovie *loadingMovie = new QMovie(":/images/cat.gif");
    ui->labelLoading->setScaledContents(true);
    ui->labelLoading->setMovie(loadingMovie);
    loadingMovie->start();
}

LoadingPage::~LoadingPage()
{
    delete ui;
}
