#include "spywidget.h"
#include "ui_spywidget.h"

#include<QHBoxLayout>
#include<QString>
#include<QLabel>
#include<QTableView>
#include<QSplitter>
#include<QModelIndex>
#include<QDebug>
#include<QPushButton>

#include<QTableWidgetItem>

//---
#include<stdio.h>
#include<string.h>
#include<pcap.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/if_ether.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip_icmp.h>
#include<stdlib.h>
#include<stdint.h>
#include <stdint.h>
#include<time.h>
#include <net/if_arp.h>
//---


#include<unistd.h>
#include"threadhh.h"

#include"taskhandler.h"
#include"widget.h"



string spyWidget::defaultgateway="NONE";
string spyWidget::gatewaymac="NONE";

string spyWidget::getgateway()
{
    FILE *fp;
    char buf[512];
    char cmd[128];
    char gateway[30];
    char *tmp;

    strcpy(cmd, "ip route");
    fp = popen(cmd, "r");
    if(NULL == fp)
    {
        perror("popen error");
    }
    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        tmp =buf;
        while(*tmp && isspace(*tmp))
            ++tmp;
        if(strncmp(tmp, "default", strlen("default")) == 0)
            break;
    }
    sscanf(buf, "%*s%*s%s", gateway);
    printf("default gateway:%s\n", gateway);
    string result=gateway;
//    for(int i=0; )
//    string result = new string(gateway);


    pclose(fp);

    Taskhandler::getgatewaymac(result);
    return result;


}


spyWidget::spyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spyWidget)
{


    ready=1;
//    ui->setupUi(this);
    devcount=0;

    QSpacerItem* hSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);



    NICselect = new QComboBox;
    startbutton = new QPushButton("start");
    stopbutton = new QPushButton("stop");

    //connect signal and slots:
    connect(NICselect,SIGNAL(currentIndexChanged(int)),this,SLOT(setDev(int)));
    connect(startbutton,SIGNAL(clicked()),this,SLOT(startAction()));
    connect(stopbutton,SIGNAL(clicked()),this,SLOT(stopAction()));

    //NICselect and button at the top;
     QHBoxLayout *hlayout = new QHBoxLayout;
     hlayout->addWidget(NICselect);
     hlayout->addSpacerItem(hSpacer1);
     hlayout->addWidget(startbutton);
     hlayout->addWidget(stopbutton);




    capture_item_model = new QStandardItemModel;


    capture_item_model->setHorizontalHeaderItem(0,new QStandardItem(tr("NO")));
    capture_item_model->setHorizontalHeaderItem(1,new QStandardItem(tr("time")));
    capture_item_model->setHorizontalHeaderItem(2,new QStandardItem(tr("srcmac")));
    capture_item_model->setHorizontalHeaderItem(3,new QStandardItem(tr("dstmac")));
    capture_item_model->setHorizontalHeaderItem(4,new QStandardItem(tr("srcip")));
    capture_item_model->setHorizontalHeaderItem(5,new QStandardItem(tr("desip")));
    capture_item_model->setHorizontalHeaderItem(6,new QStandardItem(tr("protocol")));
    capture_item_model->setHorizontalHeaderItem(7,new QStandardItem(tr("length")));
    capture_item_model->setHorizontalHeaderItem(8,new QStandardItem(tr("info")));
    capture_item_model->setHorizontalHeaderItem(9,new QStandardItem(tr("data")));






    captured = new QTableView;

    captured->setModel(capture_item_model);

    captured->setEditTriggers(QAbstractItemView::NoEditTriggers);
    captured->setSelectionBehavior(QAbstractItemView::SelectRows);


    connect(captured,SIGNAL(clicked(QModelIndex)),this,SLOT(get_selected()));



    capture_item_specific = new QTextBrowser;
    capture_item_hex = new QTextBrowser;

    QSplitter *specific_splitter = new QSplitter(Qt::Horizontal);
    specific_splitter->addWidget(capture_item_specific);
    specific_splitter->addWidget(capture_item_hex);



    QVBoxLayout*layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addWidget(captured);
    layout->addWidget(specific_splitter);
    setLayout(layout);

//    getNIClist();
//    addNIClist();

    connect(this,SIGNAL(arpcaptured(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),
            this,SLOT(dealwitharp(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));



}




spyWidget::~spyWidget()
{
    delete ui;
}






void spyWidget::getNIClist()
{
    pcap_if_t *p;
    pcap_if_t *alldev;

    char error[100];
    if(pcap_findalldevs(&alldev,error)==-1)
    {
        printf("find all devices is error\n");
        qDebug()<<"cannot get dev!";
        exit(0);
    }


    for(p=alldev;p;p=p->next)
    {
        QString tmp = QString(p->name);
        devlist[devcount++]=tmp;
        //---lack decription

        if(p->description)
        {
            printf("%s\n",p->description);
        }
    }

}

void spyWidget::addNIClist(){

    for(int i=0;i<devcount;i++)
    {
        NICselect->addItem(devlist[i]);
        emit(devAdded(devlist[i]));
    }
}


void spyWidget::setDev(int selected)
{
    dev = devlist[selected];
    emit(devChanged(dev));
}

void spyWidget::startAction()
{


    startbutton->setEnabled(false);
    stopbutton->setEnabled(true);

    hh = new Threadhh;
    connect(hh,SIGNAL(captured()),this,SLOT(capturednew()));

    connect(hh,SIGNAL(captured2(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),
            this,SLOT(capturednew2(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));


    hh->start();




    captured->setColumnHidden(9,true);
    sleep(1);
    spyWidget::defaultgateway=spyWidget::getgateway();


    //get gateway mac by asked



    printf("spyWidget::startAction: [+] get gateawy ip:%s\n",spyWidget::defaultgateway.data());
}


void spyWidget::capturednew(){
        packet nowpacket = Threadhh::packetnow;

        QString time = QString(QLatin1String(nowpacket.gettime()));
        QString srcmac = QString(QLatin1String(nowpacket.getsrcmac()));
        QString dstmac = QString(QLatin1String(nowpacket.getdstmac()));
        QString srcip = QString(QLatin1String(nowpacket.getsrcip()));
        QString dstip = QString(QLatin1String(nowpacket.getdstip()));
        QString length = QString::number(nowpacket.getlength());
        QString protocol = QString(QLatin1String(nowpacket.getprotl()));
        QString descript = QString(QLatin1String(nowpacket.getdescp()));





        capture_item_model->setItem(capturedcount, 0, new QStandardItem(QString::number(capturedcount+1)));


        capture_item_model->setItem(capturedcount, 1, new QStandardItem(time));
        capture_item_model->setItem(capturedcount, 2, new QStandardItem(srcmac));
        capture_item_model->setItem(capturedcount, 3, new QStandardItem(dstmac));
        capture_item_model->setItem(capturedcount, 4, new QStandardItem(srcip));
        capture_item_model->setItem(capturedcount, 5, new QStandardItem(dstip));
        capture_item_model->setItem(capturedcount, 6, new QStandardItem(length));
        capture_item_model->setItem(capturedcount, 7, new QStandardItem(protocol));
        capture_item_model->setItem(capturedcount, 8, new QStandardItem(descript));


        qDebug()<<descript<<"ay------------------------------------";




        capturedcount++;

}


void spyWidget::stopAction(){
    stopbutton->setEnabled(false);
    if(hh->isRunning())
        hh->terminate();
    startbutton->setEnabled(true);
}




void spyWidget::get_selected(){

//    if(capturedcount%10==1)
//        captured->resizeColumnsToContents();


   int index = captured->currentIndex().row();



    QString text;
    text = capture_item_model->item(index,1)->text();
    text+='\n';
    text +=capture_item_model->item(index,2)->text();
    text+='\n';
    text +=capture_item_model->item(index,3)->text();
    text+='\n';
    text +=capture_item_model->item(index,4)->text();
    text+='\n';
    text +=capture_item_model->item(index,5)->text();
    text+='\n';
    text +=capture_item_model->item(index,6)->text();
    text+='\n';
    text +=capture_item_model->item(index,7)->text();
    text+='\n';
    text +=capture_item_model->item(index,8)->text();
    text+='\n';
    capture_item_specific->setText(text);
//    text = "A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32"
//    ;
    capture_item_hex->setText(capture_item_model->item(index,9)->text());



//    else
//    {
////         QMap<string, string>::const_iterator i;
////        for (i = Widget::myarp.constBegin(); i != Widget::myarp.constEnd(); ++i) {
////           printf("myarp:%s ---- %s\n",i.key().data(),i.value().data());
////       }
//    }

}



void spyWidget::getsomepacket(QString packets)
{
}





//emit captured2(qsrc_mac,qdst_mac,qsrc_ip,qdst_ip,qsrc_port,

//                    qdst_port,qprotocol,qlen,qtime,content);
//}

void spyWidget::capturednew2(QString a1, QString a2, QString a3, QString a4, QString a5, QString a6,
                             QString a7, QString a8, QString a9,QString a10)
{



   emit newpackettellpage2(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);


    capture_item_model->setItem(capturedcount, 0, new QStandardItem(QString::number(capturedcount+1)));


    capture_item_model->setItem(capturedcount, 1, new QStandardItem(a9));

    capture_item_model->setItem(capturedcount, 2, new QStandardItem(a1));
    capture_item_model->setItem(capturedcount, 3, new QStandardItem(a2));

    capture_item_model->setItem(capturedcount, 4, new QStandardItem(a3));
    capture_item_model->setItem(capturedcount, 5, new QStandardItem(a4));
    capture_item_model->setItem(capturedcount, 6, new QStandardItem(a7));
    capture_item_model->setItem(capturedcount, 7, new QStandardItem(a8));

    capture_item_model->setItem(capturedcount, 8, new QStandardItem(a5));
    capture_item_model->setItem(capturedcount, 9, new QStandardItem(a10));

    if(a7=="ARP")
        emit arpcaptured(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);







//



    if(capturedcount-(captured->currentIndex().row())<5)
        captured->selectRow(capturedcount);

     capturedcount++;

}



//emit captured2(qsrc_mac,qdst_mac,qsrc_ip,qdst_ip,qsrc_port,

//                    qdst_port,qprotocol,qlen,qtime,content);


//REPLY 192.168.153.2 is at 00:50:56:fd:fe:cc tell

void spyWidget::dealwitharp(QString a1, QString a2, QString a3, QString a4, QString a5, QString a6,
                            QString a7, QString a8, QString a9,QString a10)

{




//    QString reg = "REPLY "+QString::fromStdString(spyWidget::defaultgateway)+
//            " is at "+"(([0-9a-f]{2}:){5}[0-9a-f]{2}) tell";

//    QRegExp targetarp(reg);
//    int pos = a5.indexOf(targetarp);

//    if(pos>=0)
//    {
//        spyWidget::gatewaymac = targetarp.cap(1).toStdString();
//        Widget::gatewaymac = spyWidget::gatewaymac;
//    }

//    REPLY 192.168.153.2 is at 00:50:56:fd:fe:cc tell to: 192.168.153.136[00:0c:29:68:96:2e]


    QString ipreg="(([0-9]{1,3}.){3}[0-9]{1,3})";

    QString reg = "REPLY "+ipreg+
            " is at "+"(([0-9a-f]{2}:){5}[0-9a-f]{2}) tell";

    QRegExp targetarp(reg);
    int pos = a5.indexOf(targetarp);

    if(pos>=0)
    {
        QString myip = targetarp.cap(1);
        if(myip.toStdString()==spyWidget::defaultgateway)
        {
            spyWidget::gatewaymac = targetarp.cap(3).toStdString();
            Widget::gatewaymac = spyWidget::gatewaymac;
        }
        if(!Widget::myarp.contains(myip.toStdString()))
        {
            Widget::myarp.insert(myip.toStdString(),targetarp.cap(3).toStdString());
        }
    }


}




//QRegExp arpreplymatch("REPLY (([0-9]{1,3}[.]){3}"
//                      "[0-9]{1,3}) ");

//QString tmpipfromarp="";

//int pos  = a5.indexOf(arpreplymatch);
