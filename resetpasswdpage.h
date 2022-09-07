#ifndef RESETPASSWDPAGE_H
#define RESETPASSWDPAGE_H

#include "mainwindow.h"
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QtNetwork>
#include <QUrl>
#include "forgetpasswdpage.h"
#include <QtNetwork>
#include <QUrl>

namespace Ui {
class ResetPasswdPage;
}
class MainWindow;
class ForgetPasswdPage;
class ResetPasswdPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResetPasswdPage(MainWindow *win, QWidget *parent = nullptr);
    ~ResetPasswdPage();
signals:
    void mainwindowShowSignal();

private slots:

    void on_lineEditNewPasswd_editingFinished();

    void on_lineEditNewPasswd_textEdited(const QString &arg1);

    void on_pushButtonOk_clicked();

    void on_lineEditPasswd_textEdited(const QString &arg1);

private:
    Ui::ResetPasswdPage *ui;
    MainWindow *mainwindow;

friend class MainWindow;
};

#endif // RESETPASSWDPAGE_H
