#pragma once

#include <QString>
#include <vector>

class PlagiarismChecker
{
public:
    explicit PlagiarismChecker(const QString& Pattern, const QString& Text);
    const std::vector<std::pair<int, int>>& getResultOfPlagiarismChecking();
    QString                                 validate();
    void                                    checker();
    void                                    setPattern(const QString& Pattern);
    void                                    setText(const QString& Text);

private:
    QString                          text;
    QString                          pattern;
    std::vector<std::pair<int, int>> result;
};

