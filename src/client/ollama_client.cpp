#include "ollama_client.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

OllamaClient::OllamaClient(const std::string& baseUrl)
    : baseUrl_(baseUrl), model_("phi-3") {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::cout << "[OllamaClient] Initialized with base URL: " << baseUrl_ << std::endl;
}

void OllamaClient::setModel(const std::string& m) {
    model_ = m;
}

void OllamaClient::setTemperature(float t) {
    temperature_ = t;
}

void OllamaClient::setMaxTokens(int n) {
    maxTokens_ = n;
}

std::expected<json, APIError> OllamaClient::chat(const json& messages) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return std::unexpected(APIError{0, "Failed to initialize CURL"});
    }

    std::string url = baseUrl_ + "/api/chat";

    json payload = {
        {"model", model_},
        {"messages", messages},
        {"stream", false},
        {"temperature", temperature_},
        {"max_tokens", maxTokens_}
    };

    std::string payloadStr = payload.dump();
    std::string responseStr;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadStr.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);

    // Needed for ngrok / self-signed certificates
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return std::unexpected(APIError{0, std::string("CURL error: ") + curl_easy_strerror(res)});
    }

    if (http_code != 200) {
        return std::unexpected(APIError{static_cast<int>(http_code), "HTTP error: " + responseStr});
    }

    try {
        return json::parse(responseStr);
    } catch (const std::exception& e) {
        return std::unexpected(APIError{0, std::string("JSON parse error: ") + e.what()});
    }
}