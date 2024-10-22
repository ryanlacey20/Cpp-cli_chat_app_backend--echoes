#include <iostream>
#include <curl/curl.h>
#include "crow_all.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static size_t MyWriteCallback(void* data, size_t elementSize, size_t elementCount, std::string* buffer) {
    buffer->append((char*)data, elementSize * elementCount);
    return elementSize * elementCount;
}


json getTodaysMessage() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages.json");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return json::parse(readBuffer);
}

int startServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/getMessages")([&]() {
        return crow::response(getTodaysMessage().dump());
    });

    app.port(18080).multithreaded().run();
    return 0;

}

// curl -X PUT -d '{ “m1": { "user": "Jack", "message": "hello world" }, “m2": { "user": "Sparrow", "message": "how are you" } }' 'https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages.json'