#include "plagiarismchecker.h"

#include <algorithm>
#include <functional>

PlagiarismChecker::PlagiarismChecker(const QString& Pattern, const QString& Text)
    : text(Text)
    , pattern(Pattern)
    , result()
{
}


void PlagiarismChecker::setPattern(const QString& Pattern)
{
    pattern = Pattern;
}

void PlagiarismChecker::setText(const QString& Text)
{
    text = Text;
}

QString PlagiarismChecker::validate()
{
    if(text.isEmpty() || pattern.isEmpty())
    {
        return "One text is empty. This is error !!!";
    }
    return "";
}

void PlagiarismChecker::checker()
{
    result.clear();
    const auto buffText    = text.toStdU16String();
    const auto buffPattern = pattern.toStdU16String();

    auto it = std::search(buffText.begin(), buffText.end(),
                          std::boyer_moore_searcher(
                              buffPattern.begin(), buffPattern.end()));

    while(it != buffText.end())
    {
        result.push_back(std::make_pair(it - buffText.begin(), buffPattern.size()));
        it++;
        it = std::search(it, buffText.end(),
                         std::boyer_moore_searcher(
                             buffPattern.begin(), buffPattern.end()));
    }
}

const std::vector<std::pair<int, int>>& PlagiarismChecker::getResultOfPlagiarismChecking()
{
    return result;
}

