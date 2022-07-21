#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>

class GlobalVars
{
public:
    GlobalVars();

    int maxYear;
    int maxMonth;
    int earliestYear;
    int earliestMonth;
    int startMonth;
    int endMonth;
    double initialLat;
    double initialLong;
    double minLat;
    double minLng;
    double maxLat;
    double maxLng;
    QString earliestMonthText;
};

#endif // GLOBALVARS_H
