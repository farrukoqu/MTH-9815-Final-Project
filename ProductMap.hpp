//
//  ProductMap.hpp
//  Bond Trading System
//
//

//this is to give a map to map different products
#ifndef ProductMap_hpp
#define ProductMap_hpp

#include <vector>
#include <map>
#include "products.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
using namespace boost::gregorian;
using namespace std;

class ProductMap
{
public:
    static map<string, Bond> GetProductMap()
    {
        map<string, Bond> product_map;
        vector<Bond> bonds;
        bonds.push_back(Bond("B02y", CUSIP, "B02y", 0.05, date(2024, 12, 23)));
        bonds.push_back(Bond("B03y", CUSIP, "B03y", 0.05, date(2025, 12, 23)));
        bonds.push_back(Bond("B05y", CUSIP, "B05y", 0.05, date(2027, 12, 23)));
        bonds.push_back(Bond("B07y", CUSIP, "B07y", 0.05, date(2029, 12, 23)));
        bonds.push_back(Bond("B10y", CUSIP, "B10y", 0.05, date(2032, 12, 23)));
        bonds.push_back(Bond("B20y", CUSIP, "B20y", 0.05, date(2042, 12, 23)));
        bonds.push_back(Bond("B30y", CUSIP, "B30y", 0.05, date(2052, 12, 23)));

        for (auto bond : bonds)
        {
            auto pair = make_pair(bond.GetProductId(), bond);
            product_map.insert(pair);
        }
        return product_map;
    }

    static vector<Bond> GetProducts() 
    {
        vector<Bond> bonds;
        bonds.push_back(Bond("B02y", CUSIP, "B02y", 0.05, date(2024, 12, 23)));
        bonds.push_back(Bond("B03y", CUSIP, "B03y", 0.05, date(2025, 12, 23)));
        bonds.push_back(Bond("B05y", CUSIP, "B05y", 0.05, date(2027, 12, 23)));
        bonds.push_back(Bond("B07y", CUSIP, "B07y", 0.05, date(2029, 12, 23)));
        bonds.push_back(Bond("B10y", CUSIP, "B10y", 0.05, date(2032, 12, 23)));
        bonds.push_back(Bond("B20y", CUSIP, "B20y", 0.05, date(2042, 12, 23)));
        bonds.push_back(Bond("B30y", CUSIP, "B30y", 0.05, date(2052, 12, 23)));
        return bonds;
    }

    static vector<string> GetTickers()
    {
        return vector<string>{"B02y", "B03y", "B05y", "B07y", "B10y", "B20y", "B30y"};
    }
};

#endif /* ProductMap_hpp */