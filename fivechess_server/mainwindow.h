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
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent( QPaintEvent * );
    QPoint pos2point(QPoint p);
    QPoint point2pos(QPoint p);
    void addPoint(QPoint p);
    bool is_connect;
public slots:
    void mousePressEvent(QMouseEvent *e);
    void acceptConnection();
    void readClient();
private:
    int map[MAX_SIZE][MAX_SIZE];
    int state;
    int playernow ;
    QTcpSocket *socket;
    QTcpServer *server;
};

#endif // MAINWINDOW_H
