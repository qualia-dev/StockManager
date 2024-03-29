#include "stock.h"
#include <cstring>
#include <vector>

Stock::Stock() {}


Stock::Stock(int id, const std::string& symbol, const std::string& name, int _company_id,  const std::string& company_name,
             const std::string& isin, int marketplace_id, const std::string& marketplace_name, const std::string&  market_category,
             const std::string& sector, const std::string& industry,
             const std::string& record_date, const std::string& source_data, const std::string& last_updated)
                    : _id(id), _symbol(symbol), _name(name), _company_id(_company_id), _company_name(company_name),
                      _isin(isin), _marketplace_id(marketplace_id), _marketplace_name(marketplace_name), _market_category(market_category),
                      _sector(sector), _industry(industry),
                      _record_date(record_date), _source_data(source_data), _last_updated(last_updated)
{

}

bool Stock::deserialize (void* user_param, char** cols, int nb_col)
{
    if (nb_col != 14)
        return false;

    Stock st;
    st.setId (atoi(cols[0]));
    st.setSymbol (cols[1]);
    st.setName (cols[2]);

    if (cols[3] != NULL && strcmp(cols[3], "NULL") != 0) st.setCompanyId (atoi(cols[3]));
    else st.setCompanyId (-1);

    if (cols[4] != NULL && strcmp(cols[3], "NULL") != 0) st.setCompanyName(cols[4]);
    else st.setCompanyName("");

    if (cols[5] != NULL && strcmp(cols[3], "NULL") != 0) st._isin = cols[5];
    else st._isin = "";

    st._marketplace_id = atoi(cols[6]);
    st._marketplace_name = cols[7];
    st._market_category = cols[8];
    st._sector = cols[9];
    st._industry = cols[10];
    if (cols[11] != NULL) st._record_date = cols[11];
    else st._record_date = "";
    if (cols[12] != NULL) st._source_data = cols[12];
    else st._source_data = "";
    if (cols[13] != NULL) st._last_updated = cols[13];
    else st._last_updated = "";

    std::vector<Stock>* v_stocks = (std::vector<Stock>*) user_param;
    v_stocks->push_back(st);

    return true;
}

bool Stock::deserialize_compo_index(void *user_param, char **cols, int nb_col)
{
    Stock st;
    st.setId (atoi(cols[0]));
    st.setSymbol (cols[1]);
    st.setName (cols[2]);
    st._market_category = cols[3];
    st._sector = cols[4];
    st._industry = cols[5];

    std::vector<Stock>* v_stocks = (std::vector<Stock>*) user_param;
    v_stocks->push_back(st);

    return true;
}


