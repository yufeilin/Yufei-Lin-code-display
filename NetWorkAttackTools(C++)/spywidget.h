#ifndef SPYWIDGET_H
#define SPYWIDGET_H

#include <QWidget>

#include <QDialog>
#include<QTabWidget>
#include<QComboBox>
#include<QTableView>
#include<QStandardItemModel>
#include<QTextBrowser>


#include"threadhh.h"

namespace Ui {
class spyWidget;
}

class spyWidget : public QWidget
{
    Q_OBJECT

public:

    static string defaultgateway;
    static string gatewaymac;
    static string getgateway();
    explicit spyWidget(QWidget *parent = 0);
    void addNIClist();
    void getNIClist();
    ~spyWidget();

private:

    QPushButton *startbutton;
    QPushButton *stopbutton;


    Threadhh * hh;

    //remember to del;
    int ready;

    static int capturedcount;
    Ui::spyWidget *ui;
    int devcount;
    QString devlist[20];

    QString dev;

    QTabWidget *tabWidget;
    QComboBox *NICselect;

    QTableView *captured;
    QStandardItemModel *capture_item_model;

    QTextBrowser *capture_item_specific;
    QTextBrowser *capture_item_hex;


private slots:
    void setDev(int);
    void startAction();
    void stopAction();
    void get_selected();
    void capturednew();


     void getsomepacket(QString);



     void capturednew2(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
     void dealwitharp(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);


signals:
    void devChanged(QString);
    void devAdded(QString);
    void newpackettellpage2(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);


    void arpcaptured(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);





};

#endif // SPYWIDGET_H
