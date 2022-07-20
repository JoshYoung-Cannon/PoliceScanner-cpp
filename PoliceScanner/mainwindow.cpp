#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "globalvars.h"
#include "services.h"

#include <iostream>

const QString nullOutcomeValueResponse = "- Outcome-null-value";
const QString emptyMapResponse = "- Outcome-null-value";
const QString crimeCategoryResponseFormat = "%s: %d\n";
const QString crimeOutcomeResponseFormat = "- %s\n";
const QString mapItemResponseFormat = "%s\n";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_services = new Services(this);
    QObject::connect(m_services, &Services::latestUpdateAvaliable, this, &MainWindow::onLatestUpdateAvaliable);
    QObject::connect(m_services, &Services::crimeListSummary, this, &MainWindow::onCrimeListSummary);
    globalVars = GlobalVars();
    m_services->getLatestUpdate();
    ui->inputLatidude->setValue(globalVars.initialLat);
    ui->inputLongitude->setValue(globalVars.initialLong);
    ui->inputMonth->setMinimum(globalVars.earliestMonth);
    ui->inputMonth->setMaximum(globalVars.endMonth);
    ui->inputMonth->setValue(globalVars.earliestMonth);
    ui->inputYear->setMaximum(globalVars.maxYear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{
    std::cout << "SEARCH CLICKED" << std::endl;
    double lat = this->ui->inputLatidude->value();
    double lng = this->ui->inputLongitude->value();
    int year = this->ui->inputYear->value();
    int month = this->ui->inputMonth->value();
    m_services->getCrimesList(lat,lng,year,month);
}

void MainWindow::on_inputYear_valueChanged(int year)
{
    if (year == globalVars.maxYear)
    {
        this->ui->inputMonth->setMinimum(globalVars.startMonth);
        this->ui->inputMonth->setMaximum(globalVars.maxMonth);
        this->ui->inputMonth->setValue(globalVars.startMonth);
    }
    else if (year == globalVars.earliestYear)
    {
        this->ui->inputMonth->setMinimum(globalVars.earliestMonth);
        this->ui->inputMonth->setMaximum(globalVars.endMonth);
        this->ui->inputMonth->setValue(globalVars.earliestMonth);
    }
    else
    {
        this->ui->inputMonth->setMinimum(globalVars.startMonth);
        this->ui->inputMonth->setMaximum(globalVars.endMonth);
        this->ui->inputMonth->setValue(globalVars.startMonth);
    }
}

void MainWindow::onLatestUpdateAvaliable(DateDto date)
{
    std::cout << date.getYear() << " " << date.getMonth() << std::endl;
    globalVars.maxYear = date.getYear();
    globalVars.maxMonth = date.getMonth();
    this->ui->inputYear->setMaximum(globalVars.maxYear);
}

void MainWindow::onCrimeListSummary(CrimesDto crimes)
{
    // TODO display result
    std::cout << "Display Result" << std::endl;
    QString outputText = emptyMapResponse;
    QString numberOfCrimes;
    if (crimes.m_crimesMap.empty())
    {
        std::cout << emptyMapResponse.toStdString() << std::endl;
        this->ui->outputText->setPlainText(outputText);
        return;
    }
    else
    {
        outputText = "";
        for (const auto& [category, specificCrimes] : crimes.m_crimesMap)
        {
            std::cout << category.toStdString() << ": " << specificCrimes.size() << std::endl;
            outputText = outputText + QString(crimeCategoryResponseFormat).arg(category).arg(specificCrimes.size());
            for (const auto& currentCrime : specificCrimes)
            {
                if (currentCrime.m_outcome.isEmpty())
                {
                    std::cout << nullOutcomeValueResponse.toStdString() << std::endl;
                    outputText = outputText + QString(crimeOutcomeResponseFormat).arg(nullOutcomeValueResponse);
                }
                else
                {
                    std::cout << "- " << currentCrime.m_outcome.toStdString() << std::endl;
                    outputText = outputText + QString(crimeOutcomeResponseFormat).arg(currentCrime.m_outcome);
                }
            }
            outputText = outputText + "\n";
        }
        this->ui->outputText->setPlainText(outputText);
    }
}
