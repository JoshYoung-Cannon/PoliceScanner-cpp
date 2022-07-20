#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>
#include <QString>
#include "datedto.h"
#include <QNetworkAccessManager>
#include "crimesdto.h"

class Services : public QObject
{
    Q_OBJECT
public:
    Services(QObject *parent = nullptr);
    void getLatestUpdate();
    void getCrimesList(double lat, double lon, int year, int month);

private:
    enum CurrentRequest
    {
        NONE,
        LATEST_UPDATE,
        CRIMES_LIST
    };

    QString dateRequest;
    QString crimeRequest;
    QNetworkAccessManager *m_netMgr;
    CurrentRequest m_currentRequest;

    void processLatestUpdateResponse(QNetworkReply &reply);
    void processCrimesListResponse(QNetworkReply &reply);

signals:
    void latestUpdateAvaliable(DateDto date);
    void crimeListSummary(CrimesDto crimes);

private slots:
    void onNetworkReply(QNetworkReply* reply);
};

#endif // SERVICES_H
