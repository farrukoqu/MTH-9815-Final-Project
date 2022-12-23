/**
 * positionservice.hpp
 * Defines the data types and Service for positions.
 *
 * @author Breman Thuraisingham
 */
#ifndef POSITION_SERVICE_HPP
#define POSITION_SERVICE_HPP

#include <string>
#include <map>
#include "soa.hpp"
#include "tradebookingservice.hpp"
#include "ProductMap.hpp"
#include <utility>
using namespace std;

/**
 * Position class in a particular book.
 * Type T is the product type.
 */
template<typename T>
class Position
{

public:

  // ctor for a position
  Position(const T & product_);

  // Get the product
  const T& GetProduct() const;

  // Get the position quantity
  long GetPosition(string &book);

  // Get the aggregate position
  long GetAggregatePosition();

  // Update position in the book
  void UpdatePosition(string& book, long quantity, Side side);

private:
  T product;
  map<string,long> positions;

};

/**
 * Position Service to manage positions across multiple books and secruties.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PositionService : public Service<string,Position <T>>
{

private:
	map<string, Position<T>> positions;

public:
	
	// Add a trade to the service
	PositionService() 
	{
		map<string, Bond> bond_map = ProductMap::GetProductMap();
		vector<string> tickers = ProductMap::GetTickers();

		for (auto ticker : tickers) 
		{
			Position<Bond> temp(bond_map.find(ticker)->second);
			positions.insert(make_pair(ticker, Position<Bond>(bond_map.find(ticker)->second)));
		}
	}

	// Add a trade to the service
	virtual void AddTrade(const Trade<T>& trade)
	{
		string ticker = trade.GetProduct().GetTicker();
		string book = trade.GetBook();
		long quantity = trade.GetQuantity();
		Side side = trade.GetSide();
		if (positions.find(ticker) != positions.end())
		{
			Position<Bond>& pos = positions.find(ticker)->second;
			pos.UpdatePosition(book, quantity, side);

		}
		else 
		{
			cout << "Unable to trade " << ticker << endl;
		}

		Service<string, Position<T>>::Notify(positions.find(ticker)->second);
	}

	 // Gets Position given the identifier
	 virtual Position <T>& GetPosition(string identifier)
	 {
		 return positions.find(identifier)->second;
	 }

};

template<typename T>
Position<T>::Position(const T & product_) : product(product_)
{
	string books[3]{ "TRSY1", "TRSY2", "TRSY3" };

	for (int i = 0; i < 3; i++)
	{
		positions.insert({ books[i], 0 });
	}
}

template<typename T>
const T& Position<T>::GetProduct() const
{
  return product;
}

template<typename T>
long Position<T>::GetPosition(string &book)
{
	try 
	{
		return positions[book];
	}
	catch (...)
	{
		cout << "position is not in " << book << endl;
	}
}

template<typename T>
long Position<T>::GetAggregatePosition()
{
	long result = 0;
	string books[3]{ "TRSY1","TRSY2","TRSY3" };

	for (int i = 0; i < 3; i++)
	{
		result += positions.find(books[i])->second;
	}

	return result;
}

template<typename T>
void Position<T>::UpdatePosition(string& book, long quantity, Side side)
{
	if (side == BUY)
	{
		positions[book] += quantity;
	}
	else if (side == SELL)
	{
		positions[book] -= quantity;
	}
}

#endif
