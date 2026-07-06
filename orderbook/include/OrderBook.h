#pragma once
#include "Order.h"
#include "Trade.h"
#include <map>
#include <list>
#include <cstdint>
#include <vector>

/*
    Status codes for certain OrderBook operations.
*/
enum class ReturnCode {
    SUCCESS,
    INSUFFICIENT_MEMORY,
    UNDEFINED_ERROR,
    INVALID_REQUEST,
    MAX_ORDER_CAPACITY
};

/*
    Stored on every order via a map, id -> OrderIdInfo. Used during method 
    of Order lookup via id for efficiency.
*/
struct OrderIdInfo {
    Side side;
    long price;
};


class OrderBook {
private:
    // Stores Order class objects, while same price orders arranged in FIFO lists.

    // Highest price first (use comparator)
    std::map<long, std::list<Order>, std::greater<long>> bidOrders;

    // Lowest price first (default map behaviour)
    std::map<long, std::list<Order>> askOrders;
    
    // Maps id to order side and price, used for efficient lookups of an Order via id.
    std::map<uint64_t, OrderIdInfo> OrderIdInfoStore;

    // Stores history of all trades, frontmost trade is newest trade
    std::vector<Trade> tradeHistory;

    int size;
   
public:
    OrderBook();
    
    const OrderIdInfo* getOrderInfoViaId(uint64_t id) const;
    const Order* findOrderById(uint64_t id) const;

    ReturnCode addOrderToBook(Order order);
    void printOrderBookState() const;
    void cancelOrder(uint64_t id);
    void modifyOrder(uint64_t id, long newPrice, int newQuantity);
    void addTradeToHistory(Trade t);

    uint64_t getSize() const;
    bool bidEmpty() const;
    bool askEmpty() const;
    int getAmtOfBidOrders() const;
    int getAmtOfAskOrders() const;
    long getHighestBid() const;
    long getLowestAsk() const;

    Order* getFrontBid();
    Order* getFrontAsk();
};