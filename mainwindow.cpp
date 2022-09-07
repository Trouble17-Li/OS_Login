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
        ui->labelError->setText("请输入云针ID");
        ui->labelError->show();

    }else if(ui->lineEditPasswd->text().isEmpty())
    {
        ui->labelError->setText("请输入密码");
        ui->labelError->show();

    }else{

        QJsonObject input;
        input.insert("username", ui->lineEditId->text());
        input.insert("password", ui->lineEditPasswd->text());
        QString err;
        QByteArray recData;
        if(httpRequestSend("http://192.168.0.82:18801/user/access-token", "POST", input, err, recData))
        {
            QJsonDocument doc = QJsonDocument::fromJson(recData);
            QJsonObject obj = doc.object();
            QJsonValue value = obj.value("msg");
            QJsonValue code = obj.value("code");
            if(code.toString() != "ok"){
                qDebug() << "msg:" << value.toString();
                ui->labelError->setText(value.toString());
                ui->labelError->show();
            }else{
                QJsonDocument doc = QJsonDocument::fromJson(recData);
                QJsonObject obj = doc.object();
                QJsonObject data = obj.value("data").toObject();
                token = data.value("token").toString();
                qDebug() << "token :" << token;
                this->close();
                widgetLoadingPage->show();
            }
        }else{
            qDebug() << "http reply error:" << err;
        }
    }

}

void MainWindow::mainwindowShowSlot()
{
    qDebug() << "mainwindow Show Slot ..." ;
    this->show();
}

void MainWindow::on_lineEditId_textEdited(const QString &arg1)
{
    ui->labelError->hide();
}


void MainWindow::on_lineEditPasswd_textEdited(const QString &arg1)
{
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

int MainWindow::httpRequestSend(QString url, QString thod, QJsonObject input, QString &message, QByteArray &recData){
    qDebug()<<"HttpThread  :  "<<QThread::currentThread();
    QNetworkAccessManager* netAccessManager= new QNetworkAccessManager;
    QString reqUrlStr = url;
    QUrl reqUrl =reqUrlStr;

    QJsonDocument doc = QJsonDocument::fromVariant(QVariant(input.toVariantMap()));
    QByteArray postData = doc.toJson();
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json; charset=UTF-8");
    request.setUrl(reqUrl);

    QNetworkReply *reply;
    if(thod == "POST"){
        reply =  netAccessManager->post(request,postData);
    }else{
        reply =  netAccessManager->get(request);
    }

    /* 等待请求返回结果 */
    QEventLoop loop;
    connect(netAccessManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));//finish为manager自带的信号，replyFinished是自定义的
    loop.exec();

    int res = 1;
    if (reply->error() == QNetworkReply::NoError){
        recData = reply->readAll();
        qDebug()<<"req success:" << recData;
    }else {
        message = reply->error();
        qDebug()<<"req error :" << message;
        res = -1;
    }

    /* 释放 reply 内存, 否则会造成内存泄漏 */
    reply->deleteLater();
    reply=NULL;
    return res;

}

