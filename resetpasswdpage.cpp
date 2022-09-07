#include "resetpasswdpage.h"
#include "ui_resetpasswdpage.h"

ResetPasswdPage::ResetPasswdPage(MainWindow *win, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetPasswdPage)
{
    mainwindow = win;
    ui->setupUi(this);

    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    connect(this, &ResetPasswdPage::mainwindowShowSignal, mainwindow, &MainWindow::mainwindowShowSlot);

    ui->labelError->close();
}

ResetPasswdPage::~ResetPasswdPage()
{
    delete ui;
}


void ResetPasswdPage::on_lineEditNewPasswd_editingFinished()
{
    if(ui->lineEditNewPasswd->text().length()<6){
        ui->labelError->setText("密码长度小于6位");
        ui->labelError->show();
    }
}


void ResetPasswdPage::on_lineEditNewPasswd_textEdited(const QString &arg1)
{
    ui->labelError->close();
}


void ResetPasswdPage::on_pushButtonOk_clicked()
{

    /*
         检查网络
    */
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://www.baidu.com/"));
    //QNetworkReply *reply = manager.get(request);
    manager.get(request);
    connect(&manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply)->void{
        qDebug() << "QNetworkAccessManager::finished" ;
        if(reply->error() == QNetworkReply::NoError)
        {
            qDebug() << "network is no problem";
            if((ui->lineEditNewPasswd->text() == "") || (ui->lineEditPasswd->text() == "")){
                if(ui->lineEditNewPasswd->text() == ""){
                    ui->labelError->setText("请输入新密码");
                }else{
                    ui->labelError->setText("请输入确认密码");
                }
                  ui->labelError->show();

            }else if(ui->lineEditNewPasswd->text() != ui->lineEditPasswd->text()){
                ui->labelError->setText("两次输入不一致,请检查");
                ui->labelError->show();
            }else if((ui->lineEditNewPasswd->text().length()<6) || (ui->lineEditPasswd->text().length()<6)){
                ui->labelError->setText("密码长度小于6位");
                ui->labelError->show();
            }else{
                emit mainwindowShowSignal();
                this->close();
            }

        }else{
            qDebug() << "no network";
            ui->labelError->setText("请先检查您的网络");
            ui->labelError->show();
        }
    });

    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();

}


void ResetPasswdPage::on_lineEditPasswd_textEdited(const QString &arg1)
{
    ui->labelError->close();
}


void ResetPasswdPage::on_pushButtonBack_clicked()
{
    emit mainwindowShowSignal();
    this->close();
}

