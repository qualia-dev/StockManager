#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock
{
private:
    std::string m_symbol;
    std::string m_name;
    std::string m_company_name;
    std::string m_marketplace_name;
    std::string m_market_category;
    std::string m_record_date;
    std::string m_source_data;

    // Optional fields
    std::string m_test_issue;
    std::string m_financial_status;
    std::string m_round_lot;
    std::string m_etf;
    std::string m_next_shares;

public:
    Stock(const std::string& symbol, const std::string& name, const std::string& marketplace, const std::string& category);
    Stock();

    void setSymbol(const std::string& symbol) { m_symbol = symbol; }
    void setName(const std::string& name) { m_name = name; }
    void setMarketplaceName(const std::string& marketplaceName) { m_marketplace_name = marketplaceName; }
    void setMarketCategory(const std::string& category) { m_market_category = category; }
    void setRecordDate(const std::string& date) { m_record_date = date; }

    void setTestIssue(const std::string& testIssue) { m_test_issue = testIssue; }
    void setFinancialStatus(const std::string& status) { m_financial_status = status; }
    void setRoundLot(const std::string& roundLot) { m_round_lot = roundLot; }
    void setETF(const std::string& etf) { m_etf = etf; }
    void setNextShares(const std::string& nextShares) { m_next_shares = nextShares; }

    std::string symbol() const { return m_symbol; }
    std::string name() const { return m_name; }
    std::string marketplaceName() const { return m_marketplace_name; }  // Getter for the marketplace
    std::string marketCategory() const { return m_market_category; }
    std::string recordDate() const { return m_record_date; }

    std::string testIssue() const { return m_test_issue; }
    std::string financialStatus() const { return m_financial_status; }
    std::string roundLot() const { return m_round_lot; }
    std::string etf() const { return m_etf; }
    std::string nextShares() const { return m_next_shares; }
};

#endif // STOCK_H
