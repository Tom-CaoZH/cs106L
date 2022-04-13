#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <unordered_set>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;
using std::ifstream;        using std::ofstream;

unordered_set<string> findWikiLinks(string& page_html);
string fileTostring(ifstream& input);
string itTostring(string::iterator word_start,string::iterator word_end);

// the result isn't be inputed into a file , i just output it in the screan
int main() {
    unordered_set<string> relate_links;
    ifstream file1("quokka.txt");

    string f_ks = fileTostring(file1);
    relate_links = findWikiLinks(f_ks);

    for(string relate_link : relate_links) {
        cout << relate_link << endl;
    }
}

string fileTostring(ifstream& input) {
    string ret;
    string line;
    while(std::getline(input,line)) {
        ret += line + " ";
    }
    return ret;
}

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

unordered_set<string> findWikiLinks(string& page_html) {
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
