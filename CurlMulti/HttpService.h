#pragma once

#include <vector>
#include <curl/curl.h>
#include <thread>
#include <functional>

class HttpService {
public:
    static HttpService& GetInstance();

    // Call this at the very end
    void Cleanup();

    void GetAsync(std::function<void()>& callback);
    void PostAsync();

private:
    struct HttpCallback {
        CURL* easyHandle;
        std::string buffer;
        std::function<void()>& callback;

        HttpCallback(CURL* easyHandle, std::function<void()> callback);
    };

    HttpService();


    std::thread m_EventThread;

    // TODO: Lock
    std::vector<HttpCallback> m_Callbacks;
    CURLM* m_pMultiHandle;

    int m_StillRunning; // Number of running handles

    CURL* SetupRequest();
    void FinishRequest(CURL* handle);

    void EventLoop();

    static size_t WriteData(char* data, size_t size, size_t nmemb, std::string* writerData);
};