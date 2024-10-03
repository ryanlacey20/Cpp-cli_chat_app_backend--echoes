#include <iostream>
#include "crow_all.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void queryEndpoint(const std::string &host, const std::string &target)
{
    try
    {
        net::io_context ioc;

        tcp::resolver resolver(ioc);
        tcp::socket socket(ioc);

        auto const results = resolver.resolve(host, "http");

        // Connect to server
        net::connect(socket, results.begin(), results.end());

        // Create request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send request
        http::write(socket, req);

        // receive response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(socket, buffer, res);

        // Print the response
        std::cout << res << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main()
{

    crow::SimpleApp app;

    // define your endpoint at the root directory
    CROW_ROUTE(app, "/")
    ([]()
     { return "Hello world"; });

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
}