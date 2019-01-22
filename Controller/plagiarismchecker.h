#pragma once

#include <QString>
#include <vector>

class PlagiarismChecker
{
public:
    PlagiarismChecker(const QString& text1, const QString& text2, QString& error);
    const std::vector<std::pair<int, int>>& getResultOfPlagiarismChecking();

private:
    void validate();
    void checker();

    const QString                    text1;
    const QString                    text2;
    QString                          pattern;
    std::vector<std::pair<int, int>> result;
    QString                          errorMessage;

};

