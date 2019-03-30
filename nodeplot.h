#ifndef NodePlot_H
#define NodePlot_H


#include <QMainWindow>
#include <QThread>


namespace Ui {
class NodePlot;
}

class NodePlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit NodePlot(QWidget *parent = nullptr);
//    NodePlot();
    ~NodePlot();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public slots:
//    void on_OutputReceived(QString qs);
    void on_FlushNodes();
private:
    Ui::NodePlot *ui;

};

class node:public QThread{
    Q_OBJECT
public:
    void run();
    double getx();
    double gety();
    double getv();
    double getd();
    void setx(double x);
    void sety(double y);
    void setv(double v);
    void setd(double d);
    void update(double time);
    void reflect(int err);
    //node(QObject *parent=NULL);
    node(int n);
//signals:
//    void output(QString qs);
private:
    double x,y;
    double v;
    double d;
    int lastErr = 0;
    int id;
    bool first = true;
};

class showNodes: public QThread{
    Q_OBJECT
public:
//    explicit showNodes();
    void run();

signals:
    void flushNodes();
};

#endif // NodePlot_H
