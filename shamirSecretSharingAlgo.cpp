#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

string multiply(const string& num1, const string& num2) {
    ll n1 = stoll(num1);
    ll n2 = stoll(num2);
    ll result = n1 * n2;
    return to_string(result);
}

string add(const string& num1, const string& num2) {
    ll n1 = stoll(num1);
    ll n2 = stoll(num2);
    ll result = n1 + n2;
    return to_string(result);
}

string decode(const string& value, int base) {
    string result = "0";
    for (char digit : value) {
        int digitVal = (isdigit(digit)) ? digit - '0' : tolower(digit) - 'a' + 10;
        result = multiply(result, to_string(base));
        result = add(result, to_string(digitVal));
    }
    return result;
}

string lagrangeInterpolation(const vector<pair<int, string>>& points) {
    int k = points.size();
    string constant = "0";

    for (int i = 0; i < k; ++i) {
        string term = points[i].second;
        string numerator = "1", denominator = "1";

        for (int j = 0; j < k; ++j) {
            if (i != j) {
                numerator = multiply(numerator, "-" + to_string(points[j].first));
                denominator = multiply(denominator, to_string(points[i].first - points[j].first));
            }
        }

        constant = add(constant, multiply(term, numerator));
    }

    return constant;
}

void parseJSON(const string& filename, map<string, string>& keys, vector<map<string, string>>& points) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        size_t pos;
        if ((pos = line.find("\"keys\":")) != string::npos) {
            getline(file, line);
            keys["n"] = line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1);
            getline(file, line);
            keys["k"] = line.substr(line.find(":") + 1);
        }
        else if ((pos = line.find("\"base\"")) != string::npos) {
            map<string, string> root;
            root["base"] = line.substr(line.find(":") + 3, line.find_last_of("\"") - line.find(":") - 3);
            getline(file, line);
            root["value"] = line.substr(line.find(":") + 3, line.find_last_of("\"") - line.find(":") - 3);
            points.push_back(root);
        }
    }
}

int main() {
    string filename = "input2.json";
    map<string, string> keys;
    vector<map<string, string>> points;

    parseJSON(filename, keys, points);

    int n = stoi(keys["n"]);
    int k = stoi(keys["k"]);

    // cout << n << ' ' << k << endl;

    vector<pair<int, string>> decodedPoints;
    for (int i = 0; i < n; ++i) {
        int x = i + 1;
        int base = stoi(points[i]["base"]);
        string y = decode(points[i]["value"], base);
        decodedPoints.emplace_back(x, y);
    }

    // for (auto& i: decodedPoints)
    //     cout << i.first << ' ' << i.second << endl;

    vector<pair<int, string>> selectedPoints(decodedPoints.begin(), decodedPoints.begin() + k);

    string constant = lagrangeInterpolation(selectedPoints);

    cout << "Constant term (c): " << constant << endl;

    return 0;
}