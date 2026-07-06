#include "MatchingEngine.h"
#include <iostream>

/*
    Given the current state of the book, find and execute matches
*/
void matchOrders(OrderBook& orderBook) 
{
    if (orderBook.bidEmpty() || orderBook.askEmpty()) return;

    // If bid is too low for asks, return
    if (orderBook.getHighestBid() < orderBook.getLowestAsk()) return;

    // While the highest bid is greater than the lowest ask, complete trades
    while (!orderBook.bidEmpty() && !orderBook.askEmpty() && 
    orderBook.getHighestBid() >= orderBook.getLowestAsk()) {
        Order* frontBid = orderBook.getFrontBid();
        Order* frontAsk = orderBook.getFrontAsk();

        int frontBidQ = frontBid->getQuantityOfOrder();
        int frontAskQ = frontAsk->getQuantityOfOrder();

        int tradeQuantity = std::min<int>(frontBidQ, frontAskQ);
        long tradePrice = frontBid->getPriceOfOrder();

        // Create a trade object
        Trade t{
            .bidOrderId = frontBid->getIdOfOrder(), 
            .askOrderId = frontAsk->getIdOfOrder(),
            .priceTraded = tradePrice,
            .quantityTraded = tradeQuantity,
            .timeStampOfTrade = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>
                                (std::chrono::system_clock::now().time_since_epoch()).count())
        };

        orderBook.addTradeToHistory(t);

        // Reduce quantities on both orders, or cancel if empty
        if (frontBidQ - tradeQuantity != 0) {
            frontBid->setQuantityOfOrder(frontBidQ - tradeQuantity);
        } else {
            orderBook.cancelOrder(frontBid->getIdOfOrder());
        }

        if (frontAskQ - tradeQuantity != 0) {
            frontAsk->setQuantityOfOrder(frontAskQ - tradeQuantity);
        } else {
            orderBook.cancelOrder(frontAsk->getIdOfOrder());
        }

        // Print a success message
        std::cout << "Trade executed: " << tradeQuantity << " @ " << tradePrice << "\n";
    }

    // Print the order book after matching
    orderBook.printOrderBookState();

    return;
}
