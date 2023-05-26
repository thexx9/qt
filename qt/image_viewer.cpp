#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>

class ImageViewer : public QLabel
{
public:
    ImageViewer(QWidget *parent = nullptr)
        : QLabel(parent), scaleFactor(1.0)
    {
        setWindowTitle("Image Viewer");
    }

    void loadImage(const QString &imagePath)
    {
        QPixmap image(imagePath);
        if (image.isNull()) {
            setText("Failed to load image.");
        } else {
            setPixmap(image);
            setScaledContents(true);
            resize(image.size());
            scaleFactor = 1.0;
        }
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        switch (event->key()) {
        case Qt::Key_Plus:
            zoomIn();
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_Left:
            rotateLeft();
            break;
        case Qt::Key_Right:
            rotateRight();
            break;
        case Qt::Key_R:
            flip();
            break;
        default:
            QLabel::keyPressEvent(event);
            break;
        }
    }

private:
    void zoomIn()
    {
        scaleImage(1.25);
    }

    void zoomOut()
    {
        scaleImage(0.8);
    }

    void rotateLeft()
    {
        QPixmap currentPixmap = pixmap();
        if (!currentPixmap.isNull()) {
            setPixmap(currentPixmap.transformed(QTransform().rotate(-90)), Qt::SmoothTransformation);
        }
    }

    void rotateRight()
    {
        QPixmap currentPixmap = pixmap();
        if (!currentPixmap.isNull()) {
            setPixmap(currentPixmap.transformed(QTransform().rotate(90)), Qt::SmoothTransformation);
        }
    }

    void flip()
    {
        QPixmap currentPixmap = pixmap();
        if (!currentPixmap.isNull()) {
            setPixmap(currentPixmap.transformed(QTransform().scale(-1, 1)), Qt::SmoothTransformation);
        }
    }

    void scaleImage(float factor)
    {
        scaleFactor *= factor;
        setScaledContents(true);
        resize(scaleFactor * pixmap().size());
    }

    qreal scaleFactor;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageViewer viewer;
    if (argc > 1) {
        viewer.loadImage(argv[1]);
    } else {
        viewer.setText("No image specified.");
    }

    viewer.show();

    return app.exec();
}

