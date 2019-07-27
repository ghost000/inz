#include "plagiarismchecker.h"

#include <algorithm>
#include <functional>

void PlagiarismChecker::setValue(const QString& Pattern, const QString& Text)
{
    if(pattern != Pattern || text != Text)
    {
        pattern = Pattern;
        text = Text;
        checker();
    }
}

void PlagiarismChecker::checker()
{
    QVector<QPair<int, int>> result;
    const auto buffText    = text.toStdU16String();
    const auto buffPattern = pattern.toStdU16String();

    auto resultIt = std::search(buffText.begin(), buffText.end(),
                          std::boyer_moore_searcher(
                              buffPattern.begin(), buffPattern.end()));

    while(resultIt != buffText.end())
    {
        result.push_back(qMakePair(resultIt - buffText.begin(), buffPattern.size()));

        resultIt++; //moves pointer to beginning of indicated text.
                    //otherwise it always takes same beginning of text and spins around.

        resultIt = std::search(resultIt, buffText.end(),
                         std::boyer_moore_searcher(
                             buffPattern.begin(), buffPattern.end()));
    }

    emit resultReady(result);
}
