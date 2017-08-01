#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "droplabel.h"
#include "processdialog.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(new Waifu2xConverterQtSettings)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::browseImage()
{
    QFileDialog dialog(this,
                       tr("Select image"),
                       QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters({"image/jpeg",
                               "image/png",
                               "application/octet-stream"});
    if (dialog.exec() == QFileDialog::Accepted)
        processImage(dialog.selectedFiles().first());
}

void MainWindow::processImage(const QString &imageFileName)
{
    QString outputFileName;

    if (m_settings->isUseCustomFileName()) {
        outputFileName = browseSaveLocation(imageFileName);
        if (outputFileName.isEmpty()) return;
    }

    ProcessDialog dialog(imageFileName,
                         ui->threadsBox->value(),
                         ui->scaleRatioBox->value(),
                         ui->noiseReductionLevel->value(),
                         ui->imageProcessingModeBox->currentData().toString(),
                         outputFileName,
                         m_settings->modelDirectory());
    dialog.exec();
}

QString MainWindow::browseSaveLocation(const QString &inputImageFileName)
{
    QDir dir(inputImageFileName);
    QString fileExtension;
    QFileDialog dialog(this, tr("Save"));

    dir.cdUp();

    if (inputImageFileName.contains("."))
        fileExtension = inputImageFileName.split(".").last();

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDirectory(dir.absolutePath());
    dialog.selectFile(inputImageFileName);
    dialog.exec();
    return dialog.selectedFiles().isEmpty()
            ? QString() : dialog.selectedFiles().first();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    save();
}

void MainWindow::save()
{
    m_settings->setThreadsCount(ui->threadsBox->value());
    m_settings->setScaleRatio(ui->scaleRatioBox->value());
    m_settings->setNoiseReductionLevel(ui->noiseReductionLevel->value());
    m_settings->setImageProcessingMode(ui->imageProcessingModeBox->currentData().toString());
}

void MainWindow::toggleOptions(int boxIndex){
    QString currentData = ui->imageProcessingModeBox->itemData(boxIndex).toString();
    ui->noiseReductionLevel->setEnabled(currentData.contains("noise"));
    ui->scaleRatioBox->setEnabled(currentData.contains("scale"));
}


void MainWindow::init()
{
    qApp->setApplicationDisplayName(tr("Waifu2x Converter Qt"));
    setWindowTitle(QApplication::applicationDisplayName());

    auto* dropLabel = new DropLabel(this);

    // at some point this should probably be a proper model
    ui->imageProcessingModeBox->insertItem(0, tr("Noise Reduction"), "noise");
    ui->imageProcessingModeBox->insertItem(1, tr("Upscale"),"scale");
    ui->imageProcessingModeBox->insertItem(2, tr("Noise Reduction & Upscale"), "noise_scale");

    connect(ui->action_Preferences, &QAction::triggered, [&]() {
        PreferencesDialog dialog;
        dialog.exec();
    });
    connect(ui->imageProcessingModeBox, SIGNAL(currentIndexChanged(int)),SLOT(toggleOptions(int)));
    connect(dropLabel, SIGNAL(fileDropped(QString)), this, SLOT(processImage(QString)));
    connect(ui->browseButton, SIGNAL(clicked(bool)), this, SLOT(browseImage()));
    connect(ui->action_Open_image, &QAction::triggered, this, &MainWindow::browseImage);
    connect(ui->action_About_waifu2x_converter_qt, &QAction::triggered, [&]() {
        AboutDialog dialog;
        dialog.exec();
    });
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionE_xit, &QAction::triggered, qApp, &QApplication::quit);

    ui->browseButton->setIcon(style()->standardIcon((QStyle::SP_DirOpenIcon)));
    ui->action_Open_image->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->actionE_xit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->action_About_waifu2x_converter_qt->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));

    ui->dropLayout->insertWidget(0, dropLabel, 10);

    ui->threadsBox->setValue(m_settings->threadsCount());
    ui->scaleRatioBox->setValue(m_settings->scaleRatio());
    ui->noiseReductionLevel->setValue(m_settings->noiseReductionLevel());
    int currDataIndex = ui->imageProcessingModeBox->findData(m_settings->imageProcessingMode());
    ui->imageProcessingModeBox->setCurrentIndex(currDataIndex);
}
