#ifndef REGISTPAGE_H
#define REGISTPAGE_H

#include "mainwindow.h"
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QtNetwork>
#include <QUrl>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QRegularExpression>


namespace Ui {
class RegistPage;
}
class MainWindow;
class RegistPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegistPage(MainWindow *win, QWidget *parent = nullptr);
    ~RegistPage();

    int readJsonFile();

signals:
    void mainwindowShowSignal();

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButton_clicked();


    void SetItemsProvince();
    void SetItemsCity();
    void SetItemsArea();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_lineEditPasswd_editingFinished();


    void on_pushButtonLogin_clicked();

private:
    Ui::RegistPage *ui;
    MainWindow *mainwindow;
    QTimer *m_timer;
    int i = 60;

    QByteArray data;
    QJsonArray arrayA;
    QJsonArray arrayB;
    QJsonArray arrayC;

friend class MainWindow;
};

#endif // REGISTPAGE_H
