#include "Order.h"

////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC METHODS                              ////
////////////////////////////////////////////////////////////////////////////////

bool Order::operator==(const Order& other) const {
    return id == other.id;
}

Order::Order(Side side, long price, int quantity, uint64_t timeStamp)
{
    this->side = side;
    this->price = price;
    this->quantity = quantity;
    this->timeStamp = timeStamp;

    // Initialised via addToOrder method in OrderBook 
    this->id = UNINITIALISED;
}

uint64_t Order::getTimestampOfOrder() const
{
    return timeStamp;
}

uint64_t Order::getIdOfOrder() const
{
    return id;
}

long Order::getPriceOfOrder() const
{
    return price;
}

Side Order::getSideOfOrder() const
{
    return side;
}

int Order::getQuantityOfOrder() const
{
    return quantity;
}

void Order::setIdOfOrder(uint64_t id)
{
    this->id = id;
}

void Order::setQuantityOfOrder(int quantity)
{  
    this->quantity = quantity;
}




