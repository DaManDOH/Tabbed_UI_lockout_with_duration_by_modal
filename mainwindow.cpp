#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QProgressBar>
#include <QTimer>

QString const MainWindow::sm_logMessagePattern{
    R"([%{time yyyy-MMM-dd h:mm:ss.zzztt}] - %{message})"};

// Standard QWidget extension constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /***************
     * Bookkeeping *
     ***************/
    qSetMessagePattern(sm_logMessagePattern);
    ui->setupUi(this);
    connectUiEvents();
}

// Standard QWidget extension destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectUiEvents()
{
    connect(ui->horizontalSlider,
            &QAbstractSlider::valueChanged,
            this,
            &MainWindow::horizontalSliderValueChanged);
    connect(ui->horizontalSlider,
            &QAbstractSlider::sliderReleased,
            this,
            &MainWindow::horizontalSliderSliderReleased);
    connect(ui->horizontalSlider,
            &QAbstractSlider::actionTriggered,
            this,
            &MainWindow::horizontalSliderActionTriggered);
    connect(ui->topButton, &QAbstractButton::clicked, this, &MainWindow::topButtonClicked);
    connect(ui->middleButton, &QAbstractButton::clicked, this, &MainWindow::middleButtonClicked);
    connect(ui->bottomButton, &QAbstractButton::clicked, this, &MainWindow::bottomButtonClicked);
}

void MainWindow::spawnProgressDialog(QString const &message)
{
    qDebug() << "In spawnProgressDialog()";
    m_blockingOverlay = std::make_unique<QProgressDialog>(this);
    m_blockingOverlay->setWindowModality(Qt::WindowModal);
    m_blockingOverlay->setWindowFlag(Qt::SplashScreen);
    m_blockingOverlay->setWindowFlag(Qt::WindowStaysOnTopHint);
    m_blockingOverlay->setWindowFlag(Qt::WindowCloseButtonHint, false);
    m_blockingOverlay->setCancelButton(nullptr); // Hide cancel button
    m_blockingOverlay->setWindowTitle("Please wait...");
    m_blockingOverlay->setLabelText(message);

    QProgressBar *const progressBar = new QProgressBar(m_blockingOverlay.get());
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setMaximum(0); // Make indeterminate
    m_blockingOverlay->setBar(progressBar);

    m_blockingOverlay->open();
}

void MainWindow::disableUi()
{
    // All children inherit disabled state. Could be done at a per-widget level.
    ui->centralwidget->setDisabled(true);
}

void MainWindow::enableUi()
{
    // All children inherit disabled state. Could be done at a per-widget level.
    ui->centralwidget->setDisabled(false);
}

void MainWindow::blockingUiEvent(QString const &dialogBoxMessage)
{
    qDebug() << "In blockingUiEvent()";
    disableUi();
    spawnProgressDialog(dialogBoxMessage);
    QTimer::singleShot(4000, ui->centralwidget, [this]() {
        // TODO: Other delayed synchronization, state persistence, and/or maintenance activities.

        qDebug() << "Hiding blocking progress dialog";
        m_blockingOverlay->cancel();
        m_blockingOverlay.release();
        enableUi();
    });
}

void MainWindow::horizontalSliderValueChanged(int value)
{
    qDebug() << QString{"Slider value changed to %1"}.arg(value);
    ui->lcdNumber->display(value);
}

void MainWindow::horizontalSliderSliderReleased()
{
    qDebug() << QString{"In horizontalSliderSliderReleased(): selected value: %1"}.arg(
        ui->horizontalSlider->value());
    blockingUiEvent("Slider released");
    // TODO: Other immediate synchronization, state persistence, and/or maintenance activities.
}

void MainWindow::horizontalSliderActionTriggered(int actionEnum)
{
    // Handle paging in the slider
    switch (actionEnum) {
    case QSlider::SliderSingleStepAdd:
    case QSlider::SliderSingleStepSub:
        blockingUiEvent("Slider single-stepped");
        break;
    case QSlider::SliderPageStepAdd:
    case QSlider::SliderPageStepSub:
        blockingUiEvent("Slider paged");
    }
}

void MainWindow::topButtonClicked()
{
    qDebug() << "Top button clicked";
    blockingUiEvent("Top button clicked");
    // TODO: Other immediate synchronization, state persistence, and/or maintenance activities.
}

void MainWindow::middleButtonClicked()
{
    qDebug() << "Middle button clicked";
    blockingUiEvent("Middle button clicked");
    // TODO: Other immediate synchronization, state persistence, and/or maintenance activities.
}

void MainWindow::bottomButtonClicked()
{
    qDebug() << "Bottom button clicked";
    blockingUiEvent("Bottom button clicked");
    // TODO: Other immediate synchronization, state persistence, and/or maintenance activities.
}
