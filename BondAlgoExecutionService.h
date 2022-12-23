//
//  BondAlgoExecutionService.h
//  Bond Trading System
//
//

#ifndef BondAlgoExecutionService_h
#define BondAlgoExecutionService_h
#include<string>
#include <iostream>
#include "soa.hpp"
#include "marketdataservice.hpp"
#include "executionservice.hpp"
using namespace std;

template <class T>
class AlgoExecution 
{
private:
    ExecutionOrder<T>* order;
    Market market;
    static int counter;

public:
    AlgoExecution(OrderBook<T> data) : market(CME)
    {
        double price = 0.0;
        long quantity = 0.0;

        counter++;
        PricingSide side = PricingSide(counter % 2);
        string orderID = to_string(counter);
        T product = data.GetProduct();
        double hidden_ratio = 0.9;

        if (side == BID) 
        {
            price = data.GetBidStack()[0].GetPrice();
            quantity = data.GetOfferStack()[0].GetQuantity();
        }

        order = new ExecutionOrder<T>(product, side, orderID, MARKET, price, quantity, quantity * hidden_ratio, orderID, false);

    }

    ExecutionOrder<T> GetOrder()
    {
        return *order;
    }

    Market GetMarket() 
    {
        return market;
    }

    ~AlgoExecution() 
    {
        delete order;
    }
};

template <class T>
int AlgoExecution<T>::counter = 0;
template <class T>
class BondAlgoExecutionService : public Service<string, AlgoExecution<T>> 
{
private:

public:
    void Execute(OrderBook<T> data) 
    {
        double best_bid = data.GetBidStack()[0].GetPrice();
        double best_offer = data.GetOfferStack()[0].GetPrice();
        double spread = best_offer - best_bid;
        double tol = 1. / 127;

        if (spread <= tol) 
        {
            AlgoExecution<T> algo_execution(data);
            Service<string, AlgoExecution<T> >::Notify(algo_execution);
        }
    }
};



#endif /* BondAlgoExecutionService_h */