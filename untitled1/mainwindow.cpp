#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->chooseColorButton, &QPushButton::clicked, this, &MainWindow::chooseColor);
    connect(ui->chooseBackgroundImage, &QPushButton::clicked, this, &MainWindow::chooseBackgroundImage);
    connect(ui->thresholdSlider, &QSlider::valueChanged, this, &MainWindow::updateThreshold);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);

    // Load the original image (cats.jpg)
    originalImage = cv::imread("D:/pisica.png");
    if (originalImage.empty()) {
        qWarning("Could not open or find the image.");
    }

    // Display the original image
    updateImageDisplay();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::chooseColor() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Background Color");
    if (color.isValid()) {
        // Create a new background image filled with the chosen color
        backgroundImage = cv::Mat(originalImage.size(), CV_8UC3, cv::Scalar(color.red(), color.green(), color.blue()));
        updateImageDisplay();
    }
}

void MainWindow::chooseBackgroundImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose Background Image", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        backgroundImage = cv::imread(fileName.toStdString());

        // Resize background image to match the original image size
        cv::resize(backgroundImage, backgroundImage, originalImage.size());
        updateImageDisplay();
    }
}

void MainWindow::updateThreshold(int value) {
    thresholdValue = value;
    updateImageDisplay();
}

void MainWindow::updateImageDisplay() {
    if (!originalImage.empty() && !backgroundImage.empty()) {
        // Convert the original image to grayscale
        cv::Mat grayImage;
        cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);

        // Apply thresholding
        cv::Mat mask;
        cv::threshold(grayImage, mask, thresholdValue, 255, cv::THRESH_BINARY);

        // Create the result image by blending the original and background images
        resultImage = cv::Mat::zeros(originalImage.size(), originalImage.type());
        for (int y = 0; y < originalImage.rows; ++y) {
            for (int x = 0; x < originalImage.cols; ++x) {
                if (mask.at<uchar>(y, x) == 255) {
                    resultImage.at<cv::Vec3b>(y, x) = originalImage.at<cv::Vec3b>(y, x);
                } else {
                    resultImage.at<cv::Vec3b>(y, x) = backgroundImage.at<cv::Vec3b>(y, x);
                }
            }
        }

        // Convert the result image to QImage for display
        cv::cvtColor(resultImage, resultImage, cv::COLOR_BGR2RGB);
        QImage imgDisplay(resultImage.data, resultImage.cols, resultImage.rows, resultImage.step[0], QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(imgDisplay));
    }
}

void MainWindow::saveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        cv::Mat imageToSave = resultImage.clone();  // Create a copy of the result image
        cv::cvtColor(imageToSave, imageToSave, cv::COLOR_RGB2BGR); // Convert RGB back to BGR before saving
        cv::imwrite(fileName.toStdString(), imageToSave);
    }
}
