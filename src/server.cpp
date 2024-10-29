#include <iostream>
#include <curl/curl.h>
#include "crow_all.h"
#include <nlohmann/json.hpp>
#include <chrono>



using json = nlohmann::json;

const std::string generalMessagesDBEndpointURL = "https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages/";

std::string getCurrentDate() {
    // get curr time
    auto now = std::chrono::system_clock::now();
    
    // use time_t for calendar time
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    // tm struct
    std::tm now_tm = *std::localtime(&now_c);
    
    // format YYYYMMDD
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y%m%d");
    
    return oss.str();
}
std::string parseTodaysDBURL(){
    std::ostringstream urlOss;
    urlOss << "https://echoes-86d80-default-rtdb.europe-west1.firebasedatabase.app/messages/" << getCurrentDate() << ".json";
    return urlOss.str();
}

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

bool putFirebaseData(const std::string& url, const json& data){
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
void debugResetFirbaseDB(json data){
    putFirebaseData(generalMessagesDBEndpointURL, data);
}
int startServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/testDev")([&]() {
        std::cout << __cplusplus << std::endl;
        return crow::response(getCurrentDate());
    });

    CROW_ROUTE(app, "/getTodaysMessages")([&]() {
        std::string todaysMessagesDBEndpointURL = parseTodaysDBURL();
        std::optional<std::string> todaysMessages = getFirebaseData(todaysMessagesDBEndpointURL).dump()
        if(getFirebaseData(todaysMessagesDBEndpointURL).dump())
        
        return crow::response("placeholder");

    });

// left off here, succesfully reads data from body of request
// TODO: get MD of messages to find what the Id of this message will be or else find a way to auto index the data added to the DB, the latter would be preferable

    CROW_ROUTE(app, "/putMessage").methods("PUT"_method)([](const crow::request& req) {
        auto jsonData = json::parse(req.body);

//  -----DEV WORK-----
//  ---------- MSG JSON DATA STRUCTURE --------------
        //     const json data = {
        //         {"20241023", 
        //             {
        //                 {"md", {
        //                     {"length", 2}
        //                     }
        //                 },
        //                 {"m1", {   
        //                     {"user", "Commodus"},
        //                     {"message", "MEET ME AFTER THE TOGA PARTY."},
        //                     {"timestamp", "2024-10-22T12:34:56Z"}
        //                     }
        //                 },
        //                 {"m2", {
        //                     {"user", "Maximus"},
        //                     {"message", "ARE YOU NOT ENTERTAINED?"},
        //                     {"timestamp", "2024-10-22T12:35:00Z"}
        //                     }
        //                 }
        //             }
        //     }
        // };

        // const json msgData = 
        //     {
        //         {"user", "Paul"},
        //         {"message", "Hello World."},
        //         {"timestamp", "2024-10-22T12:34:56Z"}
        //     };

//  -----DEV WORK-----



        //debugResetFirbaseDB(data);
        std::string todaysMessagesDBEndpointURL = parseTodaysDBURL();
        // putFirebaseData(todaysMessagesDBEndpointURL, msgData);
        return crow::response(jsonData.dump());

    });

    app.port(18080).multithreaded().run();
    return 0;
}
