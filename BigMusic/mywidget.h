#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QStringList>//字符串的列表类头文件
#include<QDir>//文件路径头文件
#include<QTableWidgetItem>
namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
protected:
    void paintEvent(QPaintEvent *);//画背景图
private slots:
    void init();//初始化列表
    QStringList getFileNames(const QString &path);//获取文件路径
    void addItem(QString name);//将文件显示到界面
    void init_connect();//初始接口

    void on_PlayButton_clicked();

    void on_LastButton_clicked();

    void on_NextButton_clicked();

    void on_songlabel_linkActivated();

    void on_ModeButton_clicked();
    
    void on_doubleclicked_musiclist(QTableWidgetItem *);//选中播放

    void on_VolumeButton_cliked();//显示调整音量条
    
    void on_ScanMusicButton_clicked();//添加歌曲

    void on_volumeValue_linkActivated();//音量值显示变化


public:
    void updatePosition(qint64 position);//更新播放位置

    void changeVolume();//改变音量

private:
    Ui::MyWidget *ui;
    QString MusicPath="E:\\song\\老歌";
    QMediaPlaylist *PlayList;
    QMediaPlayer *Player;
    QStringList fileList;//文件列表
    bool Playing=false;//播放状态
    int volumevalue=100;//音量值
    int count;//音乐列表行

};

#endif // MYWIDGET_H
