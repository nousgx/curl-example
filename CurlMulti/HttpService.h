#pragma once

#include <list>
#include <curl/curl.h>
#include <thread>
#include <functional>
#include <future>
#include <iostream>

class HttpService {
public:
    static HttpService& GetInstance();

    // Call this at the very end
    void Cleanup();

    //std::future<std::string> GetAsync();
    //void GetVoid();
    std::future<std::string> GetAsync(std::string url);
    void GetAsync(std::string url, std::function<void(std::string)> callback);

    std::future<std::string> PostAsync(std::string url, std::string body);

private:
    HttpService();

    enum HttpMethodType {
        GET,
        POST
    };

    struct HttpRequest {
        CURL* handle = nullptr;
        std::string str;
        std::string body;  // we need this here because CURLOPT_POSTFIELDS doesn't copy the data https://stackoverflow.com/questions/48612613/c-libcurl-not-posting-data
        virtual void Callback() = 0;

        ~HttpRequest() {
            std::cout << "~HttpRequest\n";
        }
    };

    struct HttpRequestFuture : public HttpRequest {
        std::promise<std::string> promise;

        HttpRequestFuture(std::promise<std::string> p) {
            promise = std::move(p);
        }

        void Callback() override {
            promise.set_value(str);
        }
    };

    struct HttpRequestCallback : public HttpRequest {
        std::function<void(std::string)> callback;

        HttpRequestCallback(std::function<void(std::string)> callback) {
            this->callback = std::move(callback);
        }

        void Callback() override {
            callback(str);
        }
    };


    std::thread m_EventThread;

    // TODO: Lock
    // Use list here due to O(1) erase of list compared to O(n) erase of vector
    std::list<std::shared_ptr<HttpRequest>> m_Callbacks;
    CURLM* m_pMultiHandle;

    int m_StillRunning; // Number of running handles

    int m_Run;
    int m_Transfers;

    void SetupRequest(HttpMethodType method, std::string url, std::shared_ptr<HttpRequest> httpRequest, std::string body);
    void FinishRequest(CURL* handle);

    void EventLoop();

    static size_t WriteData(void* contents, size_t size, size_t nmemb, std::string* userp);
};