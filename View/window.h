#pragma once

#include <QWidget>

#include "Controller/pdfreader.h"
#include "Controller/plagiarismchecker.h"

class QListWidgetItem;
class QTextBrowser;
class QFormLayout;
class QGridLayout;
class QListWidget;
class QPushButton;
class QScrollBar;
class QComboBox;
class QTextEdit;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget* parent = nullptr);

private slots:
    void browse(QTextBrowser* textTextBrowser, QComboBox* directoryComboBox);
    void validate();
    void onFindsBrowseItemClicked(QListWidgetItem *item);

public slots:
    void handleResults(QVector<QPair<int, int>> &result);

signals:
    void valueTextChanged(const QString& Pattern, const QString& Text);

private:
    void setUpUI();
    void connectSlotsAndSignals();
    const auto prepareExtraSelections(const QVector<QPair<int, int>>& result);
    void textBrowserUpdate(const QVector<QPair<int, int>> &result);
    void enableScrollButton(int size);
    void wasPatternFoundQMessageBoxInfo(const QVector<QPair<int, int>> &result);
    void showMsgBox(const QString& textToShow);
    void moveScrollBar(int count);
    void changeColorOfSelection(const QBrush &brush, const int count);
    void prev();
    void next();

    QComboBox* createComboBox(const QString &text);

    QTextBrowser      *textBrowserLeft;
    QTextBrowser      *textBrowserRight;

    QComboBox         *directoryComboBoxLeft;
    QComboBox         *directoryComboBoxRight;

    QPushButton       *browseButtonLeft;
    QPushButton       *browseButtonRight;
    QPushButton       *validateButton;
    QPushButton       *nextButton;
    QPushButton       *previousButton;
    QListWidget       *findsBrowse;

    QGridLayout       *mainLayout;
    QFormLayout       *layout;

    PdfReader         pdfReader;
    PlagiarismChecker plagiarismChecker;
    int               currentScrollItem;
};
