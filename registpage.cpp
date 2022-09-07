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

    if(readJsonFile())
    {
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

int RegistPage::readJsonFile()
{
    QFile file("./LocalList.json");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "file: " << file.fileName() << " read error: " << file.error();
        return -1;
    }
    data = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return -1;
    }

    QJsonObject jsonobj = doc.object();
    arrayA = jsonobj.value("data").toArray();

    return 0;
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

