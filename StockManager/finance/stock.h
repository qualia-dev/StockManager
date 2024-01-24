#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock
{
public:
    Stock(int id, const std::string& symbol, const std::string& name, int _company_id,  const std::string& company_name,
          const std::string& isin, int marketplace_id, const std::string& marketplace_name, const std::string&  market_category,
          const std::string& record_date, const std::string& source_data, const std::string& last_updated);
    Stock();

//private:
public:
    int _id;
    std::string _symbol;
    std::string _name;
    int _company_id;
    std::string _company_name;
    std::string _isin;
    int _marketplace_id;
    std::string _marketplace_name;
    std::string _market_category;    // no database table for this
    std::string _record_date;
    std::string _source_data;
    std::string _last_updated;


public:
    //getters
    int id() const { return _id; }
    std::string symbol() const { return _symbol; }
    std::string name() const { return _name; }
    int companyId() const { return _company_id; }
    std::string companyName() const { return _company_name; }
    std::string isin() const { return _isin; }
    int marketplaceId() const { return _marketplace_id; }
    std::string marketplaceName() const { return _marketplace_name; }  // Getter for the marketplace
    std::string marketCategory() const { return _market_category; }
    std::string recordDate() const { return _record_date; }
    std::string sourceData() const { return _source_data; }
    std::string lastUpdated() const { return _last_updated; }

    // setters
    void setId(int id) { _id = id; }
    void setSymbol(const std::string& symbol) { _symbol = symbol; }
    void setName(const std::string& name) { _name = name; }
    void setCompanyId(int companyId) { _company_id = companyId; }
    void setCompanyName(const std::string& companyName) { _company_name = companyName; }
    void setIsin(const std::string& isin) { _isin = isin; }
    void setMarketplaceId(int marketplaceId) { _marketplace_id = marketplaceId; }
    void setMarketplaceName(const std::string& marketplaceName) { _marketplace_name = marketplaceName; }
    void setMarketCategory(const std::string& category) { _market_category = category; }
    void setRecordDate(const std::string& date) { _record_date = date; }
    void setSourceData(const std::string& sourceData) { _source_data = sourceData; }
    void setLastUpdated(const std::string& lastUpdated) { _last_updated = lastUpdated; }

    // From db
    static bool deserialize (void* user_param, char** cols, int nb_col);
    static bool deserialize_compo_index (void* user_param, char** cols, int nb_col);

};

#endif // STOCK_H
