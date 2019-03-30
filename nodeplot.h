#ifndef NodePlot_H
#define NodePlot_H
#include <QMainWindow>
#include <QThread>
#include <QVector>


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



class showNodes: public QThread{
    Q_OBJECT
public:
//    explicit showNodes();
    void run();

signals:
    void flushNodes();
};

#endif // NodePlot_H
