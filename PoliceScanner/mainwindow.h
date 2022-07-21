#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "globalvars.h"
#include "datedto.h"
#include "services.h"
#include "crimesdto.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_guideButton_clicked();
    void on_searchButton_clicked();
    void on_inputYear_valueChanged(int year);
    void onLatestUpdateAvaliable(DateDto date);
    void onCrimeListSummary(CrimesDto crimes);

private:
    Ui::MainWindow *ui;
    GlobalVars globalVars;
    Services *m_services;

};

#endif // MAINWINDOW_H
