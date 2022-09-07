#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUi();

    qDebug() << "add Wiget ..." ;
    widgetLoadingPage = new LoadingPage(this);
    widgetForgetPasswdPage = new ForgetPasswdPage(this);
    widgetRegistPage = new RegistPage(this);
    widgetResetPasswdPage = new ResetPasswdPage(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi()
{

    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setAttribute(Qt::WA_WState_WindowOpacitySet);
    //setWindowOpacity(0.95);

    ui->labelError->hide();

}

void MainWindow::on_pushButtonLogin_clicked()
{
    qDebug() << "on_pushButtonLogin_clicked";
    if(ui->lineEditId->text().isEmpty())
    {
        ui->whiteBlockWidget_2->setGeometry(QRect(768, 500+27, 384, 54));
        ui->checkBoxRemeberPasswd->setGeometry(QRect(817, 570+27, 92, 23));
        ui->checkBoxAutoLogin->setGeometry(QRect(817, 602+27, 92, 23));
        ui->pushButtonForgetPasswd->setGeometry(QRect(1065, 570+27, 82, 26));
        ui->pushButtonRegist->setGeometry(QRect(1065, 602+27, 80, 26));
        ui->pushButtonLogin->setGeometry(QRect(928, 660+27, 64, 64));

        ui->labelError->setGeometry(QRect(788, 485, 96, 26));
        ui->labelError->setText("请输入云针ID");
        ui->labelError->show();

    }else if(ui->lineEditPasswd->text().isEmpty())
    {
        ui->checkBoxRemeberPasswd->setGeometry(QRect(817, 570+27, 92, 23));
        ui->checkBoxAutoLogin->setGeometry(QRect(817, 602+27, 92, 23));
        ui->pushButtonForgetPasswd->setGeometry(QRect(1065, 570+27, 82, 26));
        ui->pushButtonRegist->setGeometry(QRect(1065, 602+27, 80, 26));
        ui->pushButtonLogin->setGeometry(QRect(928, 660+27, 64, 64));

        ui->labelError->setGeometry(QRect(788, 555, 80, 26));
        ui->labelError->setText("请输入密码");
        ui->labelError->show();

    }else{
        this->close();
        widgetLoadingPage->show();
    }

}

void MainWindow::mainwindowShowSlot()
{
    qDebug() << "mainwindow Show Slot ..." ;
    this->show();
}

void MainWindow::on_lineEditId_textEdited(const QString &arg1)
{
    ui->whiteBlockWidget_2->setGeometry(QRect(768, 500, 384, 54));
    ui->checkBoxRemeberPasswd->setGeometry(QRect(817, 570, 92, 23));
    ui->checkBoxAutoLogin->setGeometry(QRect(817, 602, 92, 23));
    ui->pushButtonForgetPasswd->setGeometry(QRect(1065, 570, 82, 26));
    ui->pushButtonRegist->setGeometry(QRect(1065, 602, 80, 26));
    ui->pushButtonLogin->setGeometry(QRect(928, 660, 64, 64));
    ui->labelError->hide();
}


void MainWindow::on_lineEditPasswd_textEdited(const QString &arg1)
{
    ui->checkBoxRemeberPasswd->setGeometry(QRect(817, 570, 92, 23));
    ui->checkBoxAutoLogin->setGeometry(QRect(817, 602, 92, 23));
    ui->pushButtonForgetPasswd->setGeometry(QRect(1065, 570, 82, 26));
    ui->pushButtonRegist->setGeometry(QRect(1065, 602, 80, 26));
    ui->pushButtonLogin->setGeometry(QRect(928, 660, 64, 64));
    ui->whiteBlockWidget_2->setGeometry(QRect(768, 500, 384, 54));
    ui->labelError->hide();
}


void MainWindow::on_pushButtonForgetPasswd_clicked()
{
    this->close();
    widgetForgetPasswdPage->show();
}


void MainWindow::on_checkBoxRemeberPasswd_stateChanged(int arg1)
{
    if(arg1 == Qt::Unchecked)
    {
        if(ui->checkBoxAutoLogin->isChecked())
        {
            ui->checkBoxAutoLogin->setCheckState(Qt::Unchecked);
        }
    }
}


void MainWindow::on_checkBoxAutoLogin_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->checkBoxRemeberPasswd->setChecked(1);
    }
}


void MainWindow::on_pushButtonRegist_clicked()
{
    this->close();
    widgetRegistPage->show();
}

void MainWindow::resetPasswdPageShowSlot(){
    widgetResetPasswdPage->show();
}

