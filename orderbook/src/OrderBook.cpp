#include "Order.h"
#include "OrderBook.h"
#include "MatchingEngine.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC METHODS                              ////
////////////////////////////////////////////////////////////////////////////////

OrderBook::OrderBook()
{
    bidOrders = {};
    askOrders = {};
    OrderIdInfoStore = {};
    tradeHistory = {};
    size = 0;
}

/*
    Adds an order to its respective book, calls match, and completes potential
    trades.
*/
ReturnCode OrderBook::addOrderToBook(Order order)
{
    order.setIdOfOrder(size);
    size++;

    // Add to orders lists
    if (order.getSideOfOrder() == Side::BID) {
        bidOrders[order.getPriceOfOrder()].push_back(order);
    } else {
        askOrders[order.getPriceOfOrder()].push_back(order);
    }

    // Add to OrderIdInfoStore
    OrderIdInfoStore[order.getIdOfOrder()] = {order.getSideOfOrder(), order.getPriceOfOrder()};

    matchOrders(*this);

    return ReturnCode::SUCCESS;
}

/*
    Prints current state of the order book
*/
void OrderBook::printOrderBookState() const
{
    std::cout << "\n=== ORDER BOOK ===\n";

    std::cout << "ASKS\n";
    for (auto it = askOrders.rbegin(); it != askOrders.rend(); ++it) {
        int totalQuantity = 0;
        for (const Order& o : it->second) totalQuantity += o.getQuantityOfOrder();
        std::cout << "  $" << it->first << "  x" << totalQuantity << "\n";
    }

    std::cout << "---\n";

    for (auto it = bidOrders.begin(); it != bidOrders.end(); ++it) {
        int totalQuantity = 0;
        for (const Order& o : it->second) totalQuantity += o.getQuantityOfOrder();
        std::cout << "  $" << it->first << "  x" << totalQuantity << "\n";
    }

    std::cout << "BIDS\n";
    std::cout << "==================\n\n";
}

/*
    Finds an order and deletes it if fulfilled / needs to be cancelled
*/
void OrderBook::cancelOrder(uint64_t id) 
{
    const Order* order = findOrderById(id);
    long price = order->getPriceOfOrder();
    Side side = order->getSideOfOrder();

    std::list<Order>& list = (order->getSideOfOrder() == Side::BID)
    ? bidOrders[price]
    : askOrders[price];

    // Remove order from list
    list.remove(*order);

    // Delete the price list from the order book if its empty
    if (list.empty() && side == Side::BID) bidOrders.erase(price);
    else if (list.empty() && side == Side::ASK) askOrders.erase(price);

    // Delete the id from the OrderIdInfoStore
    OrderIdInfoStore.erase(id);

    return;
}

/*
    Simple modification of pre-existing order. Deletes and readds
*/
void OrderBook::modifyOrder(uint64_t id, long newPrice, int newQuantity)
{
    const Order* order = findOrderById(id);
    if (!order) return;

    Side side = order->getSideOfOrder();

    cancelOrder(id);

    uint64_t newTimestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    Order newOrder(side, newPrice, newQuantity, newTimestamp);
    addOrderToBook(newOrder);
}

void OrderBook::addTradeToHistory(Trade t) 
{
    tradeHistory.push_back(t);
    return;
}

uint64_t OrderBook::getSize() const
{
    return size;
}

/*
    Returns true if bid book empty, false otherwise
*/
bool OrderBook::bidEmpty() const
{
    return bidOrders.empty();
}

/*
    Returns true if ask book empty, false otherwise
*/
bool OrderBook::askEmpty() const
{
    return askOrders.empty();
}

/*
    Returns number of bid orders
*/
int OrderBook::getAmtOfBidOrders() const
{
    return bidOrders.size();
}

/*
    Returns number of ask orders
*/
int OrderBook::getAmtOfAskOrders() const
{
    return askOrders.size();
}

/*
    Returns highest bidding price of the stock
*/
long OrderBook::getHighestBid() const
{
    if (bidEmpty()) return 0;
    else return bidOrders.begin()->first;
}

/*
    Returns lowest asking price of the stock
*/
long OrderBook::getLowestAsk() const
{
    if (askEmpty()) return 0;
    else return askOrders.begin()->first;
}

/*
    Using id, returns OrderIdInfo struct for efficient Order instance find 
    in OrderBook. Returns nullptr for invalid id's.
*/
const OrderIdInfo* OrderBook::getOrderInfoViaId(uint64_t id) const
{
    auto info = OrderIdInfoStore.find(id);
    if (info == OrderIdInfoStore.end()) return nullptr;
    else return &(info->second);
}

/*
    Uses id (valid or invalid), to return corresponding pointer to Order in
    either bid or ask books. Returns nullptr for invalid orders.
*/
const Order* OrderBook::findOrderById(uint64_t id) const
{
    const OrderIdInfo* info = getOrderInfoViaId(id);
    if (!info) return nullptr;

    Side orderSide = info->side;
    long orderPrice = info->price;

    // Find correct list of orders for price (in either buy or sell)
    const std::list<Order>& orders = (orderSide == Side::BID) 
    ? bidOrders.find(orderPrice)->second 
    : askOrders.find(orderPrice)->second;

    // Find specific order in the list
    for (const Order& item : orders) {
        if (item.getIdOfOrder() == id) return &item;
    }

    return nullptr;
}

/*
    Returns a pointer to highest bid in the list
*/
Order* OrderBook::getFrontBid() 
{
    if (bidEmpty()) return nullptr;
    else return &(bidOrders.begin()->second.front());
}

/*
    Returns a pointer to the lowest ask in the list
*/
Order* OrderBook::getFrontAsk()
{
    if (askEmpty()) return nullptr;
    else return &(askOrders.begin()->second.front());
}
