#include "company.h"


Company::Company()
{

}

Company::Company (int id, const std::string& name, const std::string& sector, const std::string& industry, int countryId, const std::string& country)
: _id(id), _name(name), _sector(sector), _industry(industry), _country_id(countryId), _country(country)
{

}

