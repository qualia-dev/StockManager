#ifndef INDEX_H
#define INDEX_H

#include <string>
class Index
{
public:
    Index();

    Index (int id, const std::string& symbol, const std::string& name, int nb_components, double value, const std::string& currency,
          int market_id, const std::string& market_name, const std::string& description, const std::string& source_dwnd_compo,
          const std::string& last_official_publication, const std::string& last_update);

private:
    int _id;
    std::string _symbol;
    std::string _name;
    int _nb_components;
    double _value;
    std::string _currency;
    int _market_id;
    std::string _market_name;
    std::string _description;
    std::string _source_dwnd_compo;
    std::string _last_official_publication;
    std::string _last_update;

public:
    // getters
    int id() const { return _id; }
    std::string symbol() const { return _symbol; }
    std::string name() const { return _name; }
    int nb_components() const { return _nb_components; }
    double value() const { return _value; }
    std::string currency() const { return _currency; }
    int market_id() const { return _market_id; }
    std::string market_name() const { return _market_name; }
    std::string description() const { return _description; }
    std::string source_dwnd_compo() const { return _source_dwnd_compo; }
    std::string last_official_publication() const { return _last_official_publication; }
    std::string last_update() const { return _last_update; }

    // setters
    void set_id(int id) { _id = id; }
    void set_symbol(const std::string& symbol) { _symbol = symbol; }
    void set_name(const std::string& name) { _name = name; }
    void set_nb_components(int nb_components) { _nb_components = nb_components; }
    void set_value(double value) { _value = value; }
    void set_currency(const std::string& currency) { _currency = currency; }
    void set_market_id(int market_id) { _market_id = market_id; }
    void set_market_name(const std::string& market_name) { _market_name = market_name; }
    void set_description(const std::string& description) { _description = description; }
    void set_source_dwnd_compo(const std::string& source_dwnd_compo) { _source_dwnd_compo = source_dwnd_compo; }
    void set_last_official_publication(const std::string& last_official_publication) { _last_official_publication = last_official_publication; }
    void set_last_update(const std::string& last_update) { _last_update = last_update; }

    // From db
    static bool deserialize (void* user_param, char** cols, int nb_col);

};

#endif // INDEX_H
