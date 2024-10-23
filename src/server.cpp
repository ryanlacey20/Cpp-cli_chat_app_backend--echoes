#include <iostream>
#include <curl/curl.h>
#include "crow_all.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static size_t WriteCallback(void* data, size_t size, size_t nmemb, std::string* buffer) {
    buffer->append(static_cast<char*>(data), size * nmemb);
    return size * nmemb;
}

json getFirebaseData(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) return json(); 

    std::string readBuffer;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return json::parse(readBuffer); 

}

int startServer(const std::string& url) {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/getMessages")([&]() {
        return crow::response(getFirebaseData("https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages.json").dump());
    });

    app.port(18080).multithreaded().run();
    return 0;
}
