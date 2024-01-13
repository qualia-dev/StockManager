#ifndef COMPANY_H
#define COMPANY_H

#include <string>
class Company
{
public:
    Company(const std::string& name, const std::string& sector, const std::string& industry, int id);

private:

    std::string name;
    std::string sector;
    std::string industry;
    int id;

};

#endif // COMPANY_H
