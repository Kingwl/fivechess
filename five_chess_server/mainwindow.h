#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QtNetwork>
#include <QMessageBox>
#include <QMouseEvent>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int MAX_SIZE = 15;
    const int COLOR_B = 1;
    const int COLOR_W = 2;
    const int NONE_P = 0;
    const int CLIENT_P = 1;
    const int SERVER_P = 2;
    const int state_wait = 0;
    const int state_listen = 1;
    const int state_behavior = 2;
    const int R_SIZE = 15;
    int SERVER_COLOR;
    int CLIENT_COLOR;
    int state;
    int playNow;
    bool is_connect;
    void paintEvent( QPaintEvent * );

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPoint pos2point(const QPoint p);
    QPoint point2pos(const QPoint p);
    void changeState();
    void changePlayer();
    void addPoint(const QPoint p);
    QPoint showPoint;
    int getNowPlayColor() const;
    int check();
    bool is_in(int x , int y);
public slots:
    void mousePressEvent(QMouseEvent *e);
    void readClient();
    void sendRequest();
    void acceptConnection();
private:
    int map[MAX_SIZE][MAX_SIZE];
    QTcpSocket *socket;
    QTcpServer *server;

};

#endif // MAINWINDOW_H
