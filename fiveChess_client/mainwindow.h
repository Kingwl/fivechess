#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QtNetwork>
#include <QMouseEvent>
#include <QMessageBox>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int MAX_SIZE = 15;
    static const int COLOR_B = 1;
    static const int COLOR_W = 2;
    static const int CLIENT_P = COLOR_B;
    static const int SERVER_P = COLOR_W;
    static const int state_listen = 0;
    static const int state_behavior = 1;
    bool is_connect;
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent( QPaintEvent * );
    QPoint pos2point(QPoint p);
    QPoint point2pos(QPoint p);
    void addPoint(QPoint p);
public slots:
    void mousePressEvent(QMouseEvent *e);
    void readClient();
private:
    int map[MAX_SIZE][MAX_SIZE];
    int state;
    int playernow ;
    QTcpSocket *client;

};

#endif // MAINWINDOW_H
