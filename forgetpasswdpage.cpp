#include "forgetpasswdpage.h"
#include "ui_forgetpasswdpage.h"

ForgetPasswdPage::ForgetPasswdPage(MainWindow *win, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForgetPasswdPage)
{  
    mainwindow = win;
    connect(this, &ForgetPasswdPage::mainwindowShowSignal, mainwindow, &MainWindow::mainwindowShowSlot);
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    initUi();

    /*
        重新发送验证码计时器
    */
    m_timer = new QTimer;
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [=]()->void{
        if(i > 0){
            QString t;
            t.setNum(--i);
            t = t + "s";
            qDebug() << t;
            ui->pushButton->setText(t);
        }else if(i == 0){
            i = 60;
            m_timer->stop();
            ui->pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 135, 255);\n"
                                                            "color: rgb(255, 255, 255);\n"
                                                            "border-radius: 27px;\n"
                                                            "border:none;"));
            ui->pushButton->setText("重新发送");
        }
    });

    connect(this, &ForgetPasswdPage::resetPasswdPageShowSignal, mainwindow, &MainWindow::resetPasswdPageShowSlot);
}

void ForgetPasswdPage::initUi()
{
    ui->labelInfo->close();
    ui->labelError->close();
}

ForgetPasswdPage::~ForgetPasswdPage()
{
    delete ui;
}

void ForgetPasswdPage::on_pushButtonBack_clicked()
{
    emit mainwindowShowSignal();
    ui->pushButton->setText("获取验证码");
    ui->labelError->close();
    this->close();
}


void ForgetPasswdPage::on_pushButton_clicked()
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

            QRegularExpression re("^1[3456789]\\d{9}$");
            QRegularExpressionMatch match = re.match(ui->lineEditTel->text(), 0, QRegularExpression::PartialPreferCompleteMatch);

            if(ui->lineEditTel->text() == ""){
                ui->labelError->setText("请先输入手机号码");
                ui->labelError->show();
            }else if(!match.hasMatch()){
                ui->labelError->setText("请输入正确的手机号码");
                ui->labelError->show();
            }else{

                //  发送验证码
                QString err;
                QJsonObject input;
                QByteArray rec;
                input.insert("phone", ui->lineEditTel->text());
                input.insert("scene", "CustomerForgetPassword");
                if(mainwindow->httpRequestSend("http://192.168.0.82:18801/sms/send-code", "POST", input, err, rec)){
                    QJsonDocument doc = QJsonDocument::fromJson(rec);
                    QJsonObject obj = doc.object();
                    if(obj.value("code") == "ok"){
                        qDebug() << "验证码已发送";
                        ui->labelError->close();
                        ui->pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(147, 189, 255);\n"
                                                                        "color: rgb(255, 255, 255);\n"
                                                                        "border-radius: 27px;\n"
                                                                        "border:none;"));
                        ui->lineEditAuthCode->setFocusPolicy(Qt::ClickFocus);
                        ui->pushButton->setText("60s");

                        m_timer->start();

                        QString info;
                        info = "验证码已发送至" + ui->lineEditTel->text().mid(0,3) + "****" + ui->lineEditTel->text().mid(7,4) + "，请输入相应验证码";
                        ui->labelInfo->setText(info);
                        ui->labelInfo->show();

                    }else{
                        qDebug() << "验证码发送失败------" << obj.value("msg");
                    }
                }else{
                    qDebug() << "http request error:" << err;
                }

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


void ForgetPasswdPage::on_lineEditAuthCode_cursorPositionChanged(int arg1, int arg2)
{
    if(arg2 == 6){

        ui->lineEditAuthCode->setFocusPolicy(Qt::NoFocus);
        QString err;
        QByteArray rec;
        QJsonObject input;
        input.insert("phone", ui->lineEditTel->text());
        input.insert("code", ui->lineEditAuthCode->text());
        qDebug() << "Auth code : " << ui->lineEditAuthCode->text();
        if(mainwindow->httpRequestSend("http://192.168.0.82:18801/user/forget-pwd-check-code", "POST", input, err, rec)){
            QJsonDocument doc = QJsonDocument::fromJson(rec);
            QJsonObject obj = doc.object();
            if(obj.value("code") == "ok"){
                qDebug() << "验证码校验成功";
                ui->labelError->close();
                emit resetPasswdPageShowSignal();
                this->close();
            }else{
                qDebug() << "验证码校验失败------" << obj.value("msg");
                ui->labelError->setText("验证码错误");
                ui->labelError->show();
                ui->lineEditAuthCode->setFocusPolicy(Qt::ClickFocus);
            }
        }else{
            qDebug() << "http request error:" << err;
        }
    }
}


void ForgetPasswdPage::on_lineEditTel_textEdited(const QString &arg1)
{
    ui->labelError->hide();
}

