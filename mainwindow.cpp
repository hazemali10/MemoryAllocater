#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <algorithm>
#include <QLabel>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->segmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->memoryLayoutTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->stackedWidget->setCurrentWidget(ui->setupPage);
    ui->holesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::on_confirmSizeButton_clicked()
{
    confirmedMemorySize = ui->memorySizeSpinBox->value();

    ui->memorySizeSpinBox->setEnabled(false);
    ui->confirmSizeButton->setEnabled(false);

    ui->holeStartSpinBox->setEnabled(true);
    ui->holeSizeSpinBox->setEnabled(true);
    ui->addHoleButton->setEnabled(true);
    ui->createMemoryButton->setEnabled(true);
}


void MainWindow::on_addHoleButton_clicked()
{
    int start = ui->holeStartSpinBox->value();
    int size = ui->holeSizeSpinBox->value();

    if (start + size > confirmedMemorySize) {
        QMessageBox::warning(this, "Invalid Hole", "Hole exceeds memory size.");
        return;
    }

    // Check overlap with existing holes
    int newStart = start;
    int newEnd = start + size;

    for (int row = 0; row < ui->holesTable->rowCount(); row++) {
        int existingStart = ui->holesTable->item(row, 0)->text().toInt();
        int existingSize = ui->holesTable->item(row, 1)->text().toInt();
        int existingEnd = existingStart + existingSize;

        bool overlaps = newStart < existingEnd && existingStart < newEnd;

        if (overlaps) {
            QMessageBox::warning(this, "Invalid Hole", "Hole overlaps with an existing hole.");
            return;
        }
    }

    int row = ui->holesTable->rowCount();
    ui->holesTable->insertRow(row);

    ui->holesTable->setItem(row, 0, new QTableWidgetItem(QString::number(start)));
    ui->holesTable->setItem(row, 1, new QTableWidgetItem(QString::number(size)));

    // Reset inputs after successful add
    ui->holeStartSpinBox->setValue(0);
    ui->holeSizeSpinBox->setValue(0);
}


void MainWindow::on_createMemoryButton_clicked()
{
        std::vector<Hole> holes;

        for (int row = 0; row < ui->holesTable->rowCount(); row++) {
            int start = ui->holesTable->item(row, 0)->text().toInt();
            int size = ui->holesTable->item(row, 1)->text().toInt();

            holes.push_back({start, size});
        }

        memoryManager = std::make_unique<MemoryManager>(confirmedMemorySize, holes);
        ui->stackedWidget->setCurrentWidget(ui->processPage);

}


void MainWindow::on_resetSetupButton_clicked()
{
    confirmedMemorySize = 0;
    memoryManager.reset();

    ui->memorySizeSpinBox->setEnabled(true);
    ui->confirmSizeButton->setEnabled(true);

    ui->holeStartSpinBox->setEnabled(false);
    ui->holeSizeSpinBox->setEnabled(false);
    ui->addHoleButton->setEnabled(false);
    ui->createMemoryButton->setEnabled(false);

    ui->memorySizeSpinBox->setValue(0);
    ui->holeStartSpinBox->setValue(0);
    ui->holeSizeSpinBox->setValue(0);

    ui->holesTable->setRowCount(0);
}


void MainWindow::on_addSegmentButton_clicked()
{
    QString segmentName = ui->segmentNameInput->text().trimmed();
    int segmentSize = ui->segmentSizeSpinBox->value();

    if (segmentName.isEmpty()) {
        QMessageBox::warning(this, "Invalid Segment", "Please enter a segment name.");
        return;
    }

    int row = ui->segmentsTable->rowCount();
    ui->segmentsTable->insertRow(row);

    ui->segmentsTable->setItem(row, 0, new QTableWidgetItem(segmentName));
    ui->segmentsTable->setItem(row, 1, new QTableWidgetItem(QString::number(segmentSize)));

    ui->segmentNameInput->clear();
    ui->segmentSizeSpinBox->setValue(1);
}


void MainWindow::on_allocateProcessButton_2_clicked()
{
    if (!memoryManager) {
        QMessageBox::warning(this, "Error", "Memory has not been created yet.");
        return;
    }

    QString processNameQString = ui->processNameInput->text().trimmed();

    if (processNameQString.isEmpty()) {
        QMessageBox::warning(this, "Invalid Process", "Please enter a process name.");
        return;
    }

    if (ui->segmentsTable->rowCount() == 0) {
        QMessageBox::warning(this, "Invalid Process", "Please add at least one segment.");
        return;
    }

    Process process(processNameQString.toStdString());

    for (int row = 0; row < ui->segmentsTable->rowCount(); row++) {
        QString segmentName = ui->segmentsTable->item(row, 0)->text();
        int segmentSize = ui->segmentsTable->item(row, 1)->text().toInt();

        process.segments.emplace_back(segmentName.toStdString(), segmentSize);
    }

    AllocationMethod method;

    if (ui->allocationMethodBox->currentText() == "First Fit") {
        method = AllocationMethod::FirstFit;
    } else {
        method = AllocationMethod::BestFit;
    }

    bool success = memoryManager->allocateProcess(process, method);

    if (success) {
        QMessageBox::information(this, "Success", "Process allocated successfully.");

        ui->processesComboBox->addItem(processNameQString);


    } else {
        QMessageBox::warning(this, "Allocation Failed", "This process does not fit in memory.");
    }
    ui->processNameInput->clear();
    ui->segmentsTable->setRowCount(0);
    ui->segmentNameInput->clear();
    ui->segmentSizeSpinBox->setValue(0);

    refreshOutputPage();
    ui->stackedWidget->setCurrentWidget(ui->outputPage);
}

void MainWindow::refreshOutputPage()
{
    ui->memoryLayoutTable->setRowCount(0);

    if (!memoryManager) {
        return;
    }

    std::vector<Hole> holes = memoryManager->getHoles();
    std::vector<Process> processes = memoryManager->getAllocatedProcesses();
    clearOutputLayout();

    for (const Process& process : processes) {
        QLabel* processLabel = new QLabel(QString::fromStdString(process.name));
        ui->outputLayout->addWidget(processLabel);

        QTableWidget* table = new QTableWidget();
        table->setColumnCount(3);
        table->setHorizontalHeaderLabels({"Segment", "Base Address (K)", "Size (K)"});
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for (const Segment& segment : process.segments) {
            int row = table->rowCount();
            table->insertRow(row);

            table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(segment.name)));
            table->setItem(row, 1, new QTableWidgetItem(QString::number(segment.baseAddress)));
            table->setItem(row, 2, new QTableWidgetItem(QString::number(segment.size)));
        }

        ui->outputLayout->addWidget(table);
    }

    struct MemoryBlock {
        int base;
        int size;
        QString name;
    };

    std::vector<MemoryBlock> blocks;

    for (int i = 0; i < holes.size(); i++) {
        QString holeName = "Hole " + QString::number(i + 1);
        blocks.push_back({holes[i].start, holes[i].size, holeName});
    }

    for (const Process& process : processes) {
        for (const Segment& segment : process.segments) {
            QString blockName =
                QString::fromStdString(process.name) + " - " +
                QString::fromStdString(segment.name);

            blocks.push_back({segment.baseAddress, segment.size, blockName});
        }
    }

    std::sort(blocks.begin(), blocks.end(), [](const MemoryBlock& a, const MemoryBlock& b) {
        return a.base < b.base;
    });

    std::vector<MemoryBlock> completeBlocks;
    int currentAddress = 0;

    for (const MemoryBlock& block : blocks) {
        if (currentAddress < block.base) {
            completeBlocks.push_back({
                currentAddress,
                block.base - currentAddress,
                "Reserved"
            });
        }

        completeBlocks.push_back(block);
        currentAddress = block.base + block.size;
    }

    if (currentAddress < confirmedMemorySize) {
        completeBlocks.push_back({
            currentAddress,
            confirmedMemorySize - currentAddress,
            "Reserved"
        });
    }

    blocks = completeBlocks;

    for (const MemoryBlock& block : blocks) {
        int row = ui->memoryLayoutTable->rowCount();
        ui->memoryLayoutTable->insertRow(row);

        ui->memoryLayoutTable->setItem(row, 0, new QTableWidgetItem(block.name));
        ui->memoryLayoutTable->setItem(row, 1, new QTableWidgetItem(QString::number(block.base)));
        ui->memoryLayoutTable->setItem(row, 2, new QTableWidgetItem(QString::number(block.size)));
    }
}


void MainWindow::on_returnToSetupButton_clicked()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(
        this,
        "Reset Memory",
        "Are you sure? This will delete the current memory and all allocated processes.",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        memoryManager.reset();
        confirmedMemorySize = 0;

        ui->holesTable->setRowCount(0);
        ui->segmentsTable->setRowCount(0);
        ui->memoryLayoutTable->setRowCount(0);
        ui->processesComboBox->clear();

        ui->processNameInput->clear();
        ui->segmentNameInput->clear();

        ui->memorySizeSpinBox->setEnabled(true);
        ui->confirmSizeButton->setEnabled(true);

        ui->holeStartSpinBox->setEnabled(false);
        ui->holeSizeSpinBox->setEnabled(false);
        ui->addHoleButton->setEnabled(false);
        ui->createMemoryButton->setEnabled(false);

        ui->memorySizeSpinBox->setValue(1);
        ui->holeStartSpinBox->setValue(0);
        ui->holeSizeSpinBox->setValue(1);
        ui->segmentSizeSpinBox->setValue(1);

        ui->stackedWidget->setCurrentWidget(ui->setupPage);
    }
}


void MainWindow::on_backToProcessPageButton_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->processPage);
}





void MainWindow::on_deallocateProcessButton_clicked()
{
    if (!memoryManager) {
        QMessageBox::warning(this, "Error", "Memory has not been created yet.");
        return;
    }

    QString processNameQString = ui->processesComboBox->currentText();

    if (processNameQString.isEmpty()) {
        QMessageBox::warning(this, "No Process", "No process selected for deallocation.");
        return;
    }

    bool success = memoryManager->deallocateProcess(processNameQString.toStdString());

    if (success) {
        QMessageBox::information(this, "Success", "Process deallocated successfully.");

        int index = ui->processesComboBox->currentIndex();
        ui->processesComboBox->removeItem(index);
    } else {
        QMessageBox::warning(this, "Error", "Process was not found.");
    }

    refreshOutputPage();
    ui->stackedWidget->setCurrentWidget(ui->outputPage);
}

void MainWindow::clearOutputLayout()
{
    QLayoutItem* item;

    while ((item = ui->outputLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}
