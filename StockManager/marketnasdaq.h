#ifndef MARKETNASDAQ_H
#define MARKETNASDAQ_H

#include <string>
#include <vector>



class MarketNasdaq
{
public:
    MarketNasdaq();

public:
    bool dwnd_list_securities(const std::string& server, unsigned int port, const std::string& remoteFilePath, const std::string& localFilePath);
    bool extract_securities_from_file();
    bool extract_securities(std::vector<std::string>& v_securities);
};

#endif // MARKETNASDAQ_H
