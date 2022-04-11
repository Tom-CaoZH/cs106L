#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "wikiscraper.h"
#include "error.h"

using std::cout;            using std::endl;
using std::cerr;            using std::string;
using std::unordered_map;   using std::unordered_set;

string itTostring(string::iterator word_start,string::iterator word_end) {
    string ret;
    while(word_start != word_end) {
        ret += (*word_start);
        word_start++;
    }
    return ret;
}

// pay attention to the use of the function all_of
bool judge(char ch) {
    if(((ch) != '#' && (ch) != ':')) {
        return true;
    }
    return false;
}

unordered_set<string> findWikiLinks(const string& inp) {
    string page_html = inp;
    unordered_set<string> output;
    string target = "/wiki/";
    string page_name;

    string::iterator it_s;
    string::iterator word_start;

    auto start = page_html.begin();
    auto end = page_html.end();

    while(start != end) {
        it_s = std::search(start,end,target.begin(),target.end());
        if(it_s != end) {
            word_start = it_s + 6;  // reach the name
            it_s = std::find(word_start,end,'\"');
            if(std::all_of(word_start,it_s,judge)) {
                page_name = itTostring(word_start,it_s);
                output.insert(page_name);
            }
        }
        start = it_s;
    }
    return output;
}



/*
 * ==================================================================================
 * |                           DON"T EDIT ANYTHING BELOW HERE                       |
 * ==================================================================================
 */
unordered_set<string> WikiScraper::getLinkSet(const string& page_name) {
    if(linkset_cache.find(page_name) == linkset_cache.end()) {
        auto links = findWikiLinks(getPageSource(page_name));
        linkset_cache[page_name] = links;
    }
    return linkset_cache[page_name];
}


WikiScraper::WikiScraper() {
    (void)getPageSource("Main_Page");
#ifdef _WIN32
    // define something for Windows
    system("cls");
#else
    // define it for a Unix machine
    system("clear");
#endif
}


string createPageUrl(const string& page_name) {
    return "https://en.wikipedia.org/wiki/" + page_name;
}

void notFoundError(const string& msg, const string& page_name, const string& url) {
    const string title = "    AN ERROR OCCURED DURING EXECUTION.    ";
    const string border(title.size() + 4, '*');
    cerr << endl;
    errorPrint(border);
    errorPrint("* " + title + " *");
    errorPrint(border);
    errorPrint();
    errorPrint("Reason: " + msg);
    errorPrint();
    errorPrint("Debug Information:");
    errorPrint();
    errorPrint("\t- Input parameter: " + page_name);
    errorPrint("\t- Attempted url: " + url);
    errorPrint();
}

string WikiScraper::getPageSource(const string &page_name) {
    const static string not_found = "Wikipedia does not have an article with this exact name.";
    if(page_cache.find(page_name) == page_cache.end()) {
        QUrl url(createPageUrl(page_name).c_str()); // need c string to convert to QString

        QNetworkRequest request(url);

        QNetworkReply *reply(manager.get(request));

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()) , &loop, SLOT(quit()));
        loop.exec();

        string ret = reply->readAll().toStdString();
        if(std::search(ret.begin(), ret.end(), not_found.begin(), not_found.end()) != ret.end()){
            notFoundError("Page does not exist!", page_name, url.toString().toStdString());
            return "";
        }
        size_t indx = ret.find("plainlinks hlist navbar mini");
        if(indx != string::npos) {
            return ret.substr(0, indx);
        }
        page_cache[page_name] = ret;
    }
    return page_cache[page_name];
}





