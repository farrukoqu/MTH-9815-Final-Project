/**
 * historicaldataservice.hpp
 * historicaldataservice.hpp
 *
 * @author Breman Thuraisingham
 * Defines the data types and Service for historical data.
 *
 * @author Breman Thuraisingham
 */
#ifndef HISTORICAL_DATA_SERVICE_HPP
#define HISTORICAL_DATA_SERVICE_HPP

#include<string>
#include "soa.hpp"
#include "products.hpp"
#include "riskservice.hpp"
#include "positionservice.hpp"
#include "Connectors.hpp"
#include "streamingservice.hpp"

using namespace std;

/**
 * Service for processing and persisting historical data to a persistent store.
 * Keyed on some persistent key.
 * Type T is the data type to persist.
 */
template<typename T>
class HistoricalDataService : Service<string, T>
{

public:

  // Persist data to a store
  void PersistData(string persistKey, const T& data) {}

};

template<typename V>
class PositionHistoricalData : HistoricalDataService<Position<V>>
{
private:
    PositionConnector<V>* position_connector;
public:
    explicit PositionHistoricalData(PositionConnector<V>* position_connector_) : position_connector(position_connector_) {}

    virtual void PersistData(string persistKey, Position<V>& position)
    {
        position_connector->Publish(position);
    }
};

template<typename V>
class RiskHistoricalData : HistoricalDataService<PV01<V>>
{
private:
    RiskConnector<V>* risk_connector;
public:
    explicit RiskHistoricalData(RiskConnector<V>* risk_connector_) : risk_connector(risk_connector_) {}

    void PersistData(string persistKey, const PV01<V>& risk)
    {
        risk_connector->Publish(risk);
    }
};
template<typename V>
class StreamingHistoricalDataService : HistoricalDataService<PriceStream <V>>
{
private:
    StreamingConnector<V>* streaming_connector;
public:
    explicit StreamingHistoricalDataService(StreamingConnector<V>* streaming_connector_) : streaming_connector(streaming_connector_) {}

    void PersistData(string persistKey, const PriceStream <V>& streaming)
    {
        streaming_connector->Publish(streaming);
    }
};

template<typename V>
class ExecutionHistoricalService : HistoricalDataService<ExecutionOrder<V>>
{
private:
    ExecutionConnector<V>* execution_connector;
public:
    explicit ExecutionHistoricalService(ExecutionConnector<V>* execution_connector_) : execution_connector(execution_connector_) {}

    void PersistData(string persistKey, const ExecutionOrder<V>& execution)
    {
        execution_connector->Publish(execution);
    }
};

template<typename V>
class InquiryHistoricalService : HistoricalDataService<Inquiry<V>>
{
private:
    AllInquiriesConnector<V>* all_inquiries_connector;
public:
    explicit InquiryHistoricalService(AllInquiriesConnector<V>* all_inquiries_connector_) : all_inquiries_connector(all_inquiries_connector_) {}

    void PersistData(string persistKey, const Inquiry<V>& inquiry)
    {
        all_inquiries_connector->Publish(inquiry);
    }
};


#endif