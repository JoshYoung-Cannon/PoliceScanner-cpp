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

    std::map<crimesJsonFieldsEnum, QString> getCrimesJsonFields() {return m_crimesJsonFields;};
    void addCrimesJsonFields(const crimesJsonFieldsEnum& key, QString value) {m_crimesJsonFields.insert({key,value});};

private:
    std::map<crimesJsonFieldsEnum, QString> m_crimesJsonFields;
};

#endif // JSONMAPPING_H
