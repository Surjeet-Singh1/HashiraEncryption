#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

// Function to convert a number from any base to decimal
long long baseToDecimal(const std::string& value, int base) {
    long long result = 0;
    long long power = 1;
    
    for (int i = value.length() - 1; i >= 0; i--) {
        char digit = value[i];
        int digitValue;
        
        if (digit >= '0' && digit <= '9') {
            digitValue = digit - '0';
        } else if (digit >= 'a' && digit <= 'z') {
            digitValue = digit - 'a' + 10;
        } else if (digit >= 'A' && digit <= 'Z') {
            digitValue = digit - 'A' + 10;
        } else {
            continue;
        }
        
        if (digitValue >= base) {
            continue;
        }
        
        result += digitValue * power;
        power *= base;
    }
    
    return result;
}


double lagrangeInterpolation(const std::vector<std::pair<int, long long>>& points, int x) {
    double result = 0.0;
    int n = points.size();
    
    for (int i = 0; i < n; i++) {
        double term = points[i].second;
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                term *= (double)(x - points[j].first) / (points[i].first - points[j].first);
            }
        }
        
        result += term;
    }
    
    return result;
}


std::string extractValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\"";
    size_t pos = json.find(searchKey);
    if (pos == std::string::npos) return "";
    
    pos = json.find(":", pos);
    if (pos == std::string::npos) return "";
    
    pos = json.find("\"", pos);
    if (pos == std::string::npos) return "";
    
    size_t start = pos + 1;
    size_t end = json.find("\"", start);
    if (end == std::string::npos) return "";
    
    return json.substr(start, end - start);
}

void processTestCase(const std::string& filename, int testCaseNum) {
    std::cout << "Test Case " << testCaseNum << ":" << std::endl;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    

    int k = std::stoi(extractValue(content, "k"));
    std::vector<std::pair<int, long long>> points;
    
   
    for (int i = 1; i <= 20; i++) { 
        std::string iStr = std::to_string(i);
        std::string pointKey = "\"" + iStr + "\"";
        size_t pointPos = content.find(pointKey);
        
        if (pointPos != std::string::npos) {
           
            size_t blockStart = content.rfind("{", pointPos);
            size_t blockEnd = content.find("}", pointPos);
            std::string pointBlock = content.substr(blockStart, blockEnd - blockStart + 1);
            
            std::string base = extractValue(pointBlock, "base");
            std::string value = extractValue(pointBlock, "value");
            
            if (!base.empty() && !value.empty()) {
                int baseInt = std::stoi(base);
                long long y = baseToDecimal(value, baseInt);
                points.push_back({i, y});
                
                if (points.size() >= k) break; 
            }
        }
    }
    
    std::cout << "k = " << k << " (need " << k << " points)" << std::endl;
    std::cout << "Using " << points.size() << " points:" << std::endl;
    for (const auto& point : points) {
        std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
    }
    
 
    double secret = lagrangeInterpolation(points, 0);
    std::cout << "Secret (constant term): " << (long long)std::round(secret) << std::endl << std::endl;
}

int main() {
    
    processTestCase("testcase1.json", 1);
    processTestCase("testcase2.json", 2);
    
    return 0;
}
