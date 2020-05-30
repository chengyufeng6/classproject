#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QStringList>//字符串的列表类头文件
#include<QDir>//文件路径头文件
#include<QTableWidgetItem>
#include"lyricwidget.h"
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

    void on_LycButton_cliked();//显示歌词

public:
    void updatePosition(qint64 position);//更新播放位置

    void changeVolume();//改变音量

    void updateSongLrc(qint64 position);

    void updateSongLyclist();

private:
    Ui::MyWidget *ui;
    LyricWidget *Lyric=new LyricWidget(this);//创建歌词窗口指针
    QString MusicPath="E:\\song\\bigMusic";
    QMediaPlaylist *PlayList;
    QMediaPlayer *Player;
    QStringList fileList;//文件列表
    bool Playing=false;//播放状态
    int volumevalue=100;//音量值
    int count;//音乐列表行

    // 让滚动条变得好看的样式表
    QString listWidgetStyle = "QScrollBar:vertical"
                                          "{"
                                                  "width:8px;"
                                                  "background-color:transparent;"
                                                  "margin:0px,0px,0px,0px;"
                                                 " padding-top:12px;   /*上预留位置*/"
                                                 " padding-bottom:12px;    /*下预留位置*/"
                                              "}"


                                            "  QScrollBar::handle:vertical"
                                             " {"
                                                  "width:8px;"
                                                 " background-color:rgba(255,255,255,0.2);"
                                                 " border-radius:4px;"
                                                 " min-height:20px;"
                                             " }"


                                          "    QScrollBar::handle:vertical:hover"
                                           "   {"
                                            "      width:9px;"
                                                 " background-color:rgba(255,255,255,0.5);"
                                               "   border-radius:4px;"
                                             "     min-height:20;"
                                            "  }"

                                            "  QScrollBar::add-line:vertical"
                                            "  {"
                                                 " height:12px;"
                                             "     width:10px;"
                                          "        border-image:url(:/selectfile/scroll/3.png);"
                                                 " subcontrol-position:bottom;"
                                          "    }"

                                          "    QScrollBar::sub-line:vertical"
                                          "    {"
                                          "        height:12px;"
                                                  "width:10px;"
                                               "   border-image:url(:/selectfile/scroll/1.png);"
                                                  "subcontrol-position:top;"
                                           "   }"
                                          "    QScrollBar::add-line:vertical:hover"
                                           "   {"
                                                 " height:12px;"
                                                  "width:10px;"
                                                 " border-image:url(:/selectfile/scroll/4.png);"
                                                  "subcontrol-position:bottom;"
                                             " }"


                                             " QScrollBar::sub-line:vertical:hover"
                                             " {"
                                             "     height:12px;"
                                                "  width:10px;"
                                                "  border-image:url(:/selectfile/scroll/2.png);"
                                                 " subcontrol-position:top;"
                                             " }"

                                             " QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                             " {"
                                                 " background-color:transparent;"
                                               "   border-radius:4px;"
                                              "}";


};

#endif // MYWIDGET_H
