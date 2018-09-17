#ifndef SPYDIALOG_H
#define SPYDIALOG_H

#include <QDialog>
#include<QTabWidget>
#include<QComboBox>
#include<QTableView>
#include<QStandardItemModel>
#include<QTextBrowser>

namespace Ui {
class spyDialog;
}

class spyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit spyDialog(QWidget *parent = 0);

    void addNIClist();

    void getNIClist();
    ~spyDialog();

private:

    Ui::spyDialog *ui;

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

};

#endif // SPYDIALOG_H
