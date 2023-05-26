#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>

// 自定义类 ImageViewer，继承自 QLabel
class ImageViewer : public QLabel
{
public:
    ImageViewer(QWidget *parent = nullptr)
        : QLabel(parent), scaleFactor(1.0), currentIndex(-1)
    {
        setWindowTitle("图片查看器");
    }

    // 设置文件夹路径，并加载第一张图片
    void setDirectory(const QString &dirPath)
    {
        directory.setPath(dirPath);
        imageFiles = directory.entryInfoList(QStringList() << "*.jpg" << "*.png", QDir::Files);
        if (!imageFiles.isEmpty()) {
            currentIndex = 0;
            const QString imagePath = imageFiles.first().absoluteFilePath();
            loadImage(imagePath);
        }
    }

    // 加载图片的函数，传入图片的文件路径
    void loadImage(const QString &imagePath)
    {
        // 从图片文件创建 QPixmap 对象
        QPixmap image(imagePath);
        if (image.isNull()) {
            setText("加载图片失败。"); // 如果图片加载失败，显示错误消息
        } else {
            // 设置 QLabel 的 pixmap 为加载的图片
            setPixmap(image);
            setScaledContents(true); // 启用缩放以适应 QLabel 的大小
            resize(image.size()); // 调整 QLabel 的大小以匹配图片的尺寸
            scaleFactor = 1.0; // 将缩放因子重置为 1.0

            // 获取当前图片所在的文件夹路径
            QFileInfo fileInfo(imagePath);
            directory.setPath(fileInfo.absolutePath());
            imageFiles = directory.entryInfoList(QStringList() << "*.jpg" << "*.png", QDir::Files);
            currentIndex = imageFiles.indexOf(fileInfo);

            if (currentIndex == -1) {
                setText("无法获取图片列表。"); // 如果无法获取图片列表，显示错误消息
            }
        }
    }

protected:
    // 重写 keyPressEvent 函数以处理按键事件
    void keyPressEvent(QKeyEvent *event) override
    {
        switch (event->key()) {
        case Qt::Key_Plus:
            zoomIn(); // 当按下加号键时放大图片
            break;
        case Qt::Key_Minus:
            zoomOut(); // 当按下减号键时缩小图片
            break;
        case Qt::Key_Left:
            previousImage(); // 当按下左箭头键时切换到上一张图片
            break;
        case Qt::Key_Right:
            nextImage(); // 当按下右箭头键时切换到下一张图片
            break;
        case Qt::Key_R:
            flip(); // 当按下 R 键时水平翻转图片
            break;
        default:
            QLabel::keyPressEvent(event); // 将事件传递给基类以进行默认处理
            break;
        }
    }

private:
    // 放大图片的函数
    void zoomIn()
    {
        scaleImage(1.25); // 将图片按 1.25 倍缩放（增大尺寸）
    }

    // 缩小图片的函数
    void zoomOut()
    {
        scaleImage(0.8); // 将图片按 0.8 倍缩放（减小尺寸）
    }

    // 切换到上一张图片
    void previousImage()
    {
        if (currentIndex > 0) {
            currentIndex--;
        } else {
            currentIndex = imageFiles.size() - 1;
        }
        const QString imagePath = imageFiles.at(currentIndex).absoluteFilePath();
        loadImage(imagePath);
    }

    // 切换到下一张图片
    void nextImage()
    {
        currentIndex = (currentIndex + 1) % imageFiles.size();
        const QString imagePath = imageFiles.at(currentIndex).absoluteFilePath();
        loadImage(imagePath);
    }

    // 水平翻转图片的函数
	// 水平翻转图片的函数
	// 水平翻转图片的函数
	void flip()
	{
		const QPixmap *currentPixmap = pixmap(); // 获取当前图片的 pixmap
		if (currentPixmap && !currentPixmap->isNull()) {
			QPixmap flippedPixmap = currentPixmap->copy();
			flippedPixmap = flippedPixmap.transformed(QTransform().scale(-1, 1)); // 水平翻转 pixmap
			setPixmap(flippedPixmap); // 将 QLabel 的 pixmap 设置为翻转后的图片
		}
	}


    // 按给定因子缩放图片的函数
    void scaleImage(float factor)
    {
        scaleFactor *= factor; // 更新缩放因子
        setScaledContents(true); // 启用缩放以适应 QLabel 的大小
        resize(scaleFactor * sizeHint()); // 根据缩放后的图片大小调整 QLabel 的大小
    }

    qreal scaleFactor; // 缩放因子，用于放大和缩小图片
    QDir directory; // 图片所在的文件夹路径
    QFileInfoList imageFiles; // 文件夹中的图片列表
    int currentIndex; // 当前显示的图片索引
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageViewer viewer;
    if (argc > 1) {
        viewer.setDirectory(QFileInfo(argv[1]).absolutePath()); // 如果命令行参数中提供了图片文件夹路径，则设置文件夹路径并加载第一张图片
    } else {
        viewer.setText("未指定图片文件夹路径。"); // 如果没有提供图片文件夹路径，则显示一条消息
    }

    viewer.show(); // 显示 ImageViewer 窗口
    return app.exec(); // 运行应用程序事件循环
}

