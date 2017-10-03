
#ifndef CLIENT_GDAX_H

#define CLIENT_GDAX_H

#include <vector>
#include <string>



#include <boost/version.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <fstream>
#include <thread>
#include "/usr/include/boost/variant.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>


#include <curl/curl.h>

#include <cryptopp/hmac.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>


  struct mkt_symbol{
	mkt_symbol(const std::string& currency,const std::string&  base,const std::string& exchange):
	currency(currency),
	base(base),
	exchange(exchange){
		
	};
	mkt_symbol(){
		
	};

	std::string currency;
	std::string base;
	std::string exchange;
	
	bool operator<(const mkt_symbol& rhs) const{
		//int val1 = str_to_compare.compare(rhs.str_to_compare);
		//if (val1<0)
			return true;
		//else
		//	return false;
	}
	
	
};




struct order{
	mkt_symbol m1;
	double price;
	double quant;
	bool isbuy;
	std::string ordernum;
	double avg_trade_price;
	double last_entry;
	std::string extra1;
	bool suspect_traded;
	long long time1;
	long long time_placed;
	double fee;
	int stratnum;
	double trade_amt;
	double server_time;
	
};



struct f_data{
        int n;
        std::string message;
        f_data(int n1,std::string message1){
            n = n1;
            message = message1;
        };

    };




class client_gdax{
	public:
		std::string APIurl;
		std::string key;
		std::string secret;
       		std::string passphrase;
        	CURL *curl;
        	CURLcode res;
		
		client_gdax(std::string key,std::string secret,std::string passphrase);
		client_gdax();
		~client_gdax();
		
		std::string sign_data(const char *data, std::size_t data_size,std::string my_key) ;
		std::string returnTradeHistory(mkt_symbol currencyPair);
		std::map<mkt_symbol,std::string> trade_hist_pagination;

		std::string returnAvailableAccountBalances();
		int returnOrderTrades_vec(order& o1,std::string& err_str ,std::string input_str = "-99") ;
		std::string move(order& o1, double price,double amount = 0);
		int move_vec(order& o1, order& new_order,double price,std::string& err_str,double amount = -99 ,std::string input_str="-99" );

      
		int getBase64(const std::string &content, std::string &encoded);
		std::string returnOpenOrders(std::string str1);
		std::string fills_last_id;  
		int Dorequest(const std::string &UrlEndPoint, const std::string &body, std::string &result,const std::string& method,const std::string& request_path );


		std::string make_gdax_sym(mkt_symbol m);
		int DecodeBase64(const std::string &encoded, std::string &decoded);
		int getHmacSha256(const std::string &key, const std::string &content, std::string &digest);
		std::string cancelOrder(std::string ordernum );
		bool use_REST;
		std::string GenerateCheckSum( const char *buf, long bufLen );
		std::string gettime();
		static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
		std::string make_fix(std::vector<f_data>  x );
		const std::vector<uint8_t> sha256(const std::vector<uint8_t>& key, const std::vector<uint8_t>& value);

		std::string returnOrderTrades(std::string ordernum ) ;
    

    
		int place_order(mkt_symbol symbol,bool isbuy,double price,double quant,order& o1,std::string& errstr,std::string type_order="exchange limit",std::string input_string = "-99");
		std::string get_book_info(mkt_symbol symbol,int level);
		int do_public_request(const std::string &UrlEndPoint, std::string &result,const std::string& request_path );


    
};

#endif
