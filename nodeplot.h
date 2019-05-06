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
    void calc(double x, double y, double z, double* array);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

public slots:
    void on_FlushNodes();
//    void on_item_selected()
    void triggerSelection();
private:
    Ui::NodePlot *ui;
//    QTimer *m_selectionTimer;
//    CustomInputHandler* m_inputHandler;
};



class showNodes: public QThread{
    Q_OBJECT
public:
    void run();

signals:
    void flushNodes();
};

#endif // NodePlot_H
