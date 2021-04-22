#pragma once

#include <vector>
#include <curl/curl.h>
#include <thread>

class HttpService {
public:
    static HttpService& GetInstance();

    // Call this at the very end
    void Cleanup();

    void GetAsync();
    void PostAsync();

private:
    HttpService();


    std::thread m_EventThread;

    //std::vector<CURL*> m_pEasyHandles;
    CURLM* m_pMultiHandle;

    int m_StillRunning; // Number of running handles

    CURL* SetupRequest();
    void FinishRequest(CURL* handle);

    void EventLoop();
};