#ifndef FORGETPASSWDPAGE_H
#define FORGETPASSWDPAGE_H

#include "mainwindow.h"
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QtNetwork>
#include <QUrl>

namespace Ui {
class ForgetPasswdPage;
}
class MainWindow;
class ForgetPasswdPage : public QWidget
{
    Q_OBJECT

public:
    explicit ForgetPasswdPage(MainWindow *win, QWidget *parent = nullptr);
    ~ForgetPasswdPage();

    void initUi();

signals:
    void mainwindowShowSignal();
    void resetPasswdPageShowSignal();

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButton_clicked();

    void on_lineEditAuthCode_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::ForgetPasswdPage *ui;
    MainWindow *mainwindow;
    QTimer *m_timer;
    int i = 60;

friend class MainWindow;
};

#endif // FORGETPASSWDPAGE_H
