#ifndef COMPANY_H
#define COMPANY_H

#include <string>
class Company
{
public:
    Company();
    Company(int id, const std::string& name, const std::string& sector, const std::string& industry, int countryId, const std::string& country);

private:

    int _id;
    std::string _name;
    std::string _sector;     // no database table for this
    std::string _industry;   // no database table for this
    int _country_id;
    std::string _country;

public:
    //getters
    int id() const { return _id; }
    std::string name() const { return _name; }
    std::string sector() const { return _sector; }
    std::string industry() const { return _industry; }
    int countryId() const { return _country_id; }
    std::string country() const { return _country; }

    //setters
    void setId(int id) { _id = id; }
    void setName(std::string name) { _name = name; }
    void setSector(std::string sector) { _sector = sector; }
    void setIndustry(std::string industry) { _industry = industry; }
    void setCountryId(int countryId) { _country_id = countryId; }
    void setCountry(std::string country) { _country = country; }

    bool deserialize (char** row, int nb_col);
};

#endif // COMPANY_H
