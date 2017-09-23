# GDAX C++ REST Library (unofficial)

This library was implimented in C++ on Ubuntu 14.

## Getting Started

How to build a sample program
 g++ -o gdax_rest_program  main.cpp client_gdax.cpp -std=c++11 -lcurl -lcrypto -lssl -lcryptopp -lboost_chrono -lboost_date_time

To run program:
./gdax_rest_program




main.cpp

``` c++

#include "client_gdax.h"

int main(int argc, const char *argv[])
{	
	std::string key("Insert your key here");
	std::string secret("Insert your secret here");
	std::string pass_phrase("Insert your passphrase here");

	std::cout<<std::endl<<"Start GDAX rest sample program"<<std::endl<<std::endl;
	std::cout<<"To place then cancel a live buy order at $100 per BTC set variable do_place_order = true"<<std::endl<<std::endl;
	
	bool do_place_order = false;
	
	client_gdax client(key,secret,pass_phrase);
	
	mkt_symbol btc("BTC","USD","gdax");
	order order1;
	std::string response;
	
	
	std::cout<<"response from get available balances request:"<<std::endl<<std::endl<<client.returnAvailableAccountBalances()<<std::endl<<std::endl;
	std::cout<<"response my trade history:"<<std::endl<<std::endl<<client.returnTradeHistory(btc)<<std::endl<<std::endl;
	std::cout<<"response from get orderbook snapshot request:"<<std::endl<<std::endl<<client.get_book_info(btc,1)<<std::endl<<std::endl;
	
	if(do_place_order){
		bool isbuy = true;
		double quant = .01;
		double price = 100.0;
		
		int status_from_place_order = client.place_order(btc,isbuy, price,quant,order1,response);
		std::cout<<"response from place order:"<<std::endl<<std::endl<<response<<std::endl<<std::endl;
		
		if(status_from_place_order==0){
			std::cout<<"response from order_status request:"<<std::endl<<std::endl<<client.returnOrderTrades(order1.ordernum)<<std::endl<<std::endl;
			std::cout<<"response from cancel request:"<<std::endl<<std::endl<<client.cancelOrder(order1.ordernum)<<std::endl<<std::endl;
		}
	}
	
		
	
	return 0;
}

```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

