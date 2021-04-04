#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <regex>
#include "curl/curl.h"
#include <curl/curl.h>
using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


string findLinkUrl(string text, string &url)
{
	size_t startLinkPos = text.find("<a");
	if (startLinkPos == -1) {
		return "";
	}
	string linkContent = text.substr(startLinkPos);

	size_t endLinkPos = linkContent.find(">");
	if (endLinkPos == -1) {
		return "";
	}
	linkContent = linkContent.substr(0, endLinkPos);

	size_t hrefPos = linkContent.find("href=");
	if (hrefPos == -1) {
		return "";
	}
	string closingScope = linkContent.substr(hrefPos + 5, 1);
	linkContent = linkContent.substr(hrefPos + 6);
	url = linkContent.substr(0, linkContent.find(closingScope));
	if (url == "javascript:void(0)") {
		url = "";
	}
	return text.substr(startLinkPos + 2);
}


int main(void)
{
	CURL* curl;
	CURLcode res;
	string htmlContent;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.ysu.am/main/");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		string url;
		do {
			url = "";
			htmlContent = findLinkUrl(htmlContent, url);
			if (url != "") {
				cout << url << endl;
			}
		} while (htmlContent != "");
	}

	cout << endl << "-------------------" << endl;
	
/*
	string text = "Hello World";
	cout << text.find("r") << endl;
	cout << text.substr(3, 4) << endl;
	*/
	return 0;
}


