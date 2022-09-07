#include "registpage.h"
#include "ui_registpage.h"

RegistPage::RegistPage(MainWindow *win, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistPage)
{
    mainwindow = win;
    connect(this, &RegistPage::mainwindowShowSignal, mainwindow, &MainWindow::mainwindowShowSlot);
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    ui->labelError->hide();

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

    if(!readJsonFile())
    {
        qDebug() << "get area jason error";
        return;
    }

    SetItemsProvince();
}

RegistPage::~RegistPage()
{
    delete ui;
}

void RegistPage::on_pushButtonBack_clicked()
{
    emit mainwindowShowSignal();
    this->close();
}


void RegistPage::on_pushButton_clicked()
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
                ui->labelError->setText("手机号码格式不正确");
                ui->labelError->show();
            }else{

                //  发送验证码
                QString err;
                QJsonObject input;
                QByteArray rec;
                input.insert("phone", ui->lineEditTel->text());
                input.insert("scene", "CustomerRegister");
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

int RegistPage::readJsonFile()
{
//    QFile file("./LocalList.json");
//    if(!file.open(QFile::ReadOnly))
//    {
//        qDebug() << "file: " << file.fileName() << " read error: " << file.error();
//        return -1;
//    }
//    data = file.readAll();
//    file.close();
    QJsonObject obj;
    QString err;
    QByteArray rec;
    qDebug() << "远程获取地区json";
    if(mainwindow->httpRequestSend("http://192.168.0.82:18801/area", "GET", obj, err, rec)){

        data = rec;
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
        if(jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << "json error!" << jsonError.errorString();
            return -1;
        }

        QJsonObject jsonobj = doc.object();
        arrayA = jsonobj.value("data").toArray();

        return 1;
    }else{
        qDebug() << "http request error :" << err;
        return -1;
    }

}

void RegistPage::SetItemsProvince()
{
    QStringList ProvinceList;
    for(int i = 0; i<arrayA.count(); i++)
    {
        QString Province = arrayA.at(i).toObject().value("name").toString();
        ProvinceList.append(Province);
        qDebug() << "Province: " << Province;

    }
    ui->comboBox->addItems(ProvinceList);
}

void RegistPage::SetItemsCity()
{
    QStringList CityList;

    for(int i = 0; i<arrayB.count(); i++)
    {
        QString Crovince = arrayB.at(i).toObject().value("name").toString();
        CityList.append(Crovince);
        qDebug() << "Crovince: " << Crovince;
    }
    ui->comboBox_2->addItems(CityList);
}


void RegistPage::SetItemsArea()
{
    QStringList AreaList;

    for(int i = 0; i<arrayC.count(); i++)
    {
        QString Arovince = arrayC.at(i).toObject().value("name").toString();
        AreaList.append(Arovince);
        qDebug() << "Arovince: " << Arovince;
    }

    ui->comboBox_3->addItems(AreaList);
}


void RegistPage::on_comboBox_currentTextChanged(const QString &arg1)
{
    qDebug() << "arg1: " << arg1;
    int i = 0;
    for(i = 0; i<arrayA.count(); i++){
        QString name = arrayA.at(i).toObject().value("name").toString();
        qDebug() << "name :" << name;
        if(name == arg1){
            qDebug() << "---province match---";
            int count = arrayB.count();
            qDebug() << "arrayB count : " << count;
            for(int k = count; k>=0; k--){
                ui->comboBox_2->removeItem(k);
            }
            arrayB = arrayA.at(i).toObject().value("children").toArray();
            SetItemsCity();
            break;
        }
        qDebug() << "i = " << i;
    }

}


void RegistPage::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    qDebug() << "arg1: " << arg1;
    int i = 0;
    for(i = 0; i<arrayA.count(); i++){
        QString name = arrayB.at(i).toObject().value("name").toString();
        qDebug() << "name :" << name;
        if(name == arg1){
            qDebug() << "---city match---";
            int count = arrayC.count();
            qDebug() << "arrayB count : " << count;
            for(int k = count; k>=0; k--){
                ui->comboBox_3->removeItem(k);
            }
            arrayC = arrayB.at(i).toObject().value("children").toArray();
            SetItemsArea();
            break;
        }
        qDebug() << "i = " << i;
    }
}


void RegistPage::on_lineEditPasswd_editingFinished()
{
    if(ui->lineEditPasswd->text().length()<6){
        ui->labelError->setText("密码长度小于6位");
        ui->labelError->show();
    }
}


void RegistPage::on_pushButtonLogin_clicked()
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
            if(ui->lineEditName->text() == ""){
                ui->labelError->setText("请输入昵称");
                ui->labelError->show();
            }else if(ui->comboBox->currentText() == "" || ui->comboBox_2->currentText() == "" || ui->comboBox_3->currentText() == ""){
                ui->labelError->setText("请选择地区");
                ui->labelError->show();
            }else if(ui->lineEditId->text() == ""){
                ui->labelError->setText("请输入云针ID");
                ui->labelError->show();
            }else if(ui->lineEditTel->text() == ""){
                ui->labelError->setText("请输入手机号码");
                ui->labelError->show();
            }else if(ui->lineEditTel->text() != ""){
                QRegularExpression re("^1[3456789]\\d{9}$");
                QRegularExpressionMatch match = re.match(ui->lineEditTel->text(), 0, QRegularExpression::PartialPreferCompleteMatch);
                if(!match.hasMatch()){
                    ui->labelError->setText("手机号码格式不正确");
                    ui->labelError->show();
                }
            }else if(ui->lineEditAuthCode->text() == ""){
                ui->labelError->setText("请输入验证码");
                ui->labelError->show();
            }else if((ui->lineEditPasswd->text() == "") || (ui->lineEditPasswd->text() == "")){
                if(ui->lineEditPasswd2->text() == ""){
                    ui->labelError->setText("请输入新密码");
                }else{
                    ui->labelError->setText("请再次输入登录密码");
                }
                  ui->labelError->show();

            }else if(ui->lineEditPasswd->text() != ui->lineEditPasswd2->text()){
                ui->labelError->setText("两次密码输入不一致,请检查");
                ui->labelError->show();
            }else if((ui->lineEditPasswd->text().length()<6) || (ui->lineEditPasswd2->text().length()<6)){
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

