#include "stock.h"

Stock::Stock() {}


Stock::Stock(int id, const std::string& symbol, const std::string& name, int _company_id,  const std::string& company_name,
             const std::string& isin, int marketplace_id, const std::string& marketplace_name, const std::string&  market_category,
             const std::string& record_date, const std::string& source_data, const std::string& last_updated)
                    : _id(id), _symbol(symbol), _name(name), _company_id(_company_id), _company_name(company_name),
                      _isin(isin), _marketplace_id(marketplace_id), _marketplace_name(marketplace_name), _market_category(market_category),
                      _record_date(record_date), _source_data(source_data), _last_updated(last_updated)
{

}

bool Stock::deserialize(char **row, int nb_col)
{
    if (nb_col != 12)
        return false;

    _id = atoi(row[0]);
    _symbol = row[1];
    _name = row[2];
    _company_id = atoi(row[3]);
    _company_name = row[4];
    _isin = row[5];
    _marketplace_id = atoi(row[6]);
    _marketplace_name = row[7];
    _market_category = row[8];
    if (row[9] != NULL) _record_date = row[9];
    else _record_date = "";
    if (row[9] != NULL) _source_data = row[10];
    else _source_data = "";
    if (row[9] != NULL) _last_updated = row[11];
    else _last_updated = "";

    return true;
}
