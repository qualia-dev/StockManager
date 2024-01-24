#include <cstring>
#include <vector>

#include "index.h"

Index::Index() {}

Index::Index(int id, const std::string &symbol, const std::string &name, int nb_components, double value, const std::string &currency,
             int market_id, const std::string &market_name, const std::string &description, const std::string &source_dwnd_compo,
             const std::string &last_official_publication, const std::string &last_update) :
    _id(id), _symbol(symbol), _name(name), _nb_components(nb_components), _value(value), _currency(currency),
    _market_id(market_id), _market_name(market_name), _description(description), _source_dwnd_compo(source_dwnd_compo),
    _last_official_publication(last_official_publication), _last_update(last_update)
{

}

bool Index::deserialize(void *user_param, char **cols, int nb_col)
{
    if (nb_col != 12)
        return false;

    Index index;

    if (cols[0] != NULL) index.set_id (atoi(cols[0]));
    else index.set_id (-1);

    if (cols[1] != NULL) index.set_symbol (cols[1]);
    else index.set_symbol ("NULL");

    if (cols[2] != NULL) index.set_name (cols[2]);
    else index.set_name ("NULL");

    if (cols[3] != NULL) index.set_nb_components (atoi(cols[3]));
    else index.set_nb_components (-1);

    if (cols[4] != NULL) index.set_value (atof(cols[4]));
    else index.set_value (-1);

    if (cols[5] != NULL) index.set_currency (cols[5]);
    else index.set_currency ("NULL");

    if (cols[6] != NULL) index.set_market_id (atoi(cols[6]));
    else index.set_market_id (-1);

    if (cols[7] != NULL) index.set_market_name (cols[7]);
    else index.set_market_name ("NULL");

    if (cols[8] != NULL) index.set_description (cols[8]);
    else index.set_description ("NULL");

    if (cols[9] != NULL) index.set_source_dwnd_compo (cols[9]);
    else index.set_source_dwnd_compo ("NULL");

    if (cols[10] != NULL) index.set_last_official_publication (cols[10]);
    else index.set_last_official_publication ("NULL");

    if (cols[11] != NULL) index.set_last_update (cols[11]);
    else index.set_last_update ("NULL");

    std::vector<Index>* v_indexes = (std::vector<Index>*) user_param;
    v_indexes->push_back(index);

    return true;
}
