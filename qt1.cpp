#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>

class ImageViewer : public QWidget
{
public:
    ImageViewer(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 创建标签和布局
        m_imageLabel = new QLabel;
        m_imageLabel->setAlignment(Qt::AlignCenter);

        m_layout = new QVBoxLayout;
        m_layout->addWidget(m_imageLabel);
        setLayout(m_layout);

        // 设置窗口标题
        setWindowTitle(tr("Image Viewer"));
    }

    void open()
    {
        // 打开文件对话框
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

        if (!fileName.isEmpty()) {
            // 加载图片
            QPixmap pixmap(fileName);

            // 将图片设置为标签的内容
            m_imageLabel->setPixmap(pixmap);

            // 调整标签的大小以适应图片
            m_imageLabel->adjustSize();
        }
    }

private:
    QLabel *m_imageLabel;
    QVBoxLayout *m_layout;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建图片查看器
    ImageViewer viewer;

    // 打开第一张图片
    viewer.open();

    // 显示图片查看器
    viewer.show();

    // 运行应用程序
    return app.exec();
}

