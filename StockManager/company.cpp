#include "company.h"


Company::Company()
{

}

Company::Company (int id, const std::string& name, const std::string& sector, const std::string& industry, int countryId, const std::string& country)
: _id(id), _name(name), _sector(sector), _industry(industry), _country_id(countryId), _country(country)
{

}

bool Company::deserialize(char **row, int nb_col)
{
    if (nb_col != 6)
        return false;

    _id = atoi(row[0]);
    _name = row[1];
    _sector = row[2];
    _industry = row[3];
    _country_id = atoi(row[4]);
    _country = row[5];

    return true;
}

