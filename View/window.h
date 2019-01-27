#pragma once

#include <QWidget>

#include "Controller/pdfreader.h"
#include "Controller/plagiarismchecker.h"

class QPushButton;
class QComboBox;
class QTextBrowser;
class QTextEdit;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget* parent = nullptr);

private slots:
    void browse(QTextBrowser* textTextBrowser, QComboBox* directoryComboBox);
    void validate();
    void animateBrowseClick();

public slots:
    void handleResults(QVector<QPair<int, int>> &result);

signals:
    void valueTextChanged(const QString& Pattern, const QString& Text);

private:
    void textBrowserUpdate(const QVector<QPair<int, int>> &result);
    void wasPatternFoundQMessageBoxInfo(const QVector<QPair<int, int>> &result);
    void showMsgBox(const QString& textToShow);

    QComboBox* createComboBox(const QString &text);

    QTextBrowser       *text1TextBrowser;
    QTextBrowser       *text2TextBrowser;

    QComboBox          *directoryComboBox1;
    QComboBox          *directoryComboBox2;

    QPushButton        *browseButton1;
    QPushButton        *browseButton2;
    QPushButton        *validateButton1;
    QPushButton        *clearButton;

    PdfReader          pdfReader;

    PlagiarismChecker  plagiarismChecker;
};

//God object
