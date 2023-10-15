#include "influxclient.hpp"

#define TLS_CLIENT_TIMEOUT_SECS  30

bool post_to_influx(std::string server_address, std::string database ,std::string username, std::string password, std::string payload) {

    // std::ostringstream os;

    // for (const auto& datapoint: datapoints) {
    //     os << datapoint.to_line() << ",";
    // }
    // os.seekp(-1, os.cur);

    // os << " " << timestamp;

    std::ostringstream os;

    os << "POST /write?db=" << database << "&precision=s&u=" << username << "p=" << password << "HTTP/1.1\r\n";
    os << "Host: " << server_address << "\r\n";
    os << "Content-Type: application/x-www-form-urlencoded\r\n"; //TODO not sure if needed
    os << "Content-Length: " << payload.size() << "\r\n";
    os << payload << "\r\n\r\n";

    std::cout << "Posting: " << std::endl;
    std::cout << os.str() << std::endl;

    const char *post_data = os.str().c_str();
    const char *server_cstr = server_address.c_str();

    bool pass = run_tls_client_test(server_cstr, post_data, 30);

    return pass;
}