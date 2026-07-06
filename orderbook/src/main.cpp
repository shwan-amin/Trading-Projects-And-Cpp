#include <stdlib.h>
#include <iostream>
#include <Order.h>
#include <OrderBook.h>
#include <sstream>
#include <chrono>

int main()
{
    OrderBook book;
    std::string line;

    std::cout << "Order Book Terminal\n";
    std::cout << "Commands: buy <qty> <price> | sell <qty> <price> | cancel <id> | modify <id> <price> <qty> | print | quit\n\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        std::istringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "quit") {
            break;

        } else if (command == "buy" || command == "sell") {
            int quantity;
            long price;
            if (!(ss >> quantity >> price)) {
                std::cout << "Usage: buy <qty> <price>\n";
                continue;
            }
            uint64_t timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            Side side = (command == "buy") ? Side::BID : Side::ASK;
            Order order(side, price, quantity, timestamp);
            book.addOrderToBook(order);

        } else if (command == "cancel") {
            uint64_t id;
            if (!(ss >> id)) {
                std::cout << "Usage: cancel <id>\n";
                continue;
            }
            book.cancelOrder(id);

        } else if (command == "modify") {
            uint64_t id;
            long newPrice;
            int newQuantity;
            if (!(ss >> id >> newPrice >> newQuantity)) {
                std::cout << "Usage: modify <id> <price> <qty>\n";
                continue;
            }
            book.modifyOrder(id, newPrice, newQuantity);

        } else if (command == "print") {
            book.printOrderBookState();

        } else {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}