/**
 * executionservice.hpp
 * Defines the data types and Service for executions.
 *
 * @author Breman Thuraisingham
 */
#ifndef EXECUTION_SERVICE_HPP
#define EXECUTION_SERVICE_HPP

#include <string>
#include "soa.hpp"
#include "marketdataservice.hpp"

enum OrderType { FOK, IOC, MARKET, LIMIT, STOP };

enum Market { BROKERTEC, ESPEED, CME };

/**
 * An execution order that can be placed on an exchange.
 * Type T is the product type.
 */
template<typename T>
class ExecutionOrder
{

public:

    // ctor for an order
    ExecutionOrder(const T& _product, PricingSide _side, string _orderId, OrderType _orderType, double _price, double _visibleQuantity, double _hiddenQuantity, string _parentOrderId, bool _isChildOrder) :
        product(_product)
    {
        side = _side;
        orderId = _orderId;
        orderType = _orderType;
        price = _price;
        visibleQuantity = _visibleQuantity;
        hiddenQuantity = _hiddenQuantity;
        parentOrderId = _parentOrderId;
        isChildOrder = _isChildOrder;
    }

    // Get the product
    const T& GetProduct() const
    {
        return product;
    }

    // Get the order ID
    const string& GetOrderId() const
    {
        return orderId;
    }

    // Get the order type on this order
    OrderType GetOrderType() const
    {
        return orderType;
    }

    // Get the price on this order
    double GetPrice() const
    {
        return price;
    }

    // Get the visible quantity on this order
    long GetVisibleQuantity() const
    {
        return visibleQuantity;
    }

    // Get the hidden quantity
    long GetHiddenQuantity() const
    {
        return hiddenQuantity;
    }

    // Get the parent order ID
    const string& GetParentOrderId() const
    {
        return parentOrderId;
    }

    // Is child order?
    bool IsChildOrder() const
    {
        return isChildOrder;
    }

    // get pricing side
    PricingSide GetPricingSide() const
    {
        return side;
    }

private:
  T product;
  PricingSide side;
  string orderId;
  OrderType orderType;
  double price;
  double visibleQuantity;
  double hiddenQuantity;
  string parentOrderId;
  bool isChildOrder;

};

/**
 * Service for executing orders on an exchange.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class ExecutionService : public Service<string,ExecutionOrder <T>>
{

public:
    
    // Execute an order on a market
    void ExecuteOrder(ExecutionOrder<T> order, Market market)
    {
        // Notify the trading system about our order.
        Service<string, ExecutionOrder<T>>::Notify(order);

    }

};

#endif