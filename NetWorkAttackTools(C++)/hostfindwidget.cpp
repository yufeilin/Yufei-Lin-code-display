#include "hostfindwidget.h"

#include "ui_hostfindwidget.h"
#include<QLabel>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSplitter>

#include<taskhandler.h>
#include<QRegExp>


#include"hhpage2.cpp"
#include"spywidget.h"
#include"widget.h"


int hostfindWidget::packetindex = 0;


hostfindWidget::hostfindWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hostfindWidget)
{
//    ui->setupUi(this);

   QLabel *target_label = new QLabel("target");
   targetinput = new QLineEdit;
   target_label->setBuddy(targetinput);


   QLabel *type_label = new QLabel("method");
   typeinput = new QComboBox;
   typeinput->addItem("ARP");
   typeinput->addItem("ICMP");
   typeinput->addItem("TCP");
   typeinput->addItem("UDP");
   type_label->setBuddy(typeinput);



   startbutton = new QPushButton("Detect");
   QSpacerItem* hSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
   QSpacerItem* hSpacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);



   QHBoxLayout *toplayout = new QHBoxLayout;
   toplayout->addWidget(target_label);
   toplayout->addWidget(targetinput);
   toplayout->addSpacerItem(hSpacer1);
   toplayout->addWidget(type_label);
   toplayout->addWidget(typeinput);
   toplayout->addSpacerItem(hSpacer2);
   toplayout->addWidget(startbutton);


    connect(startbutton,SIGNAL(clicked(bool)),this,SLOT(startAction()));


   hostlist = new QListWidget;
   information = new QTextBrowser;

   QSplitter *hsplitter = new QSplitter(Qt::Horizontal);
   hsplitter->addWidget(hostlist);
   hsplitter->addWidget(information);

   QHBoxLayout *mainlayout = new QHBoxLayout;
   mainlayout->addWidget(hsplitter);


   QVBoxLayout *layout = new QVBoxLayout;
   layout->addLayout(toplayout);
   layout->addLayout(mainlayout);
   setLayout(layout);



}

hostfindWidget::~hostfindWidget()
{
    delete ui;
}




//emit captured2(qsrc_mac,qdst_mac,qsrc_ip,qdst_ip,qsrc_port,
//qdst_port,qprotocol,qlen,qtime,cont);
//}

void hostfindWidget::newpacketfrompage1(QString a1,QString a2,QString a3,QString a4,QString a5,
                                        QString a6,QString a7,QString a8,QString a9,QString a10)
{

    int i=0;

    if(a7=="ARP")
        i=3;

    QString current_net;
    current_net = targetinput->text();
    QStringList ipslices = current_net.split(".");

    QStringList inputipslices = a3.split(".");


    if(spyWidget::defaultgateway=="NONE")
        spyWidget::defaultgateway=spyWidget::getgateway();

    for(;i<3;i++)
    {
        if(ipslices.at(i)!=inputipslices.at(i))
            return;
    }





    packetindex++;
    QString text = QString::number(packetindex);
    information->setText(text);

    QRegExp arpreplymatch("REPLY (([0-9]{1,3}[.]){3}"
                          "[0-9]{1,3}) ");

    QString tmpipfromarp="";

    int pos  = a5.indexOf(arpreplymatch);


    int typeflag=0;

    if (a7=="ICMP")
        typeflag=1;
    if(a7=="ARP")
        typeflag=2;
    if(a7=="TCP")
        typeflag=3;

    switch (typeflag) {
    default:
        break;
    case 0:
        break;
    case 1:
        if(a5=="ICMP ECHO reply")
        {
            if(found.find(a3)==found.end())
            {
                found.insert(a3);
                hostlist->addItem(a3);
            }
        }
        else if(a5=="ICMP DST unreachable."){
            if(found.find(a3)==found.end())
            {
                found.insert(a3);
                hostlist->addItem(a3);
            }
        }
        hostlist->sortItems();
        break;

    case 2:
       if(pos>=0)
        {
            QString result=arpreplymatch.cap(1);
            if(found.find(result)==found.end())
            {
                found.insert(result);
                hostlist->addItem(result);
            }
        }
        hostlist->sortItems();
        break;

    case 3:
        string tmp;
        tmp.resize(7);
        string a5tmp=a5.toStdString();
        for(int i=0; i<7; i++){
            tmp[i]=a5tmp[i];
        }
        cout<<"tmp:"<<tmp<<endl;
        QString result;
        if(tmp=="ACK|SYN"){
            result=a3;
            if(found.find(result)==found.end())
            {
                found.insert(result);
                hostlist->addItem(result);
            }
            hostlist->sortItems();
        }

        break;


    }


}


//typeinput->addItem("ARP");
//typeinput->addItem("ICMP");
//typeinput->addItem("TCP");
//typeinput->addItem("UDP");

void hostfindWidget::startAction()
{

    startbutton->setEnabled(false);
    Taskhandler *discover=new Taskhandler;

    connect(discover,SIGNAL(finish()),this,SLOT(activatebutton()));


    QString targetips = targetinput->text();
    int typeindex = typeinput->currentIndex();


    switch(typeindex)
    {

    case 1:
        discover->settaskid(1);
        discover->settargetips(targetips.toStdString());
        discover->start();
        break;
    case 0:
        discover->settaskid(0);
        discover->settargetips(targetips.toStdString());
        discover->start();
        break;
    case 2:
        discover->settaskid(2);
        discover->settargetips(targetips.toStdString());
        discover->start();
        break;
    case 3:

        discover->settaskid(3);
        discover->settargetips(targetips.toStdString());
        discover->start();

    default:
        break;

    }




    discover=NULL;

    delete discover;

}

void hostfindWidget::activatebutton()
{
    startbutton->setEnabled(true);
    emit scanfinish();
}

void hostfindWidget::scanarp()
{

//aaa.aaa.aaa.aaa

    QString iptargets=QString::fromStdString(spyWidget::defaultgateway);
    qDebug()<<"[A]hostfindWidget::scanarp activate by arpWidget\n";
    qDebug()<<"[A+content] "<<iptargets;

    typeinput->setCurrentIndex(0);
    targetinput->setText("192.168.153.");
    startAction();
}
