#include "widget.h"
#include "ui_widget.h"
#include<QLabel>
#include<QHBoxLayout>
#include<QDebug>
#include"spydialog.h"
#include"spywidget.h"
#include"doswidget.h"
#include"hostfindwidget.h"
#include"arpwidget.h"



std::string Widget::gatewaymac="NONE";

QMap<std::string,std::string> Widget::myarp;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    QIcon spy_icon = QIcon(":img/capture.png");
    QIcon host_discovery_icon = QIcon(":img/hostDiscovery.png");
    QIcon port_scan_icon = QIcon(":img/portScan.png");
    QIcon arp_icon = QIcon(":img/arp.png");
    QIcon dos_icon = QIcon(":img/dos.png");
    QIcon sender_icon = QIcon(":img/sender.png");


    QWidget* spy_tab = new QWidget;
    QWidget *arp_tab = new QWidget;
    QWidget *port_scan_tab = new QWidget;
    QWidget *dos_tab = new QWidget;
    QWidget *sender_tab = new QWidget;
    QWidget *host_discovery_tab = new QWidget;











//page 1
    spyWidget *spy = new spyWidget;
    QLayout *spy_layout = new QHBoxLayout;
    spy_layout->addWidget(spy);
    spy_tab->setLayout(spy_layout);


//page 2
    hostfindWidget *hostfind = new hostfindWidget;
    QLayout *hostfind_layout = new QHBoxLayout;
    hostfind_layout->addWidget(hostfind);
    host_discovery_tab->setLayout(hostfind_layout);

//page 4

    arpWidget *arp = new arpWidget;
    QLayout *arp_layout = new QHBoxLayout;
    arp_layout->addWidget(arp);
    arp_tab->setLayout(arp_layout);

    connect(arp,SIGNAL(arpscan()),hostfind,SLOT(scanarp()));
    connect(hostfind,SIGNAL(scanfinish()),arp,SLOT(scanfinish()));




    connect(spy,SIGNAL(newpackettellpage2(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),
            hostfind,SLOT(newpacketfrompage1(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));


//page 5
    dosWidget *dos = new dosWidget;
    QLayout *dos_layout = new QHBoxLayout;
    dos_layout->addWidget(dos);
    dos_tab->setLayout(dos_layout);





//devices add connect signal share
    connect(spy,SIGNAL(devChanged(QString)),dos,SLOT(setDev(QString)));
    connect(spy,SIGNAL(devAdded(QString)),dos,SLOT(addDev(QString)));




// main tabwidget
    tabWidget = new QTabWidget();
    tabWidget->setIconSize(QSize(60,60));
    tabWidget->setTabShape(QTabWidget::Rounded);

    tabWidget->addTab(spy_tab,spy_icon,QString("capture"));
    tabWidget->addTab(host_discovery_tab,host_discovery_icon,QString("find"));
    tabWidget->addTab(port_scan_tab,port_scan_icon,QString("port"));
    tabWidget->addTab(arp_tab,arp_icon,QString("ARP"));
    tabWidget->addTab(dos_tab,dos_icon,QString("Dos"));
    tabWidget->addTab(sender_tab,sender_icon,QString("Send"));


    QLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    this->setLayout(mainLayout);
    this->resize(730,480);
    this->setWindowTitle("BOB");



    spy->getNIClist();
    spy->addNIClist();






    //------------------debug---------------------------
//    qDebug()<<tabWidget->count();
//    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(hey()));

    //------------------debug---------------------------



}

Widget::~Widget()
{
    delete ui;
}

void Widget::hey(){
    qDebug()<<tabWidget->currentIndex();
}
