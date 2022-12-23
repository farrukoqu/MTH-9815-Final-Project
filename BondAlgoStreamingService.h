//
//  BondAlgoStreamingService.h
//  Bond Trading System
//
//

#ifndef BondAlgoStreamingService_h
#define BondAlgoStreamingService_h
#include "soa.hpp"
#include "streamingservice.hpp"
#include "pricingservice.hpp"

template <class V>
class BondAlgoStreamingService : public Service<string, PriceStream <V>> 
{
public:
    void PublishPrice(Price<V>& data) {
        double bid_price = data.GetMid() - data.GetBidOfferSpread() / 2;
        double ask_price = data.GetMid() + data.GetBidOfferSpread() / 2;
        PriceStreamOrder bid_order(bid_price, 1000000, 1000000, BID);
        PriceStreamOrder ask_order(ask_price, 1000000, 1000000, OFFER);
        PriceStream<Bond> price_stream(data.GetProduct(), bid_order, ask_order);
        Service<string, PriceStream <V> >::Notify(price_stream);
    }
};
#endif /* BondAlgoStreamingService_h */