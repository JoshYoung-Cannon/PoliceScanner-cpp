#include "datedto.h"

DateDto::DateDto():
    m_year(2019)
  , m_month(2)
{  

}

DateDto::DateDto(int year, int month)
{
    m_year = year;
    m_month = month;
}
