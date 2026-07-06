#pragma once
#include <cstdint>

struct Trade {
    uint64_t bidOrderId;
    uint64_t askOrderId;
    long priceTraded;
    int quantityTraded;
    uint64_t timeStampOfTrade;
};