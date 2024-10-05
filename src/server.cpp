#include "crow_all.h"

int server()
{
    // Create a Crow application
    crow::SimpleApp app;

    // Define a simple GET endpoint
    CROW_ROUTE(app, "/")
    ([]()
     { return "Hello, World! This is a Crow endpoint."; });

    // Define another GET endpoint with a parameter
    CROW_ROUTE(app, "/hello/<string>")
    ([](const std::string &name)
     { return "Hello, " + name + "!"; });

    // Start the application on port 18080
    app.port(18080).multithreaded().run();

    return 0;
}