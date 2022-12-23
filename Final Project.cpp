//
//  FinalProject.cpp
//
//
//

// include all the files needed to run the system.
#include<iostream>
#include "products.hpp"
#include "positionservice.hpp"
#include "tradebookingservice.hpp"
#include "soa.hpp"
#include "Listeners.hpp"
#include "pricingservice.hpp"
#include "riskservice.hpp"
#include "Connectors.hpp"
#include "GUIService.hpp"
#include "historicaldataservice.hpp"
#include "BondAlgoExecutionService.h"
#include "executionservice.hpp"
#include "inquiryservice.hpp"
using namespace std;


int main()
{
    cout << "Bond Trading System is executing" << endl;

    // We create the following services: trade_booking, position
    TradeBookingService<Bond> trade_booking_service;
    PositionService<Bond> position_service;

    // Create and add the position listener to the trade booking service
    PositionServiceListener<Bond> position_listener(&position_service);
    trade_booking_service.AddListener(&position_listener);

    //  Create a Position Connector and create a PositionHistoricalData with Position Connector as input
    PositionConnector<Bond> position_connector_instance("output/firstpositions.txt");
    PositionHistoricalData<Bond> position_history(&position_connector_instance);

    // Create a History Position Listener with positionhistoricalData as input then add the listener to position_service
    HistPositionListener<Bond> history_position_listener(&position_history);
    position_service.AddListener(&history_position_listener);

    // Create a RiskService then add it to a risk Service listener then add the risk service listener to the position listener.
    RiskService<Bond> risk_service_instance;
    RiskServiceListener<Bond> risk_service_listener(&risk_service_instance);
    position_service.AddListener(&risk_service_listener);

    // create a Risk Connector then use it to create a Risk Historical Data object.
    RiskConnector<Bond> risk_connector("output/firstrisk.txt");
    RiskHistoricalData<Bond> risk_h(&risk_connector);

    // Add Risk Historical Data to its Historical Risk Listener
    HistRiskListener<Bond> history_risk_listener(&risk_h);
    risk_service_instance.AddListener(&history_risk_listener);

    // Generate the trades 
    TradeBookingConnector<Bond> trade_connector("data_generators/trades.txt", &trade_booking_service);
    trade_connector.TraverseTrades();

    // Create the GUI Connector which is used to create the GUI Service which is used to create the GUI Listener. We then add the Listener to the Pricing Listener.
    GUIConnector<Bond> gui_connector("output/gui.txt");
    GUIService<Bond> gui_service(&gui_connector);
    GUIListener<Bond> gui_listener(&gui_service);
    PricingService<Bond> pricing_service;
    pricing_service.AddListener(&gui_listener);

    // Now we create a Bond Algo Streaming to create its listener and add the listener to the pricing service listener
    BondAlgoStreamingService<Bond> bond_algo_streaming_service;
    AlgoStreamingListener<Bond> algo_streaming_listener(&bond_algo_streaming_service);
    pricing_service.AddListener(&algo_streaming_listener);

    // We create the streaming service to add to its listener to the Bond Algo Streaming Service listener.
    StreamingService<Bond> streaming_service;
    StreamingListener<Bond> streaming_listener(&streaming_service);
    bond_algo_streaming_service.AddListener(&streaming_listener);

    // Create a streaming connector to create Streaming Historical Data Service which we then use to create its listener and add it to the steaming listener.
    StreamingConnector<Bond> streaming_connector("output/streaming.txt");
    StreamingHistoricalDataService<Bond> streaming_hist_service(&streaming_connector);
    HistStreamingListener<Bond> hist_streaming_listener(&streaming_hist_service);
    streaming_service.AddListener(&hist_streaming_listener);
    PricingConnector<Bond> pricing_connector("data_generators/prices.txt", &pricing_service);
    pricing_connector.Subscribe();

    // Create a Bond Algo Execution Service and use it to create a Listener.
    BondAlgoExecutionService<Bond> bond_algo_execution_service_ins;
    BondAlgoExecutionListener<Bond> bond_algo_execution_listener_ins(&bond_algo_execution_service_ins);

    // Create a Market Data Service and add the bond algo execution listener
    MarketDataService<Bond> mrkt_data_service_ins;
    mrkt_data_service_ins.AddListener(&bond_algo_execution_listener_ins);

    // Create an Execution Service and create its listener by passing it in.
    ExecutionService<Bond> execution_service_ins;
    ExecutionServiceListener<Bond> execution_service_listener_ins(&execution_service_ins);
    bond_algo_execution_service_ins.AddListener(&execution_service_listener_ins);

    // Create a TradeBooking Service and its listener. Then add the listener to its execution service.
    TradeBookingService<Bond> trade_booking_service_ins;
    TradeBookingServiceListener<Bond> trade_booking_listener_ins(&trade_booking_service_ins);
    execution_service_ins.AddListener(&trade_booking_listener_ins);

    // Create a Position Service and its listener and add the listener to its trade_booking_service
    PositionService<Bond> pos_service_ins;
    PositionServiceListener<Bond> position_listener_ins(&pos_service_ins);
    trade_booking_service_ins.AddListener(&position_listener_ins);

    // Create a Position Connector and use it create Position Historical Data and Listener
    PositionConnector<Bond> position_connector_ins("output/positions.txt");
    PositionHistoricalData<Bond> position_history_ins(&position_connector_ins);
    HistPositionListener<Bond> history_position_listener_ins(&position_history_ins);
    pos_service_ins.AddListener(&history_position_listener_ins);

    // Create a Risk Service, its listener, connect the listener to its position service
    RiskService<Bond> risk_service;
    RiskServiceListener<Bond> risk_service_listener_ins(&risk_service);
    pos_service_ins.AddListener(&risk_service_listener_ins);

    // Create a Risk connector and createa Historical Risk Data and its listener then add that to its risk service listener
    RiskConnector<Bond> risk_connector_ins("output/risk.txt");
    RiskHistoricalData<Bond> risk_history(&risk_connector_ins);
    HistRiskListener<Bond> history_risk_listener_ins(&risk_history);
    risk_service.AddListener(&history_risk_listener_ins);

    // Create an Execution Connector, Execution Historical Service, Execution Historical Data Service Listener and add it to Execution Service
    ExecutionConnector<Bond> execution_connector("output/executions.txt");
    ExecutionHistoricalService<Bond> execution_hist_service(&execution_connector);
    ExecutionHistoricalDataServiceListener<Bond> execution_hist_service_listener(&execution_hist_service);
    execution_service_ins.AddListener(&execution_hist_service_listener);

    // Create a Market data connector
    MrktDataConnector<Bond> mrkt_data_connector("data_generators/marketdata.txt", &mrkt_data_service_ins);
    mrkt_data_connector.Subscribe();

    
    // Create an All Inquiries Connector and use it to create an Inquiry Historical Service and use that to create All Inquiry Historical Data Service Listener
    AllInquiriesConnector<Bond> all_inquiries_connector("output/allinquiries.txt");
    InquiryHistoricalService<Bond> inquiry_hist_service(&all_inquiries_connector);
    AllInquiryHistoricalDataServiceListener<Bond> all_inquiry_hist_listner(&inquiry_hist_service);

    // Create an Inquiry Service and add the inquiry historical listener
    InquiryService<Bond> inquiry_service;
    inquiry_service.AddListener(&all_inquiry_hist_listner);
    InquiryConnector<Bond> inquiry_connector("data_generators/inquiries.txt", &inquiry_service);
    inquiry_connector.Subscribe();
    cout << "Bond Trading System is completed." << endl;

    return 0;
}