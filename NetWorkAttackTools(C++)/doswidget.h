#ifndef DOSWIDGET_H
#define DOSWIDGET_H

#include <QWidget>
#include<QComboBox>
#include<QLineEdit>
#include<QTextBrowser>
#include<QPushButton>


#define MAXTHREAD 16
#include"threadtiaozhang.h"


namespace Ui {
class dosWidget;
}

class dosWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dosWidget(QWidget *parent = 0);

    void setline(int x,QString text);

    void flushtext();
    ~dosWidget();
private:


    Ui::dosWidget *ui;
    QComboBox *NICselect;
    QString dev;
    QPushButton *startbutton;
    QPushButton *stopbutton;

    QLineEdit *des_ip_input;
    QLineEdit *des_port_input;
    QLineEdit *thread_count_input;

    QComboBox *attack_type_select;

    Threadtiaozhang *tiaozhang[MAXTHREAD];


    QTextBrowser*commandline;
    QString textcontent[12];
    int sendedcount;



private slots:
    void setDev(QString);

    void addDev(QString);

    void startAction();
    void stopAction();

    void getsendedip(QString);


};

#endif // DOSWIDGET_H
