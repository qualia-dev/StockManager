#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <string>
class Marketplace
{
public:
    Marketplace();
    Marketplace(int id, const std::string& name, const std::string& name_full, int countryId, const std::string& country);

private:
    int _id;
    std::string _name;
    std::string _name_full;
    int _country_id;
    std::string _country;

public:
    //getters
    int id() const { return _id; }
    std::string name() const { return _name; }
    std::string name_full() const { return _name_full; }
    int countryId() const { return _country_id; }
    std::string country() const { return _country; }

    //setters
    void setId(int id) { _id = id; }
    void setName(std::string name) { _name = name; }
    void setName_full(std::string name_full) { _name_full = name_full; }
    void setCountryId(int countryId) { _country_id = countryId; }
    void setCountry(std::string country) { _country = country; }

    bool deserialize (char** row, int nb_col);
};

#endif // MARKETPLACE_H
