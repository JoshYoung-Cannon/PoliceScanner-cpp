#ifndef CRIMESDTO_H
#define CRIMESDTO_H
#include <QString>

#include <map>
#include <vector>

class Crime
{
public:
    Crime(QString category, QString outcome);
    Crime(QString category);
    QString m_category;
    QString m_outcome;
};

class CrimesDto
{
public:
    CrimesDto();

    typedef std::map<QString, std::vector<Crime>> CrimesMap;

    CrimesMap m_crimesMap;
    void addCrime(Crime crime);
};

#endif // CRIMESDTO_H
