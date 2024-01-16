#include "marketplace.h"

Marketplace::Marketplace() {}

Marketplace::Marketplace(int id, const std::string& name, const std::string& name_full, int countryId, const std::string& country)
    : _id(id), _name(name), _name_full(name_full), _country_id(countryId), _country(country) {}

bool Marketplace::deserialize(char **row, int nb_col)
{
    if (nb_col != 5)
        return false;

    _id = atoi(row[0]);
    _name = row[1];
    _name_full = row[2];
    _country_id = atoi(row[3]);
    _country = row[4];

    return true;
}
