#include "jsonmapping.h"

jsonmapping::jsonmapping()
{
    addCrimesJsonFields(crimesJsonFieldsEnum::CATEGORY, "category");
    addCrimesJsonFields(crimesJsonFieldsEnum::OUTCOME_STATUS, "outcome_status");

    addOutcomeStatusJsonFields(outcomeStatusJsonFieldsEnum::OUTCOME_STATUS_CATEGORY, "category");
}
