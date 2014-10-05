#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), state(state_wait), playNow(NONE_P), is_connect(false), showPoint(500,100)
{
    resize(640, 480);
    memset(map,0,sizeof(map));
    socket = new QTcpSocket(this);
    socket -> connectToHost("1.25.132.156", 6000);
    QObject::connect(socket,SIGNAL(connected()), this, SLOT(sendRequest()));
}
MainWindow::~MainWindow()
{
    socket -> close();
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QPoint t = e -> pos();
    if(state == state_behavior) addPoint(pos2point(t));
}

void MainWindow::readClient()
{
    QString str = socket -> readAll();
    if(str.startsWith("#set"))
    {
        if(str == "#set:black")
        {
            CLIENT_COLOR = COLOR_B;
            SERVER_COLOR = COLOR_W;
            playNow = CLIENT_P;
            state = state_behavior;
            QMessageBox::information(0,"","play now");
        }else{
            CLIENT_COLOR = COLOR_W;
            CLIENT_COLOR = COLOR_B;
            playNow = SERVER_P;
            state = state_listen;
        }
    }else if(str.startsWith("#point")){
        int a, b;
        char* ch;
        QByteArray ba = str.toLatin1();
        ch = ba.data();
        sscanf_s(ch, "#point:%d,%d", &a, &b);
        if(state == state_listen) addPoint(QPoint(a, b));
    }
}

void MainWindow::paintEvent( QPaintEvent * )
{
    QPainter p(this);
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        p.drawLine(point2pos(QPoint(0, i)), point2pos(QPoint(MAX_SIZE - 1, i)));
        p.drawLine(point2pos(QPoint(i, 0)), point2pos(QPoint(i, MAX_SIZE - 1)));
    }
    QBrush b;
    b.setStyle(Qt::SolidPattern);
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        for(int j = 0; j < MAX_SIZE; ++j)
        {
            if(map[i][j] == COLOR_W)
            {
                b.setColor(Qt::white);

            }else if(map[i][j] == COLOR_B){
                b.setColor(Qt::black);
            }else{
                continue;
            }
            p.setBrush(b);
            p.drawEllipse(point2pos(QPoint(i, j)), R_SIZE, R_SIZE);
        }
    }
    int color = getNowPlayColor();
    if(color == COLOR_W){
        b.setColor(Qt::white);
    }else{
        b.setColor(Qt::black);
    }
    p.setBrush(b);
    p.drawEllipse(showPoint, R_SIZE, R_SIZE);
}

void MainWindow::changeState()
{
    if(state == state_behavior){
        state = state_listen;
    }else{
        state = state_behavior;
    }
}

void MainWindow::changePlayer()
{
    if(playNow == CLIENT_P)
    {
        playNow = SERVER_P;
    }else{
        playNow = CLIENT_P;
    }
}
void MainWindow::addPoint(const QPoint p)
{
    if(map[p.x()][p.y()] != 0) return;
    int color = getNowPlayColor();
    map[p.x()][p.y()] = color;
    if(state == state_behavior){
        char c[100];
        sprintf_s(c, "#point:%2d,%2d", p.x(), p.y());
        socket -> write(c);
    }
    changeState();
    changePlayer();
    update();
    int ch = check();
    if(ch != -1)
    {
        QMessageBox::information(0,"","end");
        state = state_wait;
    }

}
void MainWindow::sendRequest()
{
    QMessageBox::information(0, "", "connected");
    is_connect = true;
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readClient()));

}
QPoint MainWindow::pos2point(QPoint p)
{
    int x = (p.x() - 15) / 30;
    int y = (p.y() - 15) / 30;
    return QPoint(x, y);
}

QPoint MainWindow::point2pos(QPoint p)
{
    double x = p.x() * 30 + 30;
    double y = p.y() * 30 + 30;
    return QPoint(x, y);
}
int MainWindow::getNowPlayColor() const
{
    if(playNow == CLIENT_P) return CLIENT_COLOR;
    return SERVER_COLOR;
}
int MainWindow::check()
{
    for(int i = 0; i < MAX_SIZE ;i ++)
    {
        for(int j = 0; j < MAX_SIZE; j++)
        {
            if(map[i][j] == CLIENT_COLOR || map[i][j] == SERVER_COLOR)
            {
                int num = 0;
                int flag = map[i][j];
                for(int x = 1; x < 5; x++)
                {
                    if(is_in(i + x, j) == false) break;
                    if(map[i + x][j] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                for(int x = 1; x < 5; x++)
                {
                    if(is_in(i - x, j) == false) break;
                    if(map[i - x][j] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                if(num >= 4)    return flag;
                num = 0;
                for(int y = 1; y < 5; y++)
                {
                    if(is_in(i, j + y) == false) break;
                    if(map[i][j + y] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                for(int y = 1; y < 5; y++)
                {
                     if(is_in(i, j - y) == false) break;
                    if(map[i][j - y] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                if(num >= 4) return flag;
                num = 0;
                for(int x = 1; x < 5; x++)
                {
                    if(is_in(i + x, j + x) == false) break;
                    if(map[i + x][j + x] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                for(int x = 1; x < 5; x++)
                {
                    if(is_in(i - x, j - x) == false) break;
                    if(map[i - x][j - x] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                if(num >= 4)    return flag;
                num = 0;
                for(int x = 1; x < 5; x++)
                {
                    if(is_in(i + x, j - x) == false) break;
                    if(map[i + x][j - x] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                for(int x = 1; x < 5; x++)
                {
                    if(is_in(i - x, j + x) == false) break;
                    if(map[i - x][j + x] == flag){
                        num++;
                    }else{
                        break;
                    }
                }
                if(num >= 4)    return flag;
            }
        }
    }
    return -1;
}
bool MainWindow::is_in(int x , int y)
{
    if(x < 0 || y < 0 || x >= MAX_SIZE || y >= MAX_SIZE) return false;
    return true;
}
