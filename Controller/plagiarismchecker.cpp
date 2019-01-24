#include "plagiarismchecker.h"

#include <algorithm>
#include <functional>

void PlagiarismChecker::setValue(const QString& Pattern, const QString& Text)
{
    pattern = Pattern;
    text    = Text;
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
        result.push_back(qMakePair(it - buffText.begin(), buffPattern.size()));

        it++;

        it = std::search(it, buffText.end(),
                         std::boyer_moore_searcher(
                             buffPattern.begin(), buffPattern.end()));
    }

    emit resultReady(result);
}

void PlagiarismChecker::resultReady(QVector<QPair<int, int>> &Result)
{
    Result = result;
}

