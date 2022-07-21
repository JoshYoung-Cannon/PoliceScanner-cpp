#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "globalvars.h"
#include "services.h"

#include <iostream>

const QString nullOutcomeValueResponse = "Outcome-null-value";
const QString emptyMapResponse = "No Crimes Were Recorded At This Location During This Time";
const QString crimeCategoryResponseFormat = "%1: %2";
const QString crimeOutcomeResponseFormat = "- %1";
const QString mapItemResponseFormat = "%1";
const QString instructionText = "Please choose a \n"
                                "Latitiude in the range: %1 to %2\n"
                                "Longitude in the range: %3 to %4\n"
                                "Year and Month in the range: %5-%6 to %7-%8";

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

void MainWindow::on_guideButton_clicked()
{
    std::cout << "GUIDE CLICKED" << std::endl;
    QString instructions = QString(instructionText)
            .arg(globalVars.minLat)
            .arg(globalVars.maxLat)
            .arg(globalVars.minLng)
            .arg(globalVars.maxLng)
            .arg(globalVars.earliestYear)
            .arg(globalVars.earliestMonth)
            .arg(globalVars.maxYear)
            .arg(globalVars.maxMonth);
    std::cout << instructions.toStdString() << std::endl;
    ui->outputText->setText(instructions);
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
    std::cout << "Display Result" << std::endl;
    this->ui->outputTree->clear();
    QString outputText = emptyMapResponse;
    QString numberOfCrimes;
    if (crimes.m_crimesMap.empty())
    {
        std::cout << emptyMapResponse.toStdString() << std::endl;
        QTreeWidgetItem *emptyResponseItem = new QTreeWidgetItem();
        emptyResponseItem->setText(0, emptyMapResponse);
        this->ui->outputText->setPlainText(outputText);
        this->ui->outputTree->addTopLevelItem(emptyResponseItem);
        return;
    }
    else
    {
        outputText = "";
        for (const auto& [category, specificCrimes] : crimes.m_crimesMap)
        {
            QTreeWidgetItem *categoryItem = new QTreeWidgetItem();
            categoryItem->setText(0, QString(crimeCategoryResponseFormat).arg(category).arg(specificCrimes.size()));
            std::cout << category.toStdString() << ": " << specificCrimes.size() << std::endl;
            outputText = outputText + QString(crimeCategoryResponseFormat).arg(category).arg(specificCrimes.size()) + "\n";
            for (const auto& currentCrime : specificCrimes)
            {
                QTreeWidgetItem *crimeItem = new QTreeWidgetItem();
                if (currentCrime.m_outcome.isEmpty())
                {
                    crimeItem->setText(0, QString(crimeOutcomeResponseFormat).arg(nullOutcomeValueResponse));
                    std::cout << nullOutcomeValueResponse.toStdString() << std::endl;
                    outputText = outputText + QString(crimeOutcomeResponseFormat).arg(nullOutcomeValueResponse) + "\n";
                }
                else
                {
                    crimeItem->setText(0, QString(crimeOutcomeResponseFormat).arg(currentCrime.m_outcome));
                    std::cout << "- " << currentCrime.m_outcome.toStdString() << std::endl;
                    outputText = outputText + QString(crimeOutcomeResponseFormat).arg(currentCrime.m_outcome) + "\n";
                }
                categoryItem->addChild(crimeItem);
            }
            this->ui->outputTree->addTopLevelItem(categoryItem);
            outputText = outputText + "\n";
        }
        this->ui->outputText->setPlainText(outputText);
    }

}
