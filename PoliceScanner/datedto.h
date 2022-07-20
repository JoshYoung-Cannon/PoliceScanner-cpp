#ifndef DATEDTO_H
#define DATEDTO_H


class DateDto
{
public:
    DateDto(int year, int month);
    DateDto();
    int getYear(){return m_year;};
    void setYear(int year){m_year = year;};
    int getMonth(){return m_month;};
    void setMonth(int month){m_month = month;};

private:
    int m_year;
    int m_month;
};

#endif // DATEDTO_H
