#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QFileDialog>
#include <QImage>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void chooseColor();
    void chooseBackgroundImage();
    void updateThreshold(int value);
    void saveImage();

private:
    void updateImageDisplay();

    Ui::MainWindow *ui;
    cv::Mat originalImage;
    cv::Mat backgroundImage;
    cv::Mat resultImage;
    int thresholdValue = 128; // Default threshold
};

#endif // MAINWINDOW_H
