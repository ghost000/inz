#include "plagiarismchecker.h"

#include <algorithm>
#include <functional>
#include <iostream>

PlagiarismChecker::PlagiarismChecker(const QString& text1, const QString& text2, QString& error)
    : text1(text1)
    , text2(text2)
    , pattern()
    , result()
    , errorMessage(error)
{
    validate();
    checker();
}

void PlagiarismChecker::validate()
{
    if(text1.size() == 0 || text2.size() == 0)
    {
        errorMessage = "One text is empty. This is error !!!";
    }
}

void PlagiarismChecker::checker()
{

    auto buff = text2;
    //buff.erase(std::remove(buff.begin(), buff.end(), '\n'), buff.end());

    auto buff1 = text1;
    //buff1.erase(std::remove(buff1.begin(), buff1.end(), '\n'), buff1.end());

    auto it = std::search(buff.toStdU16String().begin(), buff.toStdU16String().end(),
                            std::boyer_moore_searcher(
                              buff1.toStdU16String().begin(), buff1.toStdU16String().end()));

    while(it != buff.toStdU16String().end())
    {
        result.push_back(std::make_pair(it - buff.toStdU16String().begin(), buff1.toStdU16String().size()));

        it++;

        it = std::search(it, buff.toStdU16String().end(),
                         std::boyer_moore_searcher(
                             buff1.toStdU16String().begin(), buff1.toStdU16String().end()));

        std::cout << buff1.toStdString() << "\n";

    }
}

const std::vector<std::pair<int, int>>& PlagiarismChecker::getResultOfPlagiarismChecking()
{
    return result;
}

