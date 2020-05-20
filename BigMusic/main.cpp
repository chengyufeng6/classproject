#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    w.setWindowIcon(QIcon(":/music.ico"));
    //w.setMaximumSize(1000,1000);// 在ui中设置更方便
    w.setMinimumSize(0,0);//窗口最小
    w.show();
    return a.exec();
}
