@@@sql@@@
CREATE TABLE T_Companies (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, name_short, sector_yh TEXT, industry_yh TEXT, country_id INTEGER REFERENCES Country (id) NOT NULL)
@@@sql@@@
CREATE TABLE T_Countries (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, name_fr TEXT UNIQUE NOT NULL, continent TEXT NOT NULL)
@@@sql@@@
CREATE TABLE T_Marketplaces (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, name_full TEXT, country_id INTEGER NOT NULL REFERENCES T_Country (id))
@@@sql@@@
CREATE TABLE T_Indexes (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, symbol TEXT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, number_components INTEGER NOT NULL, value REAL NOT NULL, currency TEXT NOT NULL, market_id INTEGER REFERENCES T_Marketplaces (id), description TEXT NOT NULL, source_download_compo TEXT NOT NULL, date_official_publication TEXT, last_updated TEXT NOT NULL)
@@@sql@@@
CREATE VIEW V_Indexes AS SELECT i.id AS index_id, i.symbol AS index_symbol, i.name AS index_name, i.number_components, i.value, i.currency, i.market_id, m.name AS marketplace_name, i.description, i.source_download_compo, i.date_official_publication, i.last_updated FROM T_Indexes i JOIN T_Marketplaces m ON i.market_id = m.id
@@@sql@@@
CREATE VIEW V_Marketplaces AS SELECT T_Marketplaces.id, T_Marketplaces.name, T_Marketplaces.name_full, T_Marketplaces.country_id, T_Countries.name AS country_name FROM T_Marketplaces JOIN T_Countries ON T_Marketplaces.country_id = T_Countries.id
@@@sql@@@
CREATE VIEW V_Stocks AS SELECT T_Stocks.id, T_Stocks.symbol, T_Stocks.name, T_Stocks.company_id, T_Companies.name_short AS company_name_short, T_Stocks.ISIN, T_Stocks.marketplace_id, T_Marketplaces.name AS marketplace_name, T_Stocks.marketplace_category, T_Stocks.record_date, T_Stocks.source_data, T_Stocks.last_updated FROM T_Stocks LEFT JOIN T_Companies ON T_Stocks.company_id = T_Companies.id JOIN T_Marketplaces ON T_Stocks.marketplace_id = T_Marketplaces.id WHERE T_Stocks.company_id IS NULL OR T_Companies.id IS NOT NULL
@@@sql@@@
CREATE VIEW V_IndexComposition AS SELECT s.id AS stock_id, s.symbol AS stock_symbol, s.name AS stock_name, s.marketplace_category AS marketplace_category, i.id AS index_id, i.symbol AS index_symbol, i.name AS index_name FROM T_Indexes i JOIN T_Stocks_In_Indexes sii ON i.id = sii.id_index JOIN T_Stocks s ON sii.id_stock = s.id
@@@sql@@@
CREATE TABLE T_Stocks (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, symbol TEXT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, company_id INTEGER REFERENCES Company (id), ISIN TEXT, marketplace_id INTEGER REFERENCES T_Marketplaces (id) NOT NULL, marketplace_category TEXT, industry TEXT DEFAULT NULL, sector TEXT DEFAULT NULL, record_date TEXT, source_data TEXT, last_updated TEXT)
@@@sql@@@
CREATE TABLE T_Stocks_In_Indexes (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, id_index INTEGER REFERENCES Indexes (id) NOT NULL, id_stock INTEGER REFERENCES Stock (id) NOT NULL, UNIQUE (id_stock, id_index))
