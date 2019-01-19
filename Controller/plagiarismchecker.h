#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iterator>

class PlagiarismChecker
{
public:
    PlagiarismChecker(const std::string& text1, const std::string& text2, std::string& error, const unsigned int& Precisions = 1);
    const std::vector<std::pair<int, std::string>>& getResultOfPlagiarismChecking();

private:
    void validate(const unsigned int& Precisions);
    std::vector<std::string> split(const std::string &s, char delim);
    void splitWordFromFirstText();
    void checker();


    const std::string                        text1;
    const std::string                        text2;
    short                                    precisions;
    std::string                              pattern;
    std::vector<std::pair<int, std::string>> result;
    std::string                              errorMessage;
    std::vector<std::string>                 splitWordsFromFirstText;

    template<typename Out>
    void split(const std::string &s, char delim, Out result)
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            *(result++) = item;
        }
    }
};

