#include "doswidget.h"
#include "ui_doswidget.h"
#include<QComboBox>
#include<QHBoxLayout>
#include<QPushButton>
#include<QLabel>
#include<QDebug>
#include<QSplitter>
#include<QTextBrowser>
#include<string>
#include<QLineEdit>
#include<string>

dosWidget::dosWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dosWidget)
{
//    ui->setupUi(this);

    for(int i=0;i<MAXTHREAD;i++)
    {
        tiaozhang[i] = new Threadtiaozhang;

        connect(tiaozhang[i],SIGNAL(sendapacket(QString)),this,SLOT(getsendedip(QString)));
    }

    sendedcount=0;

    for(int i=0;i<12;i++)
        textcontent[i]="\n";

    QHBoxLayout *toplayout = new QHBoxLayout;
    NICselect = new QComboBox;

    startbutton = new QPushButton("start");
    stopbutton = new QPushButton("stop");
    QSpacerItem* hSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);



    //NICselect and button at the top;
     toplayout->addWidget(NICselect);
     toplayout->addSpacerItem(hSpacer1);
     toplayout->addWidget(startbutton);
     toplayout->addWidget(stopbutton);

     connect(startbutton,SIGNAL(clicked(bool)),this,SLOT(startAction()));
     connect(stopbutton,SIGNAL(clicked(bool)),this,SLOT(stopAction()));


     attack_type_select = new QComboBox;
     QStringList attack_types =QStringList();

     attack_types<<"SYN"<<"ACK"<<"Land"<<"UDP"<<"ICMP";

     attack_type_select->addItems(attack_types);
     attack_type_select->setCurrentIndex(0);


     des_ip_input = new QLineEdit;
     des_ip_input->setText("192.168.1.1");

     des_port_input = new QLineEdit;
     des_port_input->setText("23");

     thread_count_input=new QLineEdit;
     thread_count_input->setText("4");



//     QRegExpValidator *pRevalidotor = new QRegExpValidator(regExp, this);

//     绑入lineEdit :

//     pEditWell->setValidator(new QRegExpValidator(regExp, this));








     QHBoxLayout*type_select = new QHBoxLayout;
     type_select->addWidget(new QLabel("Type"));
     type_select->addWidget(attack_type_select);


     QHBoxLayout*des_ip = new QHBoxLayout;
     des_ip->addWidget(new QLabel("IP"));
     des_ip->addWidget(des_ip_input);



     QHBoxLayout*des_port = new QHBoxLayout;
     des_port->addWidget(new QLabel("PORT"));
     des_port->addWidget(des_port_input);

     QHBoxLayout*thread_count = new QHBoxLayout;
     thread_count->addWidget(new QLabel("THREAD"));
     thread_count->addWidget(thread_count_input);






     QVBoxLayout*leftlayout = new QVBoxLayout;
     leftlayout->addLayout(type_select);
     leftlayout->addLayout(des_ip);
     leftlayout->addLayout(des_port);
     leftlayout->addLayout(thread_count);



     commandline = new QTextBrowser;
     commandline->setTextColor(QColor(Qt::green));

    QSplitter *specific_splitter = new QSplitter(Qt::Horizontal);


     QVBoxLayout*rightlayout = new QVBoxLayout;
     rightlayout->addWidget(commandline);

     QHBoxLayout* midlayout = new QHBoxLayout;
     midlayout->addLayout(leftlayout);
     midlayout->addWidget(specific_splitter);
     midlayout->addLayout(rightlayout);




     QVBoxLayout* layout = new QVBoxLayout;

     layout->addLayout(toplayout);
     layout->addLayout(midlayout);
     setLayout(layout);





}

dosWidget::~dosWidget()
{
    delete ui;
}

void dosWidget::setDev(QString currentDev)
{
    qDebug()<<currentDev;
    dev = currentDev;
    NICselect->setCurrentText(dev);
}


void dosWidget::addDev(QString currentDev)
{
//    qDebug()<<currentDev<<"----added";
    NICselect->addItem(currentDev);
}

void dosWidget::startAction()
{


    setline(1,"DOS attacking using "+attack_type_select->currentText());
    startbutton->setEnabled(false);
    qDebug()<<"start";
    QString thread_count_qstring = thread_count_input->text();
    QString des_ip_qstring =  des_ip_input->text();
    QString des_port_qstring =  des_port_input->text();

    std::string ip = des_ip_qstring.toStdString();
    int port = des_port_qstring.toInt();

    int threadcount=thread_count_qstring.toInt();



    for(int i=0;i<threadcount;i++)
    {
        if(tiaozhang[i]->isRunning())
            {
                tiaozhang[i]->terminate();
            }

        tiaozhang[i]->set_dev(NICselect->currentText().toStdString());
        tiaozhang[i]->set_dst_ip(ip);
        tiaozhang[i]->set_dst_port(port);
        tiaozhang[i]->set_attack_type(attack_type_select->currentIndex());
        tiaozhang[i]->start();
    }

}

void dosWidget::stopAction()
{

    setline(1,"DOS attacking terminated.");

    for(int i=0;i<MAXTHREAD;i++)
    {
        if(tiaozhang[i]->isRunning())
        {
            tiaozhang[i]->terminate();
            qDebug()<<"thread "<<i<<" stoped.";
        }
    }
    qDebug()<<"---------------stoped----------------";

    startbutton->setEnabled(true);

}

void dosWidget::getsendedip(QString x)
{
    setline(12,"Using IP:"+x);
    sendedcount++;
    setline(2,"sended:"+QString::number(sendedcount));
}

void dosWidget::setline(int x,QString text){

    textcontent[x-1]="";
    textcontent[x-1]+= text;
    textcontent[x-1]+="\n";
    flushtext();
}

void dosWidget::flushtext()
{
   QString fulltext="";
   for (int i=0;i<12;i++)
   {
       fulltext+=textcontent[i];
   }

   commandline->setText(fulltext);
}

