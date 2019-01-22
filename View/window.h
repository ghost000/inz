#pragma once

#include <QWidget>
#include <QDir>

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
    void browse1();
    void browse2();
    void validate();
    void animateBrowseClick();

private:
    QComboBox* createComboBox(const QString &text);

    QString            filename1;
    QString            filename2;
    QTextBrowser       *text1TextBrowser;
    QTextBrowser       *text2TextBrowser;
    QComboBox          *directoryComboBox1;
    QComboBox          *directoryComboBox2;
    QPushButton        *browseButton1;
    QPushButton        *browseButton2;
    QPushButton        *validateButton1;
    PdfReader          pdfReader;
    QString            text1;
    QString            text2;
    PlagiarismChecker* plagiarismChecker;
};
