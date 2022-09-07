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
            ui->labelError->close();
            ui->pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(147, 189, 255);\n"
                                                            "color: rgb(255, 255, 255);\n"
                                                            "border-radius: 27px;\n"
                                                            "border:none;"));
            ui->lineEditAuthCode->setFocusPolicy(Qt::ClickFocus);
            ui->pushButton->setText("60s");

            m_timer->start();
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
    qDebug() << "old pos:" << arg1 << "   new pos:" << arg2;
    if(arg2 == 6){
        emit resetPasswdPageShowSignal();
        this->close();
    }
}

