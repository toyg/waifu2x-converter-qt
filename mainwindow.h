#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "waifu2xconverterqtsettings.h"
#include "processmodemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ProcessModeModel *processModeModel();

private slots:
    void browseImage();
    void processImage(const QString& imageFileName);
    void toggleOptions(const int boxIndex);

private:
    void init();
    QString browseSaveLocation(const QString& inputImageFileName);
    void closeEvent(QCloseEvent*);
    void save();

    Ui::MainWindow *ui;
    Waifu2xConverterQtSettings* m_settings;
    ProcessModeModel* m_procmode;
};

#endif // MAINWINDOW_H
