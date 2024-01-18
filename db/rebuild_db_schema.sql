@@@sql@@@
CREATE TABLE Country (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, name_fr TEXT UNIQUE NOT NULL, continent TEXT NOT NULL)
@@@sql@@@
CREATE TABLE Marketplace (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, name_full TEXT, country_id INTEGER NOT NULL REFERENCES Country (id))
@@@sql@@@
CREATE TABLE Company (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, name_short, sector_yh TEXT, industry_yh TEXT, country_id INTEGER REFERENCES Country (id) NOT NULL)
@@@sql@@@
CREATE VIEW MarketplaceView AS SELECT
    Marketplace.id,
    Marketplace.name,
    Marketplace.name_full,
    Marketplace.country_id,
    Country.name AS country_name
FROM
    Marketplace
JOIN
    Country ON Marketplace.country_id = Country.id
@@@sql@@@
CREATE TABLE Stock (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, symbol TEXT UNIQUE NOT NULL, name TEXT UNIQUE NOT NULL, company_id INTEGER REFERENCES Company (id), ISIN TEXT, marketplace_id REFERENCES Marketplace (id) NOT NULL, marketplace_category TEXT, record_date INTEGER, source_data TEXT, last_updated)
@@@sql@@@
CREATE VIEW StockView AS SELECT
    Stock.id,
    Stock.symbol,
    Stock.name,
    Stock.company_id,
    Company.name_short AS company_name_short,
    Stock.ISIN,
    Stock.marketplace_id,
    Marketplace.name AS marketplace_name,
    Stock.marketplace_category,
    Stock.record_date,
    Stock.source_data,
    Stock.last_updated
FROM
    Stock
LEFT JOIN
    Company ON Stock.company_id = Company.id
JOIN
    Marketplace ON Stock.marketplace_id = Marketplace.id
WHERE
    Stock.company_id IS NULL OR Company.id IS NOT NULL
