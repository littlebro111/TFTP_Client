/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_1;
    QLabel *label_1;
    QLineEdit *LocalFilePath;
    QPushButton *ChooseLocalFile;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *RemoteFilePath;
    QPushButton *ChooseRemoteFile;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *ClientIP;
    QLabel *label_4;
    QLineEdit *ServerIP;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QComboBox *Mode;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *UploadButton;
    QPushButton *DownloadButton;
    QTextBrowser *output;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 600, 150));
        verticalLayoutWidget_2 = new QWidget(widget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 0, 581, 151));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_1 = new QHBoxLayout();
        horizontalLayout_1->setObjectName(QString::fromUtf8("horizontalLayout_1"));
        label_1 = new QLabel(verticalLayoutWidget_2);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(12);
        label_1->setFont(font);

        horizontalLayout_1->addWidget(label_1);

        LocalFilePath = new QLineEdit(verticalLayoutWidget_2);
        LocalFilePath->setObjectName(QString::fromUtf8("LocalFilePath"));

        horizontalLayout_1->addWidget(LocalFilePath);

        ChooseLocalFile = new QPushButton(verticalLayoutWidget_2);
        ChooseLocalFile->setObjectName(QString::fromUtf8("ChooseLocalFile"));
        ChooseLocalFile->setFont(font);

        horizontalLayout_1->addWidget(ChooseLocalFile);


        verticalLayout->addLayout(horizontalLayout_1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout_3->addWidget(label_2);

        RemoteFilePath = new QLineEdit(verticalLayoutWidget_2);
        RemoteFilePath->setObjectName(QString::fromUtf8("RemoteFilePath"));

        horizontalLayout_3->addWidget(RemoteFilePath);

        ChooseRemoteFile = new QPushButton(verticalLayoutWidget_2);
        ChooseRemoteFile->setObjectName(QString::fromUtf8("ChooseRemoteFile"));
        ChooseRemoteFile->setFont(font);

        horizontalLayout_3->addWidget(ChooseRemoteFile);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        horizontalLayout_4->addWidget(label_3);

        ClientIP = new QLineEdit(verticalLayoutWidget_2);
        ClientIP->setObjectName(QString::fromUtf8("ClientIP"));

        horizontalLayout_4->addWidget(ClientIP);

        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        horizontalLayout_4->addWidget(label_4);

        ServerIP = new QLineEdit(verticalLayoutWidget_2);
        ServerIP->setObjectName(QString::fromUtf8("ServerIP"));

        horizontalLayout_4->addWidget(ServerIP);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(verticalLayoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        horizontalLayout_5->addWidget(label_5);

        Mode = new QComboBox(verticalLayoutWidget_2);
        Mode->addItem(QString());
        Mode->addItem(QString());
        Mode->setObjectName(QString::fromUtf8("Mode"));
        Mode->setFont(font);

        horizontalLayout_5->addWidget(Mode);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        UploadButton = new QPushButton(verticalLayoutWidget_2);
        UploadButton->setObjectName(QString::fromUtf8("UploadButton"));
        UploadButton->setFont(font);

        horizontalLayout_5->addWidget(UploadButton);

        DownloadButton = new QPushButton(verticalLayoutWidget_2);
        DownloadButton->setObjectName(QString::fromUtf8("DownloadButton"));
        DownloadButton->setFont(font);

        horizontalLayout_5->addWidget(DownloadButton);


        verticalLayout->addLayout(horizontalLayout_5);

        output = new QTextBrowser(centralwidget);
        output->setObjectName(QString::fromUtf8("output"));
        output->setGeometry(QRect(2, 155, 596, 240));
        output->setFont(font);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_1->setText(QCoreApplication::translate("MainWindow", "\346\234\254\345\234\260\346\226\207\344\273\266:", nullptr));
        ChooseLocalFile->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\277\234\347\250\213\346\226\207\344\273\266:", nullptr));
        ChooseRemoteFile->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\256\242\346\210\267\347\253\257 IP:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\234\215\345\212\241\347\253\257 IP:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\344\274\240\350\276\223\346\250\241\345\274\217:", nullptr));
        Mode->setItemText(0, QCoreApplication::translate("MainWindow", "Netascii", nullptr));
        Mode->setItemText(1, QCoreApplication::translate("MainWindow", "Octet", nullptr));

        UploadButton->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240", nullptr));
        DownloadButton->setText(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
