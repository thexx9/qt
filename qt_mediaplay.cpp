#include <QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QSlider>
#include <QKeyEvent>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建主窗口
    QWidget *window = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(window);

    // 创建视频显示部件
    QVideoWidget *videoWidget = new QVideoWidget;
    layout->addWidget(videoWidget);

    // 创建媒体播放器
    QMediaPlayer *player = new QMediaPlayer;
    player->setVideoOutput(videoWidget);

    // 创建按钮和滑块等控件
    QPushButton *openButton = new QPushButton("Open File");
    layout->addWidget(openButton);

    QPushButton *playButton = new QPushButton("Play");
    layout->addWidget(playButton);

    QPushButton *pauseButton = new QPushButton("Pause");
    layout->addWidget(pauseButton);

    QPushButton *stopButton = new QPushButton("Stop");
    layout->addWidget(stopButton);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal);
    layout->addWidget(volumeSlider);

    QSlider *progressSlider = new QSlider(Qt::Horizontal);
    layout->addWidget(progressSlider);

    QLabel *titleLabel = new QLabel;
    QLabel *artistLabel = new QLabel;
    QLabel *durationLabel = new QLabel;
    layout->addWidget(titleLabel);
    layout->addWidget(artistLabel);
    layout->addWidget(durationLabel);

    // 连接按钮和滑块等控件的信号与槽
    QObject::connect(openButton, &QPushButton::clicked, [=]() {
        // 打开文件对话框选择视频文件
        QString filename = QFileDialog::getOpenFileName(window, "Open Video");
        if (!filename.isEmpty()) {
            // 设置媒体播放器的视频来源并开始播放
            player->setMedia(QUrl::fromLocalFile(filename));
            player->play();
        }
    });

    QObject::connect(playButton, &QPushButton::clicked, player, &QMediaPlayer::play);

    QObject::connect(pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);

    QObject::connect(stopButton, &QPushButton::clicked, player, &QMediaPlayer::stop);

    QObject::connect(volumeSlider, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);

    QObject::connect(progressSlider, &QSlider::valueChanged, [=](int value) {
        // 根据滑块的值设置媒体播放器的播放位置
        player->setPosition(value * 1000);
    });

    QObject::connect(player, &QMediaPlayer::positionChanged, [=](qint64 position) {
        // 媒体播放器播放位置发生变化时更新滑块的值
        if (!progressSlider->isSliderDown()) {
            progressSlider->setValue(position / 1000);
        }
    });

    // 显示主窗口
    window->show();

    return app.exec();
}

