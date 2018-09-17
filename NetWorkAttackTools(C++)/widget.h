#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTabWidget>
#include<QMap>
#include<string>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    static QMap<std::string,std::string> myarp;
    static std::string gatewaymac;
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QTabWidget *tabWidget;

private slots:
    void hey();

};

#endif // WIDGET_H
