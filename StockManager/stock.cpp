#include "stock.h"

Stock::Stock() {}

Stock::Stock(const std::string &symbol, const std::string &name, const std::string &marketplace, const std::string &category)
    : m_symbol(symbol)
    , m_name(name)
    , m_marketplace_name(marketplace)
    , m_market_category(category)
{

}

