#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loadingpage.h"
#include "forgetpasswdpage.h"
#include "registpage.h"
#include "resetpasswdpage.h"
#include <QMap>

typedef void (*ReqCallBack)(QByteArray data);

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class LoadingPage;
class ForgetPasswdPage;
class RegistPage;
class ResetPasswdPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initUi();

    /**
     * @brief http发送和接收接口（同步）
     * @param url HTTP网络URL
     * @param thod HTTP方法，GET或POST
     * @param input 发送的数据
     * @param err http返回的错误信息
     * @param recData http接收响应
     */
    int httpRequestSend(QString url, QString thod, QJsonObject input, QString &err, QByteArray &recData);

signals:
    void loadingShowSignal();

public slots:
    void mainwindowShowSlot();
    void resetPasswdPageShowSlot();

private slots:

    void on_pushButtonLogin_clicked();

    void on_lineEditId_textEdited(const QString &arg1);

    void on_lineEditPasswd_textEdited(const QString &arg1);

    void on_pushButtonForgetPasswd_clicked();

    void on_checkBoxRemeberPasswd_stateChanged(int arg1);

    void on_checkBoxAutoLogin_stateChanged(int arg1);

    void on_pushButtonRegist_clicked();

private:
    Ui::MainWindow      *ui;
    LoadingPage         *widgetLoadingPage;
    ForgetPasswdPage    *widgetForgetPasswdPage;
    RegistPage          *widgetRegistPage;
    ResetPasswdPage     *widgetResetPasswdPage;

    QString token;

friend class LoadingPage;
friend class ForgetPasswdPage;
friend class RegistPage;
friend class ResetPasswdPage;

};
#endif // MAINWINDOW_H
