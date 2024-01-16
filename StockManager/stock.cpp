#include "stock.h"
#include <cstring>

Stock::Stock() {}


Stock::Stock(int id, const std::string& symbol, const std::string& name, int _company_id,  const std::string& company_name,
             const std::string& isin, int marketplace_id, const std::string& marketplace_name, const std::string&  market_category,
             const std::string& record_date, const std::string& source_data, const std::string& last_updated)
                    : _id(id), _symbol(symbol), _name(name), _company_id(_company_id), _company_name(company_name),
                      _isin(isin), _marketplace_id(marketplace_id), _marketplace_name(marketplace_name), _market_category(market_category),
                      _record_date(record_date), _source_data(source_data), _last_updated(last_updated)
{

}

bool Stock::deserialize(char **cols, int nb_col)
{
    if (nb_col != 12)
        return false;

    _id = atoi(cols[0]);
    _symbol = cols[1];
    _name = cols[2];

    if (cols[3] != NULL && strcmp(cols[3], "NULL") != 0) _company_id = atoi(cols[3]);
    else _company_id = -1;

    if (cols[3] != NULL && strcmp(cols[3], "NULL") != 0) _company_name = cols[4];
    else _company_name = "";

    if (cols[3] != NULL && strcmp(cols[3], "NULL") != 0) _isin = cols[5];
    else _isin = "";

    _marketplace_id = atoi(cols[6]);
    _marketplace_name = cols[7];
    _market_category = cols[8];
    if (cols[9] != NULL) _record_date = cols[9];
    else _record_date = "";
    if (cols[9] != NULL) _source_data = cols[10];
    else _source_data = "";
    if (cols[9] != NULL) _last_updated = cols[11];
    else _last_updated = "";

    return true;
}


