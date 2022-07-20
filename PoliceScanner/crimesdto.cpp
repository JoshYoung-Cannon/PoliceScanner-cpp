#include "crimesdto.h"
#include <iostream>

Crime::Crime(QString category, QString outcome)
    :m_category(category)
    , m_outcome(outcome)
{
}

Crime::Crime(QString category)
    :m_category(category)
    , m_outcome("")
{

}

CrimesDto::CrimesDto()
    :m_crimesMap()
{

}

void CrimesDto::addCrime(Crime crime)
{
    if (m_crimesMap.find(crime.m_category) == m_crimesMap.end())
    {
        std::cout << "Adding New Category To Map" << std::endl;
        m_crimesMap[crime.m_category].push_back(crime);
        return;
    }
    m_crimesMap[crime.m_category].push_back(crime);
}
