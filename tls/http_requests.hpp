//Sample response
// HTTP/1.1 204 No Content
// Content-Type: application/json
// Request-Id: 30789873-6d24-11ee-aa22-000000000000
// X-Influxdb-Build: OSS
// X-Influxdb-Version: 1.6.7~rc0
// X-Request-Id: 30789873-6d24-11ee-aa22-000000000000
// Date: Tue, 17 Oct 2023 19:34:29 GMT

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>

class HTTPRequest {
    public:
        static HTTPResponse post(std::string url, std::string data, std::map<std::string, std::string> headers);
    private:
};

class HTTPResponse {
    public:
        uint16_t code;
        std::string reason;
        std::map<std::string, std::string> headers; //Will we ever use headers? Probably skip this in order to save memory. Can include in some other library
    private:
};