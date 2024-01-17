#include <sstream>
#include <vector>

#include "marketplace.h"

Marketplace::Marketplace() {}


Marketplace::Marketplace(int id, const std::string& name, const std::string& name_full, int countryId, const std::string& country)
    : _id(id), _name(name), _name_full(name_full), _country_id(countryId), _country(country) {}


bool Marketplace::deserialize(void* user_param, char **row, int nb_col)
{
    if (nb_col != 5)
        return false;

    // because static
    Marketplace* mkp = new Marketplace();

    mkp->_id = atoi(row[0]);
    mkp->_name = row[1];
    mkp->_name_full = row[2];
    mkp->_country_id = atoi(row[3]);
    mkp->_country = row[4];

    std::vector<Marketplace>* v_marketplaces = (std::vector<Marketplace>*) user_param;
    v_marketplaces->push_back(*mkp);

    return true;
}

