#ifndef JSONMAPPING_H
#define JSONMAPPING_H

#include <QString>
#include <map>



class jsonmapping
{
public:
    jsonmapping();

    enum crimesJsonFieldsEnum
    {
        CATEGORY,
        OUTCOME_STATUS
    };

    enum outcomeStatusJsonFieldsEnum
    {
        OUTCOME_STATUS_CATEGORY
    };

    QString getCrimesJsonFields(const crimesJsonFieldsEnum& key) {return m_crimesJsonFields[key];};
    QString getOutcomeStatusJsonFields(const outcomeStatusJsonFieldsEnum& key) {return m_outcomeStatusJsonFields[key];};

private:
    std::map<crimesJsonFieldsEnum, QString> m_crimesJsonFields;
    void addCrimesJsonFields(const crimesJsonFieldsEnum& key, QString value) {m_crimesJsonFields.insert({key,value});};
    std::map<outcomeStatusJsonFieldsEnum, QString> m_outcomeStatusJsonFields;
    void addOutcomeStatusJsonFields(const outcomeStatusJsonFieldsEnum& key, QString value) {m_outcomeStatusJsonFields.insert({key,value});};
};

#endif // JSONMAPPING_H
