This is a simple single-threaded order book on a single stock.
- Implements a matching algorithm to match highest bids to lowest asks and fulfill the trade
- Records a history of all trades

To compile and run the program from main.cpp, run these commands in the terminal (be cd'd in the project root):
cd build
cmake ..
cmake --build .
./order_book

Then the commands are:
buy <quantity> <price>
sell <quantity> <price>
cancel <id>
modify <id> <newPrice> <newQuantity>
print
quit

