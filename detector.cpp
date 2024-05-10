#include<bits/stdc++.h>
#include <cstdlib>
#include "finding_pattern.h"
using namespace std;
using namespace CountPattern;
namespace fs = std::filesystem;

vector<string> specialString = {"'YES'", "'Yes'", "'yes'", 
                                "'IMPOSSIBLE'", "\"Yes\"", "\"Yes\"", 
                                "\"yes\"", "\"IMPOSSIBLE\"", "998244353",
                                "__builtin_popcount", "__builtin_popcountll", 
                                "mt19937_64", "mt19937"};

map<string, bool> specialPrefix = {
    {"mt19937", true}, 
    {"#define", true},
    {"const", true},
    {"typename", true},
    {"typedef", true},
    {"using", true},
    {"template", true},
    {"freopen", true},
    {"//", true},
    {"#pragma", true}
};

void prepareDirectory(const string& directory) {
    fs::remove_all(directory); // Caution: This removes all existing contents!
    fs::create_directory(directory);
}

bool isZipFile(const string& path) {
    return path.size() >= 4 && path.substr(path.size() - 4) == ".zip";
}

void extractZipFile(const string& zipFilePath, const string& outputFolder) {
    // Redirects only standard output to /dev/null, keeping standard error visible
    string command = "unzip -o -O UTF-8 " + zipFilePath + " -d " + outputFolder + " > /dev/null";
    int result = system(command.c_str());
    // if (result != 0) {
    //     cerr << "Failed to extract " << zipFilePath << " file." << endl;
    //     exit(1);
    // }
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(NULL);

    string testDirectory, submissionDirectory;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if ((arg == "-s") && i + 1 < argc) {
            submissionDirectory = argv[++i];
            if (isZipFile(submissionDirectory)) {
                string tempDir = "temp_submission";
                prepareDirectory(tempDir);
                extractZipFile(submissionDirectory, tempDir);
                submissionDirectory = tempDir;
            }
        } else if ((arg == "-d") && i + 1 < argc) {
            testDirectory = argv[++i];
            if (isZipFile(testDirectory)) {
                string tempDir = "temp_testdata";
                prepareDirectory(tempDir);
                extractZipFile(testDirectory, tempDir);
                testDirectory = tempDir;
            }
        } else {
            cerr << "Usage: " << argv[0] << " -d <TestDataDirectory> -s <SubmissionDirectory>\n";
            return 1;
        }
    }

    if (testDirectory.empty() || submissionDirectory.empty()) {
        cerr << "Both test data and submission directory must be specified.\n";
        cerr << "Usage: " << argv[0] << " -d <TestDataDirectory> -s <SubmissionDirectory>\n";
        return 1;
    }

    // create list of words from test data to check if code files
    set<string> wordsInText;

    for (const auto & entry : fs::recursive_directory_iterator(testDirectory)) {
        if (!fs::is_regular_file(entry)) continue;

        ifstream inp(entry.path());

        int stringCount = 0;
        string line;
        while (inp >> line && stringCount <= 10) {
            if (line.size() > 50) {
                line = line.substr(0, 50);
            }
            if (line.size() >= 3) {
                int zeroCount = 0;
                bool isArrayLimit = false;

                // Check 100005, ... (array limit)
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] == '0') {
                        zeroCount++;
                        if (zeroCount >= 3) isArrayLimit = true;
                    }
                    else zeroCount = 0;
                }

                if (isArrayLimit) continue;

                //  Check number
                bool isNumber = true;
                for (int i = 0; i < line.size(); i++) {
                    if ('0' > line[i] || line[i] > '9') {
                        isNumber = false;
                    }
                }

                // Create a list contains strings appear in TestData
                if (!isNumber) {
                    //  Convert string s to "s"
                    string quotedLine = "\"" + line + "\"";
                    wordsInText.insert(quotedLine);

                    quotedLine = "\'" + line + "\'";
                    wordsInText.insert(quotedLine);
                }
                else {
                    wordsInText.insert(line);
                }

                stringCount++;
            }
        }
    }

    // create dictionary from test data by excluding some special words
    for (auto s: specialString) {
        if (wordsInText.find(s) != wordsInText.end()) {
            wordsInText.erase(s);
        }
    }

    vector<string> dictionary(wordsInText.begin(), wordsInText.end());

    // calculate similar score of each code file with the test data
    vector<pair<long long, fs::path>> similarList;
    for (const auto & entry : fs::recursive_directory_iterator(submissionDirectory)) {
        if (!fs::is_regular_file(entry)) continue;
        ifstream inputFile(entry.path());

        long long similarity = 0;

        string line;
        string allStrings;
        while (getline(inputFile, line)) {
            string prefix;

            // Lines with define, const, ...
            for (auto ch: line) {
                if (ch != ' ') prefix += ch;
                else break;
            }

            if (specialPrefix.count(prefix)) continue;

            // Lines with array limit
            string newLine;
            stack<char> st;
            bool open = false, close = false;
            for (auto ch: line) {
                if (ch == '[') {
                    st.push(ch);
                }
                else if (ch == ']') {
                    if (st.size()) st.pop();
                }
                else {
                    if (st.empty()) newLine += ch;
                }
            }

            if (open && close) continue;

            allStrings += newLine + '\n';
        }

        similarity = countPattern(allStrings, dictionary);

        similarList.push_back({similarity, entry.path()});
    }

    sort(similarList.begin(), similarList.end(), greater<pair<long long, fs::path>>());

    for (auto [score, name]: similarList) {
        cout << score << ' ' << name.filename() << "\n";
    }

    fs::remove_all("temp_testdata");
    fs::remove_all("temp_submission");
 
    return 0;
}