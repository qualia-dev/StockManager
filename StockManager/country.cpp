#include "country.h"




Country::Country() {}

Country::Country(int id, const std::string& name, const std::string& name_fr, const std::string& continent)
    : _id(id), _name(name), _name_fr(name_fr), _continent(continent) {}
