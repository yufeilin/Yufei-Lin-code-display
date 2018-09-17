#ifndef HOSTFINDWIDGET_H
#define HOSTFINDWIDGET_H

#include <QWidget>
#include<QLineEdit>
#include<QComboBox>
#include<QListWidget>
#include<QPushButton>
#include<QTextBrowser>
#include<QSet>


namespace Ui {
class hostfindWidget;
}


class hostfindWidget : public QWidget
{
    Q_OBJECT

public:
    explicit hostfindWidget(QWidget *parent = 0);
    ~hostfindWidget();

private:

    QSet<QString> found;
    static int packetindex;
    Ui::hostfindWidget *ui;
    QLineEdit *targetinput;
    QComboBox *typeinput;
    QPushButton *startbutton;

    QListWidget* hostlist;
    QTextBrowser* information;


private slots:
    void newpacketfrompage1(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void startAction();

    void activatebutton();

    void scanarp();

signals:
     void scanfinish();

};

#endif // HOSTFINDWIDGET_H
