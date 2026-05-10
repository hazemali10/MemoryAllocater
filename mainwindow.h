#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "MemoryManager.h"
#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_confirmSizeButton_clicked();

    void on_addHoleButton_clicked();

    void on_createMemoryButton_clicked();

    void on_resetSetupButton_clicked();

    void on_addSegmentButton_clicked();

    void on_allocateProcessButton_2_clicked();
     void refreshOutputPage();

    void on_returnToSetupButton_clicked();

     void on_backToProcessPageButton_clicked();

    void on_deallocateProcessButton_clicked();
    void clearOutputLayout();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MemoryManager> memoryManager;
    int confirmedMemorySize = 0;

};
#endif // MAINWINDOW_H
