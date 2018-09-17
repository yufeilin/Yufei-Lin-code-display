#ifndef ARPWIDGET_H
#define ARPWIDGET_H

#include <QWidget>
#include<QStandardItemModel>


namespace Ui {
class arpWidget;
}

class arpWidget : public QWidget
{
    Q_OBJECT

public:

    QString alicemac;
    QString tommac;

    //table row count;
    int count;

    QStandardItemModel *tablemodel;
    explicit arpWidget(QWidget *parent = 0);

    ~arpWidget();

private slots:
    void on_attacktypeinput_currentIndexChanged(int index);

    void on_refresh_clicked();


    void scanfinish();


    void on_ip_mac_table_activated(const QModelIndex &index);

    void on_ip_mac_table_clicked(const QModelIndex &index);

    void on_selectasAlice_clicked();

    void on_selectasTom_clicked();

    void on_startbutton_clicked();

    void on_stopbutton_clicked();

signals:
     void arpscan();

private:
    Ui::arpWidget *ui;
    void addtoList();
};

#endif // ARPWIDGET_H
