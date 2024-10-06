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
        Database db;
        
        return  db.executeQuery("SELECT * FROM messages ORDER BY created_at"); });

    app.port(18080).multithreaded().run();

    return 0;
}
