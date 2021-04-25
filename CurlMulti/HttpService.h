#pragma once

#include <list>
#include <curl/curl.h>
#include <thread>
#include <functional>
#include <future>

class HttpService {
public:
    static HttpService& GetInstance();

    // Call this at the very end
    void Cleanup();

    //std::future<std::string> GetAsync();
    void GetVoid();
    void PostAsync();

private:
    HttpService();


    std::thread m_EventThread;

    std::string temp;

    // TODO: Lock
    // Use list here due to O(1) erase of list compared to O(n) erase of vector
    //std::list<std::shared_ptr<HttpCallback>> m_Callbacks;
    CURLM* m_pMultiHandle;

    int m_StillRunning; // Number of running handles

    CURL* SetupRequest();
    void FinishRequest(CURL* handle);

    void EventLoop();

    static size_t WriteData(void* contents, size_t size, size_t nmemb, std::string* userp);
};