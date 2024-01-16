#include "marketplace.h"

Marketplace::Marketplace() {}

Marketplace::Marketplace(int id, const std::string& name, const std::string& name_full, int countryId, const std::string& country)
    : _id(id), _name(name), _name_full(name_full), _country_id(countryId), _country(country) {}
