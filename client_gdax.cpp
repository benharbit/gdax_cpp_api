#include "client_gdax.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include "boost/date_time/posix_time/posix_time.hpp" 
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>


#include "/usr/include/boost/variant.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <cryptopp/hmac.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <stdio.h>
#include <stdlib.h>


// Curl write callback function. Appends fetched *content to *userp pointer.
// *userp pointer is set up by curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result) line.
// In this case *userp will point to result.
size_t client_gdax::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
    
}





 std::string client_gdax::returnOpenOrders(std::string str1){
    std::string buff;
    std::string path = "/fills?";
    if(str1==""){
          //all
        path += "limit=3&";//&product_id=all";
    }
    else{ 
        path += "after=";
        path += str1;
        path += "&";
        path += "limit=3&";
    }
    std::string err_str;
    Dorequest("https://api.gdax.com", buff, err_str,"GET",path );
    return err_str;
 }
 




 std::string client_gdax::returnAvailableAccountBalances(){
 	std::string buff;
 	std::string path = "/position";
	std::string response_str;
 	Dorequest("https://api.gdax.com", buff, response_str,"GET",path );
 	return response_str;
 }


std::string client_gdax::returnTradeHistory(mkt_symbol currencyPair){
    std::string sym1 = make_gdax_sym(currencyPair);
    std::string buff;
    std::string path = "/fills?";
    std::string before("");
   
    if(trade_hist_pagination.count(currencyPair)>0){
        before = trade_hist_pagination[currencyPair]; 
    }

    if(""==before){
          //all
        path += "limit=3&";//&product_id=all";
        path += "product_id=";
        path += sym1;
    }
    else{ 
        path += "before=";
        path += before;
        path += "&";
        path += "limit=30&";
                path += "product_id=";
        path += sym1;
    }
    std::string err_str;
    Dorequest("https://api.gdax.com", buff, err_str,"GET",path );
    return err_str;

}






static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";






std::string HexToBytes(const std::string& hex) {
  std::vector<char> bytes;

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    std::string byteString = hex.substr(i, 2);
    char byte = (char) strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }
  

  std::string xxx;
  for(int i = 0;i<bytes.size();++i)
    xxx +=bytes[i]; 

  return xxx;
}




int client_gdax::do_public_request(const std::string &UrlEndPoint, std::string &result,const std::string& request_path )
{
    
    if(curl) {
       
        std::string  url = APIurl + UrlEndPoint;
        std::string now1 =  gettime();
       
        // Headers
        struct curl_slist *httpHeaders = NULL;
        httpHeaders = curl_slist_append(httpHeaders, ("CB-ACCESS-KEY: " + key).c_str());
        httpHeaders = curl_slist_append(httpHeaders,("CB-ACCESS-TIMESTAMP: " + now1).c_str());
        httpHeaders = curl_slist_append(httpHeaders, ("CB-ACCESS-PASSPHRASE: " + passphrase).c_str());
        httpHeaders = curl_slist_append(httpHeaders, "User-Agent:api");
       
        
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); // debug option
        std::string run22  =  url.c_str();
        run22 += request_path;
        
        curl_easy_setopt(curl, CURLOPT_URL, run22.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, httpHeaders);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        res = curl_easy_perform(curl);
        curl_slist_free_all(httpHeaders);

        if (res != CURLE_OK)
        {
            return res;
        }
        return res;
        
    }
    else
    {
        // curl not properly initialized curl = NULL
        return -11;//curlERR;
        
    }
}

std::string client_gdax::cancelOrder(std::string ordernum ){


std::string buff;
 std::string path = "/orders/"+ordernum;
 std::string err_str;
Dorequest("https://api.gdax.com", buff, err_str,"DELETE",path );

return err_str;
}




int client_gdax::Dorequest(const std::string &UrlEndPoint, const std::string &body, std::string &result,const std::string& method,const std::string& request_path )
{
    
    if(curl) {
       
        std::string  url = APIurl + UrlEndPoint;
        std::string  payload;
        std::string  signature;
        std::string secretKey_decoded;
        std::string signature_encoded;
        std::string now1 =  gettime();

        std::string messagea_64;

        payload = now1 + method + request_path + body;
      
        DecodeBase64(secret, secretKey_decoded);
        getHmacSha256(secretKey_decoded, payload, signature);
        std::string xxx = HexToBytes(signature);
    	getBase64(xxx,messagea_64);
    	
    


  
        
        // Headers
        struct curl_slist *httpHeaders = NULL;
        httpHeaders = curl_slist_append(httpHeaders, ("CB-ACCESS-KEY: " + key).c_str());
        httpHeaders = curl_slist_append(httpHeaders,("CB-ACCESS-TIMESTAMP: " + now1).c_str());
        httpHeaders = curl_slist_append(httpHeaders, ("CB-ACCESS-PASSPHRASE: " + passphrase).c_str());
        httpHeaders = curl_slist_append(httpHeaders, "User-Agent:api");
        httpHeaders = curl_slist_append(httpHeaders, ("CB-ACCESS-SIGN: " + messagea_64).c_str());
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // debug option
		
		std::string run22  =  url.c_str();
        run22 += request_path;
		if(method=="DELETE"){
        	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            curl_easy_setopt(curl, CURLOPT_POST, 0);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 0);
        }
        
        


        curl_easy_setopt(curl, CURLOPT_URL, run22.c_str());
        if(method=="POST"){
        	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
        	curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 0);
        	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        	httpHeaders = curl_slist_append(httpHeaders, "Content-Type: Application/JSON");

        }
        else if(method=="GET"){
        	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
        	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
             curl_easy_setopt(curl, CURLOPT_POST, 0);
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, httpHeaders);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        res = curl_easy_perform(curl);
        curl_slist_free_all(httpHeaders);

        if (res != CURLE_OK)
        {
           
            return res;
        }
        return res;
        
    }
    else
    {
        // curl not properly initialized curl = NULL
        return -11;//curlERR;
        
    }
}


const std::vector<uint8_t> client_gdax::sha256(const std::vector<uint8_t>& key, const std::vector<uint8_t>& value)
{
    unsigned int len = SHA256_DIGEST_LENGTH;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    size_t key_len = key.size();
    size_t value_len = value.size();

    HMAC_CTX hmac;
    HMAC_CTX_init(&hmac);
    HMAC_Init_ex(&hmac, (unsigned char*) key.data(), key_len, EVP_sha256(), NULL);
    HMAC_Update(&hmac, (unsigned char*) value.data(), value_len);
    HMAC_Final(&hmac, hash, &len);
    HMAC_CTX_cleanup(&hmac);

    return std::vector<uint8_t>((uint8_t*) hash, (uint8_t*) hash+SHA256_DIGEST_LENGTH);
}

	
	//fix_data
	

	client_gdax::client_gdax(){
		curl = curl_easy_init();
	}



	client_gdax::~client_gdax(){
		curl_easy_cleanup(curl);
	}
	client_gdax::client_gdax(std::string key,std::string secret,std::string passphrase ):
		key(key),secret(secret),passphrase(passphrase){
		
	    curl = curl_easy_init();
	    

	}
	

std::string client_gdax::make_gdax_sym(mkt_symbol m){
	std::string sym(m.currency)	;
	sym +="-";
	sym += m.base;
	return sym;
}



   std::string client_gdax::returnOrderTrades(std::string ordernum ){
   	std::string buff;
 	std::string path = "/orders/"+ordernum;
	std::string err_str;
	Dorequest("https://api.gdax.com", buff, err_str,"GET",path );
	
	return err_str;
   }



  std::string client_gdax::get_book_info(mkt_symbol symbol,int level){

	std::string result;
	std::string sym = make_gdax_sym(symbol);
	std::string req("/products/");
	req += sym;
	req +=  "/book?level=";
	if(level<0 || level>3){
		std::cout<<"orderbook level must between 1 and 3"<<std::endl;
	}
	char buff[20];
	snprintf(buff,20,"%d",level);
	req += buff;



	int ans = do_public_request("https://api.gdax.com", result,req);
	
	return result;
}






   int client_gdax::place_order(mkt_symbol symbol,bool isbuy,double price,double quant,order& o1,std::string& err_str,std::string type_order,std::string input_string ){
	std::string sym = make_gdax_sym(symbol);
	std::string side;
	
	if(isbuy)
		side = "buy";
	else
		side = "sell";

	char buff[300];



    if(type_order=="exchange market")
        snprintf(buff,300,"{\"size\":\"%.2f\",\"price\":\"%.8f\",\"side\":\"%s\",\"product_id\": \"%s\"}",quant,price,side.c_str(),sym.c_str());
    else
	   snprintf(buff,300,"{\"size\":\"%.2f\",\"price\":\"%.8f\",\"side\":\"%s\",\"product_id\": \"%s\",\"post_only\": \"true\"}",quant,price,side.c_str(),sym.c_str());
	
	
	if(input_string=="-99")
		Dorequest("https://api.gdax.com", buff, err_str,"POST","/orders" );
	else
		err_str = input_string;
	
	if(err_str.find("\"id\":\"")==std::string::npos)
		return 1;

    if(err_str.find("\"rejected\"")!=std::string::npos)
        return 1;

	 std::stringstream ss;
     ss<<err_str;
	 boost::property_tree::ptree info;
     boost::property_tree::read_json(ss,info);
     double amount = 0;
     double temp_amount = 0 ;

     o1.trade_amt = 0;
     o1.avg_trade_price = 0;

     o1.ordernum = info.get<std::string>("id").c_str();
    

     o1.price = atof(info.get<std::string>("price").c_str());
     o1.m1 = symbol;
     o1.quant =  atof(info.get<std::string>("size").c_str());
     o1.isbuy  =isbuy;


     std::string io_time = info.get<std::string>("created_at").c_str();

     double fillsz = atof(info.get<std::string>("filled_size").c_str());

     double avg_price ;
     if(fillsz>10e-10){
     	o1.trade_amt = fillsz;
     	o1.avg_trade_price = atof(info.get<std::string>("executed_value").c_str())/fillsz;
     	o1.fee = atof(info.get<std::string>("fill_fees").c_str());
     }
     else{
     	o1.trade_amt = 0;
     	o1.avg_trade_price  = 0;
     	o1.fee = 0;
     }

     
 	
	std::size_t found = io_time.find("T");
	if(found != std::string::npos){
		io_time[found] = ' ';
		io_time.erase(io_time.end()-1);
		boost::posix_time::ptime t(boost::posix_time::time_from_string(io_time));
		time_t time1 = boost::posix_time::to_time_t(t);
     	o1.server_time = time1;
	}
     
    
     o1.time1 = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
     return 0;


}



	std::string client_gdax::gettime (){

	
	 timeval curTime;
	 gettimeofday(&curTime, NULL);
	 int milli = curTime.tv_usec / 1000;
	 char buffer [80];
	 strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S", gmtime(&curTime.tv_sec));
	 char time_buffer2[200];
	 snprintf(time_buffer2, 200, "%s.%d000Z", buffer,milli);
	 snprintf(time_buffer2, 200, "%sZ", buffer);

	 //return time_buffer2;
	 
	time_t result = time(NULL);
	long long result2 = result;
	char buff4[40];
	snprintf(buff4,40,"%lld",result2);

	char buff5[40];
	snprintf(buff5,40,"%lld.%d000",result2,milli);
	return buff5;
		
	}




	
	int client_gdax::getBase64(const std::string &content, std::string &encoded)
{
    
    using CryptoPP::StringSource;
    using CryptoPP::Base64Encoder;
    using CryptoPP::StringSink;
    
    byte buffer[1024] = {};
    
    for (int i = 0; i < content.length(); i++)
    {
        buffer[i] = content[i];
    };
    
   StringSource ss(buffer, content.length(), true, new Base64Encoder( new StringSink(encoded), false));
    //StringSource ss(encoded, true, new Base64Decoder(new HexEncoder(new StringSink(encoded))));
    
    return 0;
    
}



int client_gdax::getHmacSha256(const std::string &key, const std::string &content, std::string &digest)
{
    
    using CryptoPP::SecByteBlock;
    using CryptoPP::StringSource;
    using CryptoPP::HexEncoder;
    using CryptoPP::StringSink;
    using CryptoPP::HMAC;
    using CryptoPP::HashFilter;
    using std::transform;
    
    SecByteBlock byteKey((const byte*)key.data(), key.size());
    std::string  mac;
    digest.clear();
    
    HMAC<CryptoPP::SHA256> hmac(byteKey, byteKey.size());
    StringSource ss1(content, true, new HashFilter(hmac, new StringSink(mac)));
    StringSource ss2(mac, true, new HexEncoder(new StringSink(digest)));
    transform(digest.begin(), digest.end(), digest.begin(), ::tolower);
    
    return 0;
    
}

int client_gdax::DecodeBase64(const std::string &encoded, std::string &decoded){
    using CryptoPP::StringSource;
    using CryptoPP::Base64Decoder;
    using CryptoPP::StringSink;

	//std::string decoded;
   
	StringSource ss(encoded, true,
    new Base64Decoder(
        new StringSink(decoded)
    ) // Base64Decoder
); // StringSource

	}

	



