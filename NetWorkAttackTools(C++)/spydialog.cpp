#include "spydialog.h"
#include "ui_spydialog.h"
#include<QHBoxLayout>
#include<QString>
#include<QLabel>
#include<QTableView>
#include<QSplitter>
#include<QModelIndex>
#include<QDebug>


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



spyDialog::spyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::spyDialog)
{
   devcount=0;
   QSpacerItem* hSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);




   QHBoxLayout *hlayout = new QHBoxLayout;
   NICselect = new QComboBox;
   QPushButton *startbutton = new QPushButton("start");
   QPushButton *stopbutton = new QPushButton("stop");






//   getNIClist();
//   addNIClist();

   connect(NICselect,SIGNAL(currentIndexChanged(int)),this,SLOT(setDev(int)));
   connect(startbutton,SIGNAL(clicked()),this,SLOT(startAction()));
   connect(stopbutton,SIGNAL(clicked()),this,SLOT(stopAction()));



   //NICselect and button at the top;
    hlayout->addWidget(NICselect);
    hlayout->addSpacerItem(hSpacer1);
    hlayout->addWidget(startbutton);
    hlayout->addWidget(stopbutton);




   capture_item_model = new QStandardItemModel;
   capture_item_model->setHorizontalHeaderItem(0,new QStandardItem(tr("NO")));
   capture_item_model->setHorizontalHeaderItem(1,new QStandardItem(tr("time")));
   capture_item_model->setHorizontalHeaderItem(2,new QStandardItem(tr("srcip")));
   capture_item_model->setHorizontalHeaderItem(3,new QStandardItem(tr("desip")));
   capture_item_model->setHorizontalHeaderItem(4,new QStandardItem(tr("protocol")));
   capture_item_model->setHorizontalHeaderItem(5,new QStandardItem(tr("length")));
   capture_item_model->setHorizontalHeaderItem(6,new QStandardItem(tr("info")));




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



}

void spyDialog::getNIClist()
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

void spyDialog::addNIClist(){
    for(int i=0;i<devcount;i++)
    {
        NICselect->addItem(devlist[i]);
    }
}

void spyDialog::setDev(int selected)
{
    dev = devlist[selected];
    qDebug()<<dev;
}


void spyDialog::startAction()
{

    //test add item

    capture_item_model->setItem(0,0,new QStandardItem("1"));
    capture_item_model->setItem(0,1,new QStandardItem("1452665114"));
    capture_item_model->setItem(0,2,new QStandardItem("125.216.249.137"));
    capture_item_model->setItem(0,3,new QStandardItem("125.216.249.254"));
    capture_item_model->setItem(0,4,new QStandardItem("ARP"));
    capture_item_model->setItem(0,5,new QStandardItem("46"));
    capture_item_model->setItem(0,6,new QStandardItem("Who has 125.216.249.136?......"));

    capture_item_model->setItem(1,0,new QStandardItem("2"));
    capture_item_model->setItem(1,1,new QStandardItem("14526654151"));
    capture_item_model->setItem(1,2,new QStandardItem("125.216.249.137"));
    capture_item_model->setItem(1,3,new QStandardItem("125.216.249.254"));
    capture_item_model->setItem(1,4,new QStandardItem("ARP"));
    capture_item_model->setItem(1,5,new QStandardItem("46"));
    capture_item_model->setItem(1,6,new QStandardItem("Who has 125.216.249.136?......"));




    capture_item_specific->setText(tr("6563	\n"
                                      "1872.384320000	\n"
                                      "192.168.244.134	111.202.114.35	\n"
                                      "TCP	\n"
                                      "54	\n"
                                      "56532 > http [FIN, ACK] \n"
                                      "Seq=1934 Ack=375 Win=30016 Len=0\n"));


    capture_item_hex->setText(tr("0000   00 50 56 e9 7a fe 00 0c 29 27 6f b1 08 00 45 00"
                                 "0010   00 28 cc 6a 40 00 40 06 d7 48 c0 a8 f4 86 6f ca"
                                 "0020   72 23 dc d4 00 50 d6 e9 77 fb 3c 42 50 68 50 11"
                                 "0030   75 40 97 37 00 00"));



    //end test



}

void spyDialog::stopAction(){

}

spyDialog::~spyDialog()
{
    delete ui;
}




void spyDialog::get_selected(){
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
//    text +=capture_item_model->item(index,7)->text();
    capture_item_specific->setText(text);
    text = "A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32A2 B0 3E 3F 55 39 32"
    ;capture_item_hex->setText(text);
}
