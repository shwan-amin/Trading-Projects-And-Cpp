#pragma once
#include <cstdint>

#define UNINITIALISED 0

enum class Side { 
    BID, 
    ASK 
};

class Order {
private:
    uint64_t id;
    Side side;
    long price;
    int quantity;
    uint64_t timeStamp;
    
public:
    Order(Side side, long price, int quantity, uint64_t timeStamp);
    bool operator==(const Order& other) const;

    uint64_t getTimestampOfOrder() const;
    uint64_t getIdOfOrder() const;
    long getPriceOfOrder() const;
    Side getSideOfOrder() const;
    int getQuantityOfOrder() const;

    void setIdOfOrder(uint64_t id);
    void setQuantityOfOrder(int quantity);
};