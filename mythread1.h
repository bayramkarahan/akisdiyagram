#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QtCore>
#include<diagram.h>
#include<diagramscene.h>
#include<QTimer>
class MyThread:public QThread
{

public:
    MyThread()
    {
timer = new QTimer(this);
QObject::connect(timer, SIGNAL(timeout()), this, SLOT(zamanlama()));


    }
    QTimer *timer;
    int index=0;
    void run()
    {
        if (index==0) timer->start();

     }

    QList<DiagramItem*> diagramItem;
   DiagramScene *scene;




public slots:
   void worker()
   {



   };
    void zamanlama()
    {

        diagramItem[index]->setText("stop",QColor(255,255,0,255));
        qDebug()<<diagramItem[index]->myDiagramType;
        scene->update();
        index++;
       // quit();
       // sleep(3);

        //wait();
        if (diagramItem.length()==index) timer->stop();
    };
protected:
private:

};

#endif // MYTHREAD_H
