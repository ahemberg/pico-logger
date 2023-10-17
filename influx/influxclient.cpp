#include "influxclient.hpp"

#define TLS_CLIENT_TIMEOUT_SECS  30


bool post_to_influx(std::string server_address, uint16_t port, std::string database ,std::string username, std::string password, std::string payload) {

    std::ostringstream os;

    os << "POST /write?db=" << database << "&precision=s&u=" << username << "&p=" << password <<  " HTTP/1.1\r\n";
    os << "Host: "<< server_address << ":" << port << "\r\n";
    os << "User-Agent: curl/8.1.2\r\n";
    os << "Accept: */*\r\n";
    os << "Content-Length: "<< payload.size()-1 <<"\r\n";
    os << "Content-Type: application/x-www-form-urlencoded\r\n";
    os << "\r\n";
    os << payload;
    os << "\r\n";

    std::cout << "Posting: " << std::endl;
    std::cout << os.str() << std::endl;

    std::string server_response = send_tls_request(server_address, os.str(), port, TLS_CLIENT_TIMEOUT_SECS);

    std::cout << "Got response" << std::endl;
    std::cout << server_response << std::endl;

    //Todo parse the response. Check for 204, if not fail the request for now.
    //Future work: if partial save was achieved, make sure to only retain messages not saved
    return true;
}