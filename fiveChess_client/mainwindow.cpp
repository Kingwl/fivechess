#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), state(state_behavior), playernow(SERVER_P),client(nullptr),is_connect(false)
{
    memset(map,0,sizeof(map));
    resize(640,480);
    playernow = CLIENT_P;
    map[1][2] = 1;
    map[2][3] = 2;
    client = new QTcpSocket(this);
    client->connectToHost("127.0.0.1",6000);
    if(client->isOpen())
    {
        client->write("#connect");
    }
    QObject::connect(client,SIGNAL(readyRead()),this,SLOT(readClient()));
}

MainWindow::~MainWindow()
{

}
void MainWindow::paintEvent( QPaintEvent * )
{
    QPainter p(this);
    for(int i = 0 ; i < 15 ; ++i)
    {
        p.drawLine(30, 30 + i * 30, 450, 30 + i * 30);
        p.drawLine(30 + i * 30, 30, 30 + i * 30, 450);
    }
    QBrush b;
    b.setStyle(Qt::SolidPattern);
    for(int i = 0 ; i < 15 ; ++i )
    {
        for(int j = 0 ; j < 15 ; ++j)
        {
            if(map[i][j] == SERVER_P)
            {
                b.setColor(Qt::white);

            }else if(map[i][j] == CLIENT_P){
                b.setColor(Qt::black);
            }else{
                continue;
            }
            p.setBrush(b);
            p.drawEllipse(point2pos(QPoint(i, j)), 15, 15);
        }

    }

}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QPoint t = e->pos();
    if(state == state_behavior) addPoint(pos2point(t));


}
QPoint MainWindow::pos2point(QPoint p)
{
    int x = (p.x() - 15)/30;
    int y = (p.y() - 15)/30;
    return QPoint(x,y);
}

QPoint MainWindow::point2pos(QPoint p)
{
    double x = p.x() * 30 + 30;
    double y = p.y() * 30 + 30;
    return QPoint(x,y);
}
void MainWindow::addPoint(QPoint p)
{
    if(map[p.x()][p.y()] != 0) return;
    map[p.x()][p.y()] = playernow;
    if(state == state_behavior){
        char c[100];
        sprintf(c,"#point:%2d,%2d",p.x(),p.y());
        client->write(c);
        state = state_listen;
    }else{
        state = state_behavior;
    }
    if(playernow == CLIENT_P)
        playernow = SERVER_P;
    else
        playernow = CLIENT_P;
    //QMessageBox::information(0,"","y");
    update();
}
void MainWindow::readClient()
{
    QString str = client->readAll();
    if(is_connect == false)
    {
        if(str == "#connected")
        {
            is_connect = true;
            QMessageBox::information(0,"","connected");
        }
    }
    else{
         if(state == state_listen){
            if(str.startsWith("#point:"))
            {
                int a,b;
                char*  ch;

                QByteArray ba = str.toLatin1();

                ch=ba.data();
                sscanf(ch,"#point:%d,%d",&a,&b);
                addPoint(QPoint(a,b));
            }
         }

    }
}
