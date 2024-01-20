#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include "marketnasdaq.h"
#include "ftpdownloader.h"
#include "stock.h"

MarketNasdaq::MarketNasdaq(SqliteWrap* db)
{
    _db = db;
    // initialize current marketplace - TODO : should be done from the values in the database
    _marketplace = new Marketplace(1, "Nasdaq", "Nasdaq Stock Market", 2, "USA");
}

bool MarketNasdaq::dwnd_list_securities(const std::string& server, unsigned int port, const std::string& remoteFilePath, const std::string& localFilePath)
{
    FtpDownloader ftp_downloader(server, port);

    if (!ftp_downloader.downloadFile(remoteFilePath, localFilePath))
    {
        std::cout << "Downloaded file from FTP server: " << remoteFilePath << std::endl;
    }
    else
    {
        std::cout << "Failed to download file from FTP server: " << remoteFilePath << std::endl;
        return false;
    }

    std::vector<char> buff_securities = ftp_downloader.buffer();

    // Convert the vector<char> to vector<std::string>
    std::stringstream ss(std::string(buff_securities.begin(), buff_securities.end()));

    std::vector<std::string> lines_securities;
    std::string line;

    while (std::getline(ss, line)) {
        lines_securities.push_back(line);
    }

    return extract_securities (lines_securities);
}

bool MarketNasdaq::extract_securities_from_file()
{
    // open read file and extract lines to vector<char>
    std::string file_path = "../data/nasdaqlisted10.txt";
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return false;
    }

    file.seekg(0, std::ios::beg);               // Move back to the beginning

    std::vector<std::string> lines_securities;
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {
        lines_securities.push_back(line);
    }

    file.close();   // Close the file

    return extract_securities (lines_securities);
}

bool MarketNasdaq::extract_securities(std::vector<std::string>& v_securities)
{
    if (v_securities.empty()) return true;  // it s not an error

    // get the last line containing date-time of the dataset, extract date-time and format to YYYY-MM-DD HH:MM:SS
    std::string record_date = v_securities.back();
    std::size_t found = record_date.find("File Creation Time:");
    if (found != std::string::npos) {
        record_date = record_date.substr(found + sizeof("File Creation Time:") - 1);
    } else {
        std::cerr << "Date not found in the last element of the vector." << std::endl;
        return false;
    }

    record_date.erase(0, record_date.find_first_not_of(" \t\r\n|"));    // Trim leading and trailing spaces or delimiters |
    record_date.erase(record_date.find_last_not_of(" \t\r\n|") + 1);

    // format the date and time
    std::istringstream iss(record_date);
    tm tm = {};
    iss >> std::get_time(&tm, "%m%d%Y%H:%M");

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");    // convert to YYYY-MM-DD HH:MM:SS
    record_date = oss.str();

    // Get the current time
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm = *std::localtime(&now);
    oss.str("");  // Clear the content of the ostringstream
    oss.clear();   // Reset the error state flags
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::string last_updated = oss.str();

    // prepare the stream to iterate over the vector
    std::stringstream ss;
    for (const auto& line : v_securities) {
        ss << line << '\n';  // Add newline after each line
    }

    std::string line;
    std::getline(ss, line);  // Skip the first line

    int count = 0;
    int count_error = 0;
    while (std::getline(ss, line)) {
        std::stringstream line_ss(line);

        std::string symbol, name, market_category, test_issue, financial_status;
        std::string round_lot, etf, next_shares, record_date;

        // Extract fields from each line
        std::getline(line_ss, symbol, '|');
        std::getline(line_ss, name, '|');

        // name contains the type of security (Common Stock, Common Shares, Warrant ...)
        // it is located after the name separated by ' - '
        // keep only 'Common Stock'
        size_t lastDashPos = name.rfind('-');
        if (lastDashPos != std::string::npos && name.substr(lastDashPos + 1).find("Common Stock") != std::string::npos) {
            // Remove " - Common Stock" from the name
            name = name.substr(0, lastDashPos);

            // remove trailing spaces from name
            name.erase(name.find_last_not_of(" \t\r\n") + 1);
        }
        else {
            // skip this line
            continue;
        }

        std::getline(line_ss, market_category, '|');
        std::getline(line_ss, test_issue, '|');
        std::getline(line_ss, financial_status, '|');
        std::getline(line_ss, round_lot, '|');
        std::getline(line_ss, etf, '|');
        std::getline(line_ss, next_shares, '|');
        next_shares.erase(next_shares.end() - 1);  //remove \r from the end of the string

        if (test_issue == "Y") continue;        // Skip test issues
        if (financial_status != "N") continue;  // Normal (Default): Issuer Is NOT Deficient, Delinquent, or Bankrupt.
        if (etf != "N") continue;               // Skip ETFs
        if (next_shares != "N") continue;       // Skip NextShares

        Stock s;
        s.setId(-1);
        s.setSymbol(symbol);
        s.setName(name);
        s.setCompanyId(-1);
        s.setCompanyName("tbd");
        s.setIsin("NULL");
        s.setMarketplaceId(_marketplace->id());
        s.setMarketplaceName(_marketplace->name());
        if (market_category == "Q" )s.setMarketCategory("Global Select Market");
        else if (market_category == "G") s.setMarketCategory("Global Market");
        else if (market_category == "S") s.setMarketCategory("Capital Market");
        else s.setMarketCategory("Error Not Q-G-S");
        s.setRecordDate(record_date);
        s.setSourceData("NASDAQ-Listed Securities FTP Download");
        s.setLastUpdated(last_updated);

        //insert stock in database
        std::string sql = "INSERT INTO Stock (symbol, name, company_id, ISIN, marketplace_id, marketplace_category, record_date, source_data, last_updated) VALUES ("
                          "'" + s.symbol() + "', "
                                         "'" + s.name() + "', ";

        if (s.companyId() == -1) sql += "NULL, ";
        else sql += std::to_string(s.companyId()) + ", ";
        //sql += "1, ";

        if (s.isin() == "NULL") sql += s.isin() + ", ";
        else sql += "'" + s.isin() + "', ";
        //sql += "'ISIN', ";

        sql += std::to_string(s.marketplaceId()) + ", "
                        "'" + s.marketCategory() + "', "
                        "'" + s.recordDate() + "', "
                        "'" + s.sourceData() + "', "
                        "'" + s.lastUpdated() + "');";

        bool res = _db->execute_sql(sql);
        count ++;

        if (!res) {
            std::cerr << "Error inserting stock in database." << std::endl;
            count_error++;
            continue;
        }

        std::cout << "DB Inserted ! " << count << " " << symbol << " " << name << " " << market_category << " " << test_issue << " " << financial_status << " " << round_lot << " " << etf << " " << next_shares << " " << std::endl;
    }

    std::cout << "Total stocks: " << count << " - Inserted : " << count-count_error << " - Error : " << count_error << std::endl;

    return true;
}


















