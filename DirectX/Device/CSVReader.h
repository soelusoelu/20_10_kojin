#pragma once

#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader();
    CSVReader(const std::string& fileName);
    ~CSVReader();
    std::vector<int> load(const std::string& fileName);
    std::vector<std::string> loadString(const std::string& fileName);
    std::vector<int> getParseData() const;
    std::vector<std::string> getParseStringData() const;
    int getWidth();
    int getHeight();

private:
    void parse(const std::string& filePath);
    void parseString(const std::string& filePath);

private:
    std::vector<int> mCSV;
    std::vector<std::string> mCSVString;
    int mWidthCount;
    int mHeightCount;
};
