// CurlMulti.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "HttpService.h"


class Foo {
public:
    void curl() {
        HttpService::GetInstance().GetAsync("http://localhost:3000/", std::bind(&Foo::callback, this, std::placeholders::_1));
    }

    void callback(std::string a) {
        std::cout << "FOO OOOOOOO!!!!\n";
    }
};

int main()
{
    HttpService& httpService = HttpService::GetInstance();

    std::future<std::string> f5 = httpService.PostAsync("http://localhost:3000/", "{\"hi\" : \"there\"}");
    std::cout << f5.get() << "\n";

    std::future<std::string> f1 = httpService.GetAsync("http://localhost:3000/");

    std::cout << f1.get() << "\n";


    httpService.GetAsync("http://localhost:3000/", [](std::string a) { std::cout << "Testing callback\n" << "Received: string\n\t" << a; });

    Foo foo;
    foo.curl();
    //httpService.GetVoid();

    std::future<std::string> f3 = httpService.GetAsync("http://localhost:3000/delay/");
    std::future<std::string> f2 = httpService.GetAsync("http://localhost:3000/delay/");


    Foo foo1;
    foo1.curl();   

    std::cout << f3.get() << "\n";

    foo.curl();

    std::cout << f2.get() << "\n";

   /* std::future<std::string> f1 = httpService.GetAsync();

    try {
        std::cout << f1.get() << "\n";
    }
    catch (const std::future_error & e) {
        std::cout << "Caught a future_error with code\"" << e.code() << "\"\nMessage: \"" << e.what() << "\"\n";
    }*/
    httpService.Cleanup(); // or HttpService::Get().Cleanup();
    curl_global_cleanup();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
