#pragma once

#include <QString>
#include <QVector>
#include <QPair>
#include <QObject>

class PlagiarismChecker : public QObject
{
    Q_OBJECT

public:
    PlagiarismChecker()          = default;
    virtual ~PlagiarismChecker() = default;

public slots:
    void setValue(const QString& Pattern, const QString& Text);

signals:
    void resultReady(QVector<QPair<int, int>> &result);

private:
    void checker();

    QString text;
    QString pattern;
};
