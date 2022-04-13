
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;




/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */

void printloader(const vector<string>& loader) {
    cout << "{ " ;
    for(auto item : loader) {
        cout << item << " ,";
    }
    cout << " }" << endl;
}

int getComLinks(unordered_set<string> links1, unordered_set<string> links2) {
    int counter = 0;
    for(auto it = links1.begin(); it != links1.end(); it++) {
        auto it2 = links2.find(*it);
        if(it2 != links2.end()) {
            counter++;
        }
    }
    return counter;
}

vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    WikiScraper scraper;

    auto cmpFn = [end_page,&scraper](const vector<string>& loader1, const vector<string> &loader2) {
        string page1 = loader1[loader1.size()-1];
        string page2 = loader2[loader2.size()-1];
        unordered_set<string> end_links = scraper.getLinkSet(end_page);
        int num1 = getComLinks(scraper.getLinkSet(page1),end_links);
        int num2 = getComLinks(scraper.getLinkSet(page2),end_links);
        return num1 - num2;
    };

    priority_queue<vector<string>,vector<vector<string>>,decltype(cmpFn)> loaders(cmpFn);
    vector<string> start;
    start.push_back(start_page);
    unordered_set<string> visited;
    loaders.push(start);

    while(!loaders.empty()) {
        vector<string> first_loader = loaders.top();
        printloader(first_loader);
        loaders.pop();
        string taregt = first_loader[first_loader.size() - 1];
        auto target_set = scraper.getLinkSet(first_loader[first_loader.size() - 1]);
        auto it = std::find(target_set.begin(),target_set.end(),end_page);
        if(it != target_set.end()) {
            // can find the end_page then return
            first_loader.push_back(end_page);
            return first_loader;
        }
        else {
            // going to search
            for(auto link : target_set) {
                auto it2 = visited.find(link);
                if(it2 == visited.end()) {
                    // unvisited
                    visited.insert(link);
                    vector<string> loader = first_loader;
                    first_loader.push_back(link);
                    loaders.push(first_loader);
                }
            }
        }
    }
    // no target
    return {};
}



int main() {
    auto ladder = findWikiLadder("Milkshake", "Gene");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        printloader(ladder);
    }

    return 0;
}




