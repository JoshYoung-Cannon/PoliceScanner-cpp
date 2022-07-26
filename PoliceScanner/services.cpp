#include "services.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include "crimesdto.h"

const QString policeAPIRoot = "https://data.police.uk/api/";
const QString lastUpdateAPICall = "crime-last-updated";
const QString crimeAtLocationAPICall = "crimes-street/all-crime?lat=%1&lng=%2&date=%3-%4";

Services::Services(QObject *parent)
 : QObject(parent)
 , dateRequest(policeAPIRoot + lastUpdateAPICall)
 , crimeRequest(policeAPIRoot + crimeAtLocationAPICall)
 , m_netMgr(new QNetworkAccessManager(this))
 , m_currentRequest(CurrentRequest::NONE)
 , m_jsonReference()
{
    QObject::connect(m_netMgr, &QNetworkAccessManager::finished, this, &Services::onNetworkReply);
}

void Services::getLatestUpdate()
{
    QNetworkRequest request;
    request.setUrl(dateRequest);

    m_currentRequest = CurrentRequest::LATEST_UPDATE;
    QNetworkReply* reply = m_netMgr->get(request);
}

void Services::getCrimesList(double lat, double lon, int year, int month)
{
    QString req = QString(crimeRequest)
            .arg(lat)
            .arg(lon)
            .arg(year)
            .arg(month);
    std::cout << req.toStdString() << std::endl;
    QNetworkRequest request;
    request.setUrl(req);
    m_currentRequest = CurrentRequest::CRIMES_LIST;
    QNetworkReply* reply = m_netMgr->get(request);

}

void Services::onNetworkReply(QNetworkReply* reply)
{
    if (!reply)
    {
        std::cout << "NO REPLY TO PROCESS" << std::endl;
        return;
    }

    std::cout << "Network Replied" << std::endl;
    switch(m_currentRequest)
    {
    case CurrentRequest::LATEST_UPDATE:
        std::cout << "Latest Update" << std::endl;
        processLatestUpdateResponse(*reply);
        return;
    case CurrentRequest::CRIMES_LIST:
        std::cout << "Crimes List" << std::endl;
        processCrimesListResponse(*reply);
        return;
    default:
        std::cout << "Something went wrong" << std::endl;
        return;
    }
}

void Services::processLatestUpdateResponse(QNetworkReply &reply)
{
    QJsonDocument doc(QJsonDocument::fromJson(reply.readAll()));
    QVariant date = doc.object().value("date").toVariant();

    if (date.isNull()){
        std::cout << "No date found" << std::endl;
        return;
    }

    std::cout << date.toString().toStdString() << std::endl;
    QStringList dateComponents = date.toString().split("-");
    DateDto dateDto(dateComponents[0].toInt(), dateComponents[1].toInt());
    std::cout << "Year: " << dateDto.getYear() << " Month: " << dateDto.getMonth() << std::endl;
    emit latestUpdateAvaliable(dateDto);
}

void Services::processCrimesListResponse(QNetworkReply &reply)
{
    QJsonDocument doc(QJsonDocument::fromJson(reply.readAll()));

    CrimesDto crimeList;
    QJsonArray array = doc.array();
    foreach(const QJsonValue &v, array){
        QJsonObject o = v.toObject();
        std::cout << o.value(m_jsonReference.getCrimesJsonFields(jsonmapping::crimesJsonFieldsEnum::CATEGORY)).toString().toStdString() << std::endl;
        if (o.value(m_jsonReference.getCrimesJsonFields(jsonmapping::crimesJsonFieldsEnum::OUTCOME_STATUS)).isNull())
        {
            crimeList.addCrime(Crime(o.value(m_jsonReference.getCrimesJsonFields(jsonmapping::crimesJsonFieldsEnum::CATEGORY)).toString()));
        }
        else
        {
            QJsonObject outcome = o.value(m_jsonReference.getCrimesJsonFields(jsonmapping::crimesJsonFieldsEnum::OUTCOME_STATUS)).toObject();
            crimeList.addCrime(Crime(o.value(m_jsonReference.getCrimesJsonFields(jsonmapping::crimesJsonFieldsEnum::CATEGORY)).toString(), outcome.value(m_jsonReference.getOutcomeStatusJsonFields(jsonmapping::outcomeStatusJsonFieldsEnum::OUTCOME_STATUS_CATEGORY)).toString()));
        }

    }
    emit crimeListSummary(crimeList);

}
