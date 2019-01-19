#include "plagiarismchecker.h"

#include <algorithm>
#include <functional>
#include <iostream>

PlagiarismChecker::PlagiarismChecker(const std::string& text1, const std::string& text2, std::string& error, const unsigned int& Precisions)
    : text1(text1),
      text2(text2),
      precisions(),
      pattern(),
      result(),
      errorMessage(error)
{
    validate(Precisions);
    //splitWordFromFirstText();
    checker();
}

void PlagiarismChecker::validate(const unsigned int& Precisions)
{
    /*if(precisions < 4)
    {
        precisions = 4;
    }
    else
    {
        precisions = Precisions;
    }*/

    precisions = Precisions;

    if(text1.size() == 0 || text2.size() == 0)
    {
        errorMessage = "One text is empty. This is error !!!";
    }
}

std::vector<std::string> PlagiarismChecker::split(const std::string &s, char delim)
{
    //debug
    //std::cout << "PlagiarismChecker::split --> \n";

    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    //for(auto i : elems)
    //    std::cout << i << "\n";

    return elems;
}

void PlagiarismChecker::splitWordFromFirstText()
{
    //debug
    //std::cout << "PlagiarismChecker::splitWordFromFirstText --> \n";

    std::string buff = text1;
    buff.erase(std::remove(buff.begin(), buff.end(), '\n'), buff.end());
    splitWordsFromFirstText = split(buff, ' ');

    //debug
    //for(auto i : splitWordsFromFirstText)
    //    std::cout << i << "\n";
}

void PlagiarismChecker::checker()
{
    //debug
    //std::cout << "PlagiarismChecker::checker --> \n";


    /*auto buff1 = text1;
    buff1.erase(std::remove(buff1.begin(), buff1.end(), '\n'), buff1.end());

    for(unsigned int i = 0; (i + precisions) < buff1.size(); i++)
    {
        auto itb = buff1.begin() + i;
        auto ite = buff1.begin() + i + precisions;

        auto buff = text2;
        buff.erase(std::remove(buff.begin(), buff.end(), '\n'), buff.end());

        auto it = std::search(buff.begin(), buff.end(),
                           std::boyer_moore_searcher(
                               itb, ite));

        //debug
        if(it != buff.end())
        {
            std::cout << *itb << "  -   " << *ite << "\n";

            //debug
            //std::cout << " !!!!! text2.begin() - it " << it - text2.begin() << " -- buf == " << text1.substr(i,i+precisions) << " i = " << i << " precision = " << precisions << "\n";

            result.push_back(std::make_pair(it - buff.begin(), buff1.substr(i, precisions)));
            std::cout << buff1.substr(i, precisions) << "\n";
        }

    }*/
    auto buff = text2;
    buff.erase(std::remove(buff.begin(), buff.end(), '\n'), buff.end());

    auto buff1 = text1;
    buff1.erase(std::remove(buff1.begin(), buff1.end(), '\n'), buff1.end());

    auto it = std::search(buff.begin(), buff.end(),
                       std::boyer_moore_searcher(
                           buff1.begin(), buff1.end()));
    while(it != buff.end())
    {

        result.push_back(std::make_pair(it - buff.begin(), buff1));

        it++;
        it = std::search(it, buff.end(),
                               std::boyer_moore_searcher(
                                   buff1.begin(), buff1.end()));
        std::cout << buff1 << "\n";

    }


    /*auto iterFront = splitWordsFromFirstText.begin();
    auto iterFrontForWindowSerch = splitWordsFromFirstText.begin();
    std::string buff;

    for(unsigned int i = 0; i < splitWordsFromFirstText.size(); i++)
    {
        buff.clear();
        if((iterFrontForWindowSerch + i + precisions) == splitWordsFromFirstText.end())
        {
            return;
        }

        for(auto iterFrontB = iterFront + i; iterFrontB != iterFrontForWindowSerch + i + precisions; iterFrontB++)
        {
            buff.append(*iterFrontB + " ");
        }

        //debug
        //std::cout << buff << " lolo \n";

        std::string buff2 = text2;
        buff2.erase(std::remove(buff2.begin(), buff2.end(), '\n'), buff2.end());

        auto it = std::search(buff2.begin(), buff2.end(),
                           std::boyer_moore_searcher(
                               buff.begin(), buff.end()));
        //debug
        //std::cout << " text2.begin() - it == " << it - text2.begin() << "\n";

        if(it != buff2.end())
        {
            //debug
            std::cout << " !!!!! text2.begin() - it " << it - buff2.begin() << " -- buf == " << buff << "buff size = " << buff.size() << "\n";

            result.push_back(std::make_pair(it - buff2.begin(), buff));
        }
    }*/
}

const std::vector<std::pair<int, std::string>>& PlagiarismChecker::getResultOfPlagiarismChecking()
{
    return result;
}

