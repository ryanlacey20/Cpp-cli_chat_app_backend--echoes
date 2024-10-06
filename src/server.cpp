#include <iostream>
#include "crow_all.h"
#include "dbConnection.h"

int startServer()
{
    // Create a Crow application
    crow::SimpleApp app;

    // Define a simple GET endpoint
    CROW_ROUTE(app, "/getMessages")
    ([](const crow::request &req)
     {

        std::cout << "hello"; 
        return "Request complete, logged to console"; });

    app.port(18080).multithreaded().run();

    return 0;
}
