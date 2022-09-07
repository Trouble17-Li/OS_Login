#ifndef LOADINGPAGE_H
#define LOADINGPAGE_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class LoadingPage;
}
class MainWindow;
class LoadingPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingPage(MainWindow *win, QWidget *parent = nullptr);
    ~LoadingPage();

private:
    MainWindow *mainwindow;
    Ui::LoadingPage *ui;

friend class MainWindow;
};

#endif // LOADINGPAGE_H
