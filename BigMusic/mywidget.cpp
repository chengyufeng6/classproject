#include "mywidget.h"
#include "ui_mywidget.h"
#include<QMediaPlayer>
#include<QPainter>//设置背景图
#include<QDebug>// 测试
#include<QStandardPaths>//文件路径
#include<QFileDialog>//调用本地添加文件窗口
#include<QFileInfo>//文件信息
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    init();
    ui->PlayButton->setIcon(QIcon(":/play.ico"));//设置 播放图标
    ui->PlayButton->setToolTip("播放");
    ui->songlabel->setText(fileList.first());// 设置显示第一首歌名
    ui->songlabel->setStyleSheet("color:rgba(85, 255, 127, 1);"
                                 "font-size:20px;"
                                 "font-family:华文楷体;");   //设置歌曲样式表 rgba  a为alpha(透明度) 0/1 为完全透明/不透明
    ui->ModeButton->setIcon(QIcon(":/indexcycle.png"));
    ui->ModeButton->setToolTip("顺序循环模式");
    ui->VolumeButton->setIcon(QIcon(":/volume.png"));
    ui->VolumeSlider->setMaximum(100);//设置最大值为100
    ui->VolumeSlider->setSliderPosition(volumevalue);//设置滑块初始位置等同在volumevalue处
    ui->VolumeSlider->hide();//音量条初始隐藏
    ui->volumeValue->hide();//音量值初始隐藏
    ui->volumeValue->setStyleSheet("color:rgb(122,255,12)");//音量值颜色
    ui->timelabel->setStyleSheet("color:rgb(200,250,10)");//时间进度标签颜色
    ui->LastButton->setIcon(QIcon(":/last.png"));//上一曲icon
    ui->NextButton->setIcon(QIcon(":/next.png"));//下一曲icon
    init_connect();
}

inline void MyWidget::init_connect()//初始信号与槽 设为inline效率更快
{
    /*注意 在ui设计界面，设计信号和槽时，会把信息存在ui文件中，在编译时会在成成的头文件**.h 中把连接代码加
     * 所以在ui中直接转到槽的槽函数会自动连接，不需要connect 但是为了方便观看，仍然写出connect
     * 并且根据实验自己再次connect会导致连接失效，按钮有时会不起作用
     * 详细信息参考https://blog.csdn.net/u013378306/article/details/52431826
     */
    //connect(ui->PlayButton,&QPushButton::clicked,this,&MyWidget::on_PlayButton_clicked);//播放
    //connect(ui->LastButton,&QPushButton::clicked,this,&MyWidget::on_LastButton_clicked);//上一曲
    //connect(ui->NextButton,&QPushButton::clicked,this,&MyWidget::on_NextButton_clicked);//下一曲
    connect(ui->ModeButton,&QPushButton::clicked,this,&MyWidget::on_ModeButton_clicked);//改变播放模式
    connect(Player,&QMediaPlayer::currentMediaChanged,this,&MyWidget::on_songlabel_linkActivated);//显示当前播放曲名
    connect(ui->MusicList,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(on_doubleclicked_musiclist(QTableWidgetItem*)));//选中播放
    connect(ui->VolumeButton,&QPushButton::clicked,this,&MyWidget::on_VolumeButton_cliked);//点击音量键显示 并改变/不改变音量
    connect(ui->VolumeSlider,&QSlider::valueChanged, this,&MyWidget::changeVolume);//通过拖动音量条改变音量
    connect(ui->VolumeSlider,&QSlider::valueChanged,this,&MyWidget::on_volumeValue_linkActivated);//实时音量数字显示
    connect(Player,&QMediaPlayer::positionChanged,this,&MyWidget::updatePosition);//播放的位置改变->更新进度条播放位置
    connect(ui->SliderDuration,&QSlider::sliderMoved,Player,&QMediaPlayer::setPosition);//进度条移动->更新音乐的播放位置
}

void MyWidget::paintEvent(QPaintEvent *)//会自动调用
{
    QPainter p(this);//创建画家对象，指定当前窗口为绘图设备
    //p.drawPixmap(0, 0, width(), height(), QPixmap("../Image/background4.jpg"));//设置背景图  width(), height()会自动获取窗口宽高
    p.drawPixmap(rect(),QPixmap("../Image/background4.jpg")); //此句等同上句子
}

void MyWidget::init()//初始化函数接口
{
    PlayList=new QMediaPlaylist;//实例化播放列表
    Player=new QMediaPlayer;
    fileList=getFileNames(this->MusicPath);//获取文件夹下所有音乐文件
    //qDebug()<<"所有音乐 \n"<<fileList;
    for(int i=0;i<fileList.size();i++)
    {
        QString filename=fileList.at(i);//第i个文件
        addItem(filename);
        PlayList->addMedia(QUrl::fromLocalFile(MusicPath+"\\"+filename));
    }
    qDebug()<<"原来列表媒体数"<<PlayList->mediaCount();
     PlayList->setCurrentIndex(0);//在播放列表位置激活媒体内容
     PlayList->setPlaybackMode(QMediaPlaylist::Loop);//顺序
     Player->setPlaylist(PlayList);
     Player->setVolume(volumevalue);//初始音量
}

 QStringList MyWidget::getFileNames(const QString &path)//获取多文件名
 {
     QDir dir(path);
     QStringList nameFilters;
     nameFilters<<"*.mp3";
     QStringList files=dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Name); //获取文件信息
     return files;
 }

 void MyWidget::addItem(QString name)//将文件显示到界面
 {
     QStringList header;//设置表头
     header<<"歌曲名";
     ui->MusicList->setColumnCount(1);//设置1列
     ui->MusicList->setHorizontalHeaderLabels(header);
     int count=ui->MusicList->rowCount();//默认行数为0
    ui->MusicList->setRowCount(count+1);//调整表格的行数
     QTableWidgetItem *itemName=new QTableWidgetItem(name);
     itemName->setBackgroundColor(QColor(20,0,60));//设置背景颜色
     itemName->setTextColor(QColor(200,111,100));//设置文本颜色
     itemName->setFont(QFont("Helvetica"));//设置字体
     ui->MusicList->setItem(count,0,itemName);
 }

 void MyWidget::on_PlayButton_clicked()//播放 &&暂停
 {
    if(!Playing)
    {
        Playing=true;
        Player->play();
        //qDebug()<<"playing\n";
        ui->PlayButton->setIcon(QIcon(":/pause.ico"));
        ui->PlayButton->setToolTip("暂停");
    }
    else
    {
        Playing=false;
        if(Player->PlayingState==1)
        Player->pause();
        //qDebug()<<"pauseing\n";
        ui->PlayButton->setIcon(QIcon(":/play.ico"));
        ui->PlayButton->setToolTip("播放");
    }
 }

void MyWidget::on_LastButton_clicked()//上一曲
{

    PlayList->previous();//previous 以前的
    PlayList->previousIndex();
}

void MyWidget::on_NextButton_clicked()//下一曲
{

    PlayList->next();//进入播放列表中的下一个媒体内容
    PlayList->nextIndex();//返回此项索引
}

void MyWidget::on_songlabel_linkActivated()//显示当前播放歌曲名
{
    int index=PlayList->currentIndex();//获取当前位置
    QString songlabel=fileList.at(index);//提取在当前位置的文件名
    ui->songlabel->setText(songlabel);
}

void MyWidget::on_ModeButton_clicked()//改变播放模式
{
    if(PlayList->playbackMode()==QMediaPlaylist::Loop)
    {
        ui->ModeButton->setIcon(QIcon(":/random.png"));
        ui->ModeButton->setToolTip("随机模式");
        PlayList->setPlaybackMode(QMediaPlaylist::Random);//随机模式
    }
    else if(PlayList->playbackMode()==QMediaPlaylist::Random)
    {
        ui->ModeButton->setIcon(QIcon(":/singlecycle.png"));
        ui->ModeButton->setToolTip("单曲循环模式");
        PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单项模式
    }
    else if(PlayList->playbackMode()==QMediaPlaylist::CurrentItemInLoop)
    {
        //ui->pushButton_9->setIcon(QIcon(":/image/image/loop.png"));
        ui->ModeButton->setIcon(QIcon(":/indexcycle.png"));
        ui->ModeButton->setToolTip("顺序循环模式");
        PlayList->setPlaybackMode(QMediaPlaylist::Loop);//顺序循环模式
    }
}

void MyWidget::on_doubleclicked_musiclist(QTableWidgetItem *item)//双击播放选中
{
    int selectdeindex;
    selectdeindex=item->row();//获取行号  但是行号从0开始计算  列中显示的1其实是0
    qDebug()<<"选中"<<selectdeindex<<"行";
    PlayList->setCurrentIndex(selectdeindex);//在select位置激活媒体
}



void MyWidget::on_VolumeButton_cliked()//点击显示音量条和值，设置icon
{
    if(ui->VolumeSlider->isHidden())
    {
        ui->VolumeSlider->show();
        ui->volumeValue->show();
    }
    else
    {
     ui->VolumeSlider->hide();
     ui->volumeValue->hide();
    }
    if(ui->VolumeSlider->value()==100)
    {
        ui->VolumeButton->setIcon(QIcon(":/maxvolume.png"));
    }
    else if(ui->VolumeSlider->value()==0)
    {
        ui->VolumeButton->setIcon(QIcon(":/minvolume.png"));
    }
    else
    {
        ui->VolumeButton->setIcon(QIcon(":/volume.png"));
    }
}

 void MyWidget::changeVolume()
 {
      Player->setVolume(ui->VolumeSlider->value());//通过滑块值设置音量
     //qDebug()<<"位置"<<ui->VolumeSlider->value();
 }

void MyWidget::on_ScanMusicButton_clicked()
{
    QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    QString filePath =QFileDialog::getOpenFileName(this, tr("Open File"),
                                         musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                                         tr("MP3 files (*.mp3);;All files (*.*)"));
    if (!filePath.isEmpty())//如果选中
     {

        QFileInfo fileInfo=QFileInfo(filePath);//文件信息
        QString filename=fileInfo.fileName();//文件名
        addItem(filename);//文件名加到列表显示
        fileList.push_back(filename);//文件添加到播放列表list
        qDebug()<<"文件路径"<<filePath;
        PlayList->addMedia(QUrl::fromLocalFile(filePath));//文件添加到playlist
         qDebug()<<"添加后歌曲后总数："<<PlayList->mediaCount();


     }
}

void MyWidget::on_volumeValue_linkActivated()//实时音量数字化显示
{

    int volumeval=ui->VolumeSlider->value();
    ui->volumeValue->setNum(volumeval);

}

static QString Time(qint64 time)//转换时间
{
    qint64 seconds=time/1000;
    const qint64 minutes=seconds/60;
    seconds-=minutes*60;
    return QStringLiteral("%1:%2")
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
    /*QString::arg ( int a, int fieldWidth = 0, int base = 10, const QChar & fillChar = QLatin1Char( ' ' ) )
     * fieldWidth表示字符宽度，base表示进制
     * QStringLiteral是Qt为我们提供了 QStringLiteral(str) 宏
     * 使用 QStringLiteral 宏可以在编译期把代码里的常量字符串 str 直接构造为 QString 对象
     */
}

void MyWidget::updatePosition(qint64 position)//播放的位置改变->更新进度条播放位置&&进度时间标签位置
{
    //qDebug()<<"播放位置"<<position;
    ui->SliderDuration->setRange(0,Player->duration());//设置进度条长度范围
    ui->SliderDuration->setValue(position);//设置进度条位置同position
    ui->timelabel->setText(Time(position)+"/"+Time(Player->duration()));//更新时间标签
}

MyWidget::~MyWidget()
{
    delete ui;
}
