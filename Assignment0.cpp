#include <iostream>
#include <sstream>
#include <vector>
#include <sstream>

using namespace std;

// replace every occurrence of substring with another substring
string replace_substring(string line, string replacement, string to_replace) {
    size_t n = 0;
    while ((n = line.find(replacement, n)) != string::npos) {
        line.replace(n, replacement.length(), to_replace);
        n += to_replace.length();
    }
    return line;
}


int main(int argc, char *argv[]) {
    string line;
    // replace every occurrence of new line with whitespace
    line = replace_substring(argv[1], "\\n", " ");
    // replace every occurrence of tab with whitespace
    line = replace_substring(line, "\\t", " ");
    
    // replace several whitespaces with one
    size_t n;
    while((n = line.find("  ")) != string::npos )
        line = line.replace(n, 2, " " );

    // split string to words by whitespaces
    stringstream new_line(line);
    string segment;
    vector<string> seglist;

    while(getline(new_line, segment, ' '))
    {
        seglist.push_back(segment);
    }

    // the number of words is the size of array received by string splitting
    cout << "Word count: " << seglist.size() << '\n';
    return 0;
}