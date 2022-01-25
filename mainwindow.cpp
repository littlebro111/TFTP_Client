#include "mainwindow.h"
#include "ui_mainwindow.h"

SYSTEMTIME st;
FILE* LogFile;
SOCKET ClientSocket;
SOCKADDR_IN ServerAddr, ClientAddr;
unsigned int addr_length;
char* ClientIP, * ServerIP;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SocketCreate(){
    // 初始化Winsock
    WORD wdVersion = MAKEWORD(2, 2);
    WSADATA wdSockMsg;
    if (WSAStartup(wdVersion, &wdSockMsg) != 0) {
        ui->output->append("Failed to initialize Winsock!");
        WSACleanup();
        return;
    }
//    ui->output->append("Succeed to initialize Winsock!");

    // 创建客户端socket，并设置为非阻塞模式
    unsigned long mode = 1;
    ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ioctlsocket(ClientSocket, FIONBIO, &mode);
    if (ClientSocket == INVALID_SOCKET) {
        ui->output->append("Failed to create socket!");
        closesocket(ClientSocket);
        WSACleanup();
        return;
    }
//    ui->output->append("Succeed to create socket!");

    ClientAddr.sin_family = AF_INET;
    ClientAddr.sin_port = htons(CLIENTPORT);
    ClientAddr.sin_addr.S_un.S_addr = inet_addr(ClientIP);
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(69);
    ServerAddr.sin_addr.S_un.S_addr = inet_addr(ServerIP);
    addr_length = sizeof(struct sockaddr_in);

    // 绑定socket
    if (bind(ClientSocket, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR) {
        ui->output->append("Failed to bind socket!");
        closesocket(ClientSocket);
        WSACleanup();
        return;
    }
//    ui->output->append("Succeed to bind socket!");
}

void MainWindow::SocketClose() {
    closesocket(ClientSocket);
    WSACleanup();
}

void MainWindow::on_ChooseLocalFile_pressed(){
    QString PathName = QFileDialog::getOpenFileName(this, "选择本地文件", "E:/", tr("file(*)"));
    ui->LocalFilePath->setText(PathName);
}

void MainWindow::on_ChooseRemoteFile_pressed(){
    QString PathName = QFileDialog::getOpenFileName(this, "选择远程文件", "E:/Tftpd64/", tr("file(*)"));
    ui->RemoteFilePath->setText(PathName);
}

void MainWindow::on_UploadButton_pressed(){
    QByteArray Qfilename = ui->LocalFilePath->text().toLatin1();
    QByteArray QserverIP = ui->ServerIP->text().toLatin1();
    QByteArray QclientIP = ui->ClientIP->text().toLatin1();
    char * FilePath = Qfilename.data();
    char buf[512], filename[512];
    ServerIP = QserverIP.data();
    ClientIP = QclientIP.data();
    int choice, wait, size, count, resent = 0;
    tftpPacket sendPacket, recvPacket;
    sockaddr_in NewServerAddr;
    LogFile = fopen("tftp.log", "a");
    ui->output->append("****************upload****************");
    QCoreApplication::processEvents();
    SocketCreate();

    // 取文件名
    int temp = 0;
    for(int i = 0; FilePath[i] != '\0'; ++i, ++temp){
        if(FilePath[i] == '/'){
            ++i;
            temp = 0;
        }
        filename[temp] = FilePath[i];
    }
    filename[temp] = '\0';

    // 客户端发送WRQ包并等待接收服务器端的ACK响应
    sendPacket.cmd = htons(2);
    choice = ui->Mode->currentIndex();
    sprintf(buf, "传输文件的数据类型选择为：%s", choice == 0 ? "netascii" : "octet");
    ui->output->append(buf);
    QCoreApplication::processEvents();
    if (choice == 0) sprintf(sendPacket.filename, "%s%c%s%c", filename, 0, "netascii", 0);
    else sprintf(sendPacket.filename, "%s%c%s%c", filename, 0, "octet", 0);
    for (count = 0; count < RETRANSMISSION; ++count) {
        sendto(ClientSocket, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&ServerAddr, addr_length);
        ui->output->append("正在向服务器发送上传请求...");
        QCoreApplication::processEvents();
        for (wait = 0; wait < TIMEOUT; wait += 100) {
            size = recvfrom(ClientSocket, (char*)&recvPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&NewServerAddr, (int*)&addr_length);
            if (size > 0 && size < 4){
                sprintf(buf, "数据包损坏：size = %d", size);
                ui->output->append(buf);
                QCoreApplication::processEvents();
            }
            if (size >= 4 && recvPacket.cmd == htons(4) && recvPacket.block == htons(0)) break;
            Sleep(100);
        }
        if (wait < TIMEOUT) {
            ui->output->append("连接成功！");
            QCoreApplication::processEvents();
            break;
        }else {
            ui->output->append("连接失败，正在重试...");
            QCoreApplication::processEvents();
            char time[70] = { 0 }, log[512] = { 0 };
            GetLocalTime(&st);
            sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            sprintf_s(log, "%s WARN: upload %s, Mode: %s, Details: Connect timeout!\n", time, filename, choice == 0 ? "netascii" : "octet");
            fwrite(log, 1, strlen(log), LogFile);
            ++resent;
        }
    }
    if (count >= RETRANSMISSION) {
        ui->output->append("连接失败！");
        ui->output->append("**************************************");
        char time[70] = { 0 }, log[512] = { 0 };
        GetLocalTime(&st);
        sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        sprintf_s(log, "%s ERROR: upload %s, Mode: %s, Details: Connect failure!\n", time, filename, choice == 0 ? "netascii" : "octet");
        fwrite(log, 1, strlen(log), LogFile);
        fclose(LogFile);
        SocketClose();
        return;
    }

    // 打开待上传文件
    FILE* file = NULL;
    if (choice == 0) file = fopen(FilePath, "r");
    else file = fopen(FilePath, "rb");
    if (file == NULL) {
        ui->output->append("文件不存在！");
        ui->output->append("**************************************");
        char time[70] = { 0 }, log[512] = { 0 };
        GetLocalTime(&st);
        sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        sprintf_s(log, "%s ERROR: upload %s, Mode: %s, Details: File not exists!\n", time, filename, choice == 0 ? "netascii" : "octet");
        fwrite(log, 1, strlen(log), LogFile);
        fclose(LogFile);
        SocketClose();
        return;
    }

    // 发送数据
    unsigned short block = 1;
    int datasize = 0;
    double bytes = 0;
    clock_t start, end;
    sendPacket.cmd = htons(3);
    start = clock();
    do {
        memset(sendPacket.data, 0, sizeof(sendPacket.data));
        sendPacket.block = htons(block);
        datasize = fread(sendPacket.data, 1, 512, file);
        bytes += datasize;
        for (count = 0; count < RETRANSMISSION; ++count) {
            sendto(ClientSocket, (char*)&sendPacket, datasize + 4, 0, (struct sockaddr*)&NewServerAddr, addr_length);
            sprintf(buf, "正在发送第%d块数据块...", block);
            ui->output->append(buf);
            QCoreApplication::processEvents();
            for (wait = 0; wait < TIMEOUT; wait += 100) {
                size = recvfrom(ClientSocket, (char*)&recvPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&NewServerAddr, (int*)&addr_length);
                if (size > 0 && size < 4){
                    sprintf(buf, "第%d个数据包损坏：size = %d", block, size);
                    ui->output->append(buf);
                    QCoreApplication::processEvents();
                }
                if (size >= 4 && recvPacket.cmd == htons(4) && recvPacket.block == htons(block)) break;
                Sleep(100);
            }
            if (wait < TIMEOUT) break;
            else {
                sprintf(buf, "第%d块数据块发送失败，正在重传...", block);
                ui->output->append(buf);
                QCoreApplication::processEvents();
                char time[70] = { 0 }, log[512] = { 0 };
                GetLocalTime(&st);
                sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
                sprintf_s(log, "%s WARN: upload %s, Mode: %s, Details: %dth data upload timeout!\n",
                          time, filename, choice == 0 ? "netascii" : "octet", block);
                fwrite(log, 1, strlen(log), LogFile);
                ++resent;
            }
        }
        if (count >= RETRANSMISSION) {
            sprintf(buf, "第%d块数据块发送失败！", block);
            ui->output->append(buf);
            ui->output->append("**************************************");
            char time[70] = { 0 }, log[512] = { 0 };
            GetLocalTime(&st);
            sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            sprintf_s(log, "%s ERROR: upload %s, Mode: %s, Details: %dth data upload failure!\n",
                      time, filename, choice == 0 ? "netascii" : "octet", block);
            fwrite(log, 1, strlen(log), LogFile);
            fclose(LogFile);
            fclose(file);
            SocketClose();
            return;
        }
        ++block;
    } while (datasize == 512);
    end = clock();
    ui->output->append("文件上传成功！");
    fclose(file);

    // 计算相关数据
    double TotalTime = (double)(end - start) / CLK_TCK;
    double LossRate = (double)resent / (resent + block - 1);
    sprintf(buf, "上传文件总大小：%.2f KB", bytes / 1024.0);
    ui->output->append(buf);
    sprintf(buf, "上传文件总时间：%.2f s", TotalTime);
    ui->output->append(buf);
    sprintf(buf, "上传文件总速率：%.2f KB/s", bytes / (1024.0 * TotalTime));
    ui->output->append(buf);
    sprintf(buf, "上传文件丢包数：%d，丢包率：%.2f%%", resent, 100 * LossRate);
    ui->output->append(buf);
    ui->output->append("**************************************");

    // 写入日志
    char time[70] = { 0 }, log[512] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    sprintf_s(log, "%s INFO: upload %s, Mode: %s, Size: %.2f KB, Time: %.2f s, Loss rate: %.2f%%\n",
              time, filename, choice == 0 ? "netascii" : "octet", bytes / 1024.0, TotalTime, 100 * LossRate);
    fwrite(log, 1, strlen(log), LogFile);
    fclose(LogFile);
    SocketClose();
}

void MainWindow::on_DownloadButton_pressed(){
    QByteArray QlocalFile = ui->LocalFilePath->text().toLatin1();
    QByteArray QremoteFile = ui->RemoteFilePath->text().toLatin1();
    QByteArray QserverIP = ui->ServerIP->text().toLatin1();
    QByteArray QclientIP = ui->ClientIP->text().toLatin1();
    char * LocalFile = QlocalFile.data();
    char * RemoteFile = QremoteFile.data();
    if (strstr(RemoteFile, "E:/Tftpd64/") || strstr(RemoteFile, "e:/Tftpd64/")) RemoteFile += 11;
    ServerIP = QserverIP.data();
    ClientIP = QclientIP.data();
    char buf[512];
    int choice, wait, resent = 0;
    tftpPacket sendPacket, recvPacket;
    sockaddr_in NewServerAddr;
    SocketCreate();

    ui->output->append("***************download***************");
    sprintf_s(buf, "远程文件地址：%s", RemoteFile);
    ui->output->append(buf);
    sprintf_s(buf, "本地文件地址：%s", LocalFile);
    ui->output->append(buf);
    QCoreApplication::processEvents();
    LogFile = fopen("tftp.log", "a");

    // 客户端发送RRQ包
    sendPacket.cmd = htons(1);
    choice = ui->Mode->currentIndex();
    if (choice == 0) sprintf(sendPacket.filename, "%s%c%s%c", RemoteFile, 0, "netascii", 0);
    else sprintf(sendPacket.filename, "%s%c%s%c", RemoteFile, 0, "octet", 0);
    sendto(ClientSocket, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&ServerAddr, addr_length);

    // 创建本地写入文件
    FILE* file = NULL;
    if (choice == 0) file = fopen(LocalFile, "w");
    else file = fopen(LocalFile, "wb");
    if (file == NULL) {
        ui->output->append("创建本地文件失败！");
        ui->output->append("**************************************");
        char time[70] = { 0 }, log[512] = { 0 };
        GetLocalTime(&st);
        sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        sprintf_s(log, "%s ERROR: download %s to %s, Mode: %s, Details: Fail to create file!\n",
                  time, RemoteFile, LocalFile, choice == 1 ? "netascii" : "octet");
        fwrite(log, 1, strlen(log), LogFile);
        fclose(LogFile);
        SocketClose();
        return;
    }

    // 接收数据
    unsigned short block = 1;
    int datasize;
    double bytes = 0;
    clock_t start, end;
    sendPacket.cmd = htons(4);
    start = clock();
    do {
        for (wait = 0; wait < TIMEOUT * RETRANSMISSION; wait += 100) {
            datasize = recvfrom(ClientSocket, (char*)&recvPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&NewServerAddr, (int*)&addr_length);
            if (wait == TIMEOUT && block == 1) {
                ui->output->append("连接失败！");
                ui->output->append("**************************************");
                char time[70] = { 0 }, log[512] = { 0 };
                GetLocalTime(&st);
                sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
                sprintf_s(log, "%s Error: download %s to %s, Mode: %s, Details: Connect failure!\n",
                          time, RemoteFile, LocalFile, choice == 0 ? "netascii" : "octet");
                fwrite(log, 1, strlen(log), LogFile);
                fclose(file);
                fclose(LogFile);
                SocketClose();
                return;
            }
            if (datasize > 0 && datasize < 4){
                sprintf(buf, "第%d个数据包损坏：size = %d", block, datasize);
                ui->output->append(buf);
                QCoreApplication::processEvents();
            }else if (datasize >= 4 && recvPacket.cmd == htons(3) && recvPacket.block == htons(block)) {
                sprintf(buf, "正在接收第%d块数据块...", block);
                ui->output->append(buf);
                QCoreApplication::processEvents();
                sendPacket.block = recvPacket.block;
                sendto(ClientSocket, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&NewServerAddr, addr_length);
                fwrite(recvPacket.data, 1, datasize - 4, file);
                break;
            }else {
                if (wait != 0 && wait % TIMEOUT == 0) {
                    sprintf(buf, "第%d块数据块接收超时，正在重试...", block);
                    ui->output->append(buf);
                    QCoreApplication::processEvents();
                    sendto(ClientSocket, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&NewServerAddr, addr_length);
                    char time[70] = { 0 }, log[512] = { 0 };
                    GetLocalTime(&st);
                    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
                    sprintf_s(log, "%s WARN: download %s to %s, Mode: %s, Details: %dth data download timeout!\n",
                              time, RemoteFile, LocalFile, choice == 0 ? "netascii" : "octet", block);
                    fwrite(log, 1, strlen(log), LogFile);
                    ++resent;
                }
            }
            Sleep(100);
        }
        bytes += datasize - 4;
        if (wait >= TIMEOUT * RETRANSMISSION) {
            sprintf(buf, "第%d数据块下载失败！", block);
            ui->output->append(buf);
            ui->output->append("**************************************");
            char time[70] = { 0 }, log[512] = { 0 };
            GetLocalTime(&st);
            sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            sprintf_s(log, "%s Error: download %s to %s, Mode: %s, Details: %dth data download failure!\n",
                      time, RemoteFile, LocalFile, choice == 0 ? "netascii" : "octet", block);
            fwrite(log, 1, strlen(log), LogFile);
            fclose(file);
            fclose(LogFile);
            SocketClose();
            return;
        }
        ++block;
    } while (datasize == 516);
    end = clock();

    // 计算相关数据
    double TotalTime = (double)(end - start) / CLK_TCK;
    double LossRate = (double)resent / (resent + block - 1);
    sprintf(buf, "下载文件总大小：%.2f KB", bytes / 1024.0);
    ui->output->append(buf);
    sprintf(buf, "下载文件总时间：%.2f s", TotalTime);
    ui->output->append(buf);
    sprintf(buf, "下载文件总速率：%.2f KB/s", bytes / (1024.0 * TotalTime));
    ui->output->append(buf);
    sprintf(buf, "下载文件丢包数：%d，丢包率：%.2f%%", resent, 100 * LossRate);
    ui->output->append(buf);
    ui->output->append("**************************************");
    fclose(file);

    // 写入日志
    char time[70] = { 0 }, log[512] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    sprintf_s(log, "%s INFO: download %s to %s, Mode: %s, Size: %.2f KB, Time: %.2f s, Loss rate: %.2f%%\n",
              time, RemoteFile, LocalFile, choice == 0 ? "netascii" : "octet", bytes / 1024.0, TotalTime, 100 * LossRate);
    fwrite(log, 1, strlen(log), LogFile);
    fclose(LogFile);
    SocketClose();
}
