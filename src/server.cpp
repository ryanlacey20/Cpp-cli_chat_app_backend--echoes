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

bool
 putFirebaseData(const std::string& url, const json& data){
    CURL* curl = curl_easy_init();
    if (!curl) return false; 

    std::string stringData = data.dump(); 

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stringData.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr); 

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl); 

    return res == CURLE_OK; 
}

int startServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/getMessages")([&]() {
        return crow::response(getFirebaseData("https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages/0.json").dump());
    });

    CROW_ROUTE(app, "/putMessage")([&]() {

            const json data = {
                {"1", {   
                    {"user", "Commdus"},
                    {"message", "MEET ME AFTER THE TOGA PARTY."},
                    {"timestamp", "2024-10-22T12:34:56Z"}
                    }
                },
                {"2", {
                    {"user", "Maximus"},
                    {"message", "ARE YOU NOT ENTERTAINED?"},
                    {"timestamp", "2024-10-22T12:35:00Z"}
                    }
                }
        };

        //  ---------- MSG JSON DATA STRUCTURE --------------
   
        // {
        //         {"1", {   
        //             {"user", "Russel"},
        //             {"message", "MEET ME APRES TOGA PARTY!"},
        //             {"timestamp", "2024-10-22T12:34:56Z"}
        //             }
        //         },
        //         {"2", {
        //             {"user", "Crow"},
        //             {"message", "ARE YOU NOT ENTERTAINED?"},
        //             {"timestamp", "2024-10-22T12:35:00Z"}
        //             }
        //         }
        // };


        
        putFirebaseData("https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages.json", data);
        return crow::response("ran, check DB");

    });

    app.port(18080).multithreaded().run();
    return 0;
}
