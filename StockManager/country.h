#ifndef COUNTRY_H
#define COUNTRY_H

#include <string>
class Country
{
public:
    Country();
    Country(int id, const std::string& name, const std::string& name_fr, const std::string& continent);

private:
    int _id;
    std::string _name;
    std::string _name_fr;
    std::string _continent;
public:
    //getters
    int id() const { return _id; }
    std::string name() const { return _name; }
    std::string name_fr() const { return _name_fr; }
    std::string continent() const { return _continent; }
    //setters
    void setId(int id) { _id = id; }
    void setName(std::string name) { _name = name; }
    void setName_fr(std::string name_fr) { _name_fr = name_fr; }
    void setContinent(std::string continent) { _continent = continent; }

};

#endif // COUNTRY_H
