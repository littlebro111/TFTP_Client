#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QString"
#include "QFileDialog"
#include "QDateTime"
#include <QMainWindow>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#define CLIENTPORT 6666
#define TIMEOUT 3000
#define RETRANSMISSION 5

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    void InitWinsock();
    void SocketCreate();
    void SocketClose();

private slots:
    void on_ChooseLocalFile_pressed();
    void on_ChooseRemoteFile_pressed();
    void on_UploadButton_pressed();
    void on_DownloadButton_pressed();

private:
    Ui::MainWindow *ui;
};

// TFTP数据包结构体
struct tftpPacket {
    unsigned short cmd;
    union {
        unsigned short code;
        unsigned short block;
        char filename[2];
    };
    char data[512];
};

using namespace std;

#endif // MAINWINDOW_H
