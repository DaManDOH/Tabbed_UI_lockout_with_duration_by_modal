#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void horizontalSliderValueChanged(int);
    void horizontalSliderSliderReleased();
    void horizontalSliderActionTriggered(int);
    void topButtonClicked();
    void middleButtonClicked();
    void bottomButtonClicked();

private:
    static QString const sm_logMessagePattern;

    Ui::MainWindow *ui;
    std::unique_ptr<QProgressDialog> m_blockingOverlay;

    void connectUiEvents();

    /*
     * Grey out interactive widgets/UI elements. Gives user visual feedback that the UX is unavailable for the duration.
     */
    void disableUi();

    /*
     * Undo greying-out of interactive widgets/UI elements.
     */
    void enableUi();

    void blockingUiEvent(QString const &);
    void spawnProgressDialog(QString const &);
};
#endif // MAINWINDOW_H
