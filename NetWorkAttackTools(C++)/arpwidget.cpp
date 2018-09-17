#include "arpwidget.h"
#include "ui_arpwidget.h"


#include"spywidget.h"
#include"widget.h"
#include"arpthread.h"
arpWidget::arpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::arpWidget)
{
    ui->setupUi(this);
    count=0;
    ui->attacktypeinput->addItem("Block single out");
    ui->attacktypeinput->addItem("Block single in");
    ui->attacktypeinput->addItem("Middleman");
    ui->attacktypeinput->addItem("All Die");
    ui->NAinput->setEnabled(false);


//     capture_item_model = new QStandardItemModel;
//    capture_item_model->setHorizontalHeaderItem(0,new QStandardItem(tr("NO")));

//    captured->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    captured->setSelectionBehavior(QAbstractItemView::SelectRows);


//    connect(captured,SIGNAL(clicked(QModelIndex)),this,SLOT(get_selected()));

    tablemodel = new QStandardItemModel;


    tablemodel->setHorizontalHeaderItem(0,new QStandardItem(tr("NO")));
    tablemodel->setHorizontalHeaderItem(1,new QStandardItem(tr(" I P ")));
    tablemodel->setHorizontalHeaderItem(2,new QStandardItem(tr("M A C")));


    ui->ip_mac_table->setModel(tablemodel);
    ui->ip_mac_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ip_mac_table->setSelectionBehavior(QAbstractItemView::SelectRows);

}

arpWidget::~arpWidget()
{
    delete ui;
}


//ui->attacktypeinput->addItem("Block single out");
//ui->attacktypeinput->addItem("Block single in");
//ui->attacktypeinput->addItem("Middleman");
//ui->attacktypeinput->addItem("All Die");

void arpWidget::on_attacktypeinput_currentIndexChanged(int index)
{
    if(index==0||index==1)
    {
        ui->tomip->setText("");
        ui->aliceip->setText("");
        ui->tomip->setEnabled(false);
        ui->aliceip->setEnabled(true);
        ui->selectasTom->setEnabled(false);
        ui->selectasAlice->setEnabled(true);
    }
    if(index==2)
    {
        ui->tomip->setText("");
        ui->aliceip->setText("");
        ui->tomip->setEnabled(true);
        ui->aliceip->setEnabled(true);
        ui->selectasTom->setEnabled(true);
        ui->selectasAlice->setEnabled(true);

    }
    if(index==3)
    {
        ui->tomip->setText("");
        ui->aliceip->setText("");
        ui->tomip->setEnabled(false);
        ui->aliceip->setEnabled(false);
        ui->selectasTom->setEnabled(false);
        ui->selectasAlice->setEnabled(false);
        ui->aliceip->setText(QString::fromStdString(spyWidget::defaultgateway));

    }

}

void arpWidget::on_refresh_clicked()
{
    ui->refresh->setEnabled(false);
    ui->NAinput->setText(QString::fromStdString(spyWidget::defaultgateway+"/24"));

    if(spyWidget::defaultgateway!="NONE")
    {
        emit arpscan();
    }




//    QMap<int, int>::const_iterator i;
//   for (i = map.constBegin(); i != map.constEnd(); ++i) {
//       qDebug() << i.key() << ":" << i.value();
//   }


}

//capture_item_model->setItem(capturedcount, 1, new QStandardItem(time));

void arpWidget::addtoList(){
   QMap<string,string>::const_iterator i;
   for(i = Widget::myarp.constBegin();i!=Widget::myarp.constEnd();++i)
   {
       tablemodel->setItem(count,0,new QStandardItem(QString::number(count+1)));
       tablemodel->setItem(count,1,new QStandardItem(QString::fromStdString(i.key())));
       tablemodel->setItem(count,2,new QStandardItem(QString::fromStdString(i.value())));
       count++;
   }

}


void arpWidget::scanfinish(){
    tablemodel->clear();
    count=0;
    addtoList();
    ui->refresh->setEnabled(true);
}

void arpWidget::on_ip_mac_table_activated(const QModelIndex &index)
{

}

void arpWidget::on_ip_mac_table_clicked(const QModelIndex &index)
{

}



void arpWidget::on_selectasAlice_clicked()
{
    int currentrow=ui->ip_mac_table->currentIndex().row();
    ui->aliceip->setText(tablemodel->item(currentrow,1)->text());
    this->alicemac=tablemodel->item(currentrow,2)->text();

    //ip store in lineedit,while mac in member;
}

void arpWidget::on_selectasTom_clicked()
{
    int currentrow=ui->ip_mac_table->currentIndex().row();
    ui->tomip->setText(tablemodel->item(currentrow,1)->text());
    this->tommac = tablemodel->item(currentrow,2)->text();
}

void arpWidget::on_startbutton_clicked()
{
    int typeindex = ui->attacktypeinput->currentIndex();
    Arpthread *arper = new Arpthread;
    arper->setTaskid(typeindex);
    arper->target_ip=ui->aliceip->text();
    arper->target_mac=this->alicemac;
    arper->gateway = QString::fromStdString(spyWidget::defaultgateway);
    arper->run();
}

void arpWidget::on_stopbutton_clicked()
{

}
