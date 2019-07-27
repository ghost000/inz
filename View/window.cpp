#include "window.h"

#include <QListWidgetItem>
#include <QApplication>
#include <QTextBrowser>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QComboBox>
#include <QShortcut>
#include <QTextEdit>
#include <QLabel>
#include <QDir>

Window::Window(QWidget *parent)
    : QWidget(parent)
    , textBrowserLeft(new QTextBrowser())
    , textBrowserRight(new QTextBrowser())
    , directoryComboBoxLeft(createComboBox(QDir::toNativeSeparators(QDir::currentPath())))
    , directoryComboBoxRight(createComboBox(QDir::toNativeSeparators(QDir::currentPath())))
    , browseButtonLeft(new QPushButton(tr("&Browse first pdf"),this))
    , browseButtonRight(new QPushButton(tr("&Browse second pdf"),this))
    , validateButton(new QPushButton(tr("&Validate"),this))
    , nextButton(new QPushButton(tr("&Next"),this))
    , previousButton(new QPushButton(tr("&Previous"),this))
    , findsBrowse(new QListWidget())
    , mainLayout(new QGridLayout(this))
    , layout(new QFormLayout())
    , pdfReader("")
    , currentScrollItem(0)
{
    setUpUI();
    connectSlotsAndSignals();
}

void Window::setUpUI()
{
    setWindowTitle(tr("Antyplagiat"));

    textBrowserLeft->setReadOnly(true);
    textBrowserRight->setReadOnly(true);
    nextButton->setEnabled(false);
    previousButton->setEnabled(false);

    mainLayout->addWidget(directoryComboBoxLeft, 0, 0);
    mainLayout->addWidget(browseButtonLeft, 0, 1);
    mainLayout->addWidget(directoryComboBoxRight, 0, 2);
    mainLayout->addWidget(browseButtonRight, 0, 3);
    layout->addRow(validateButton);
    layout->addRow(findsBrowse);
    layout->addRow(nextButton);
    layout->addRow(previousButton);
    mainLayout->addLayout(layout, 0, 4, 0, 1);

    mainLayout->addWidget(new QLabel(tr("PDF 1 text:")), 1, 0, 1, 1);
    mainLayout->addWidget(textBrowserLeft, 2, 0, 1, 2);
    mainLayout->addWidget(new QLabel(tr("PDF 2 text:")), 1, 2, 1, 1);
    mainLayout->addWidget(textBrowserRight, 2, 2, 1, 2);
}

void Window::connectSlotsAndSignals()
{
    connect(browseButtonLeft, &QAbstractButton::clicked, this, [this]()
    {
        Window::browse(textBrowserLeft, directoryComboBoxLeft);
    });

    connect(browseButtonRight, &QAbstractButton::clicked, this, [this]()
    {
        Window::browse(textBrowserRight, directoryComboBoxRight);
    });

    connect(findsBrowse, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onFindsBrowseItemClicked(QListWidgetItem*)));

    connect(validateButton, &QAbstractButton::clicked, this, &Window::validate);
    connect(nextButton, &QAbstractButton::clicked, this, &Window::next);
    connect(previousButton, &QAbstractButton::clicked, this, &Window::prev);

    connect(this, SIGNAL(valueTextChanged(const QString&, const QString&)),
            &plagiarismChecker, SLOT(setValue(const QString&, const QString&)));

    connect(&plagiarismChecker, SIGNAL(resultReady(QVector<QPair<int, int>> &)),
            this, SLOT(handleResults(QVector<QPair<int, int>> &)));

    connect(new QShortcut(QKeySequence::Quit, this),
            &QShortcut::activated, qApp, &QApplication::quit);
}

void Window::onFindsBrowseItemClicked(QListWidgetItem* item)
{    
    changeColorOfSelection(Qt::red, currentScrollItem);
    currentScrollItem = item->listWidget()->currentRow();
    moveScrollBar(currentScrollItem);
}

void Window::browse(QTextBrowser* textTextBrowser, QComboBox* directoryComboBox)
{
    QString directory = QDir::toNativeSeparators( QFileDialog::getOpenFileName(
                                                      this, tr("Bowse PDF file"),
                                                      QDir::currentPath(), tr("PDF file (*.pdf)")));
    if (!directory.isEmpty())
    {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }

    if(directory.right(4) != ".pdf")
    {
        showMsgBox("Please select pdf file !!!");
    }
    else
    {
        pdfReader.changePdfFilename(directory);
        textTextBrowser->setText(pdfReader.getTxt());
    }
}

void Window::validate()
{
    if(textBrowserLeft->toPlainText().isEmpty() || textBrowserRight->toPlainText().isEmpty())
    {
        showMsgBox("Please select two pdf files !!!");
    }
    else if(textBrowserLeft->textCursor().selectedText().isEmpty())
    {
        showMsgBox("Please select text in first pdf file !!!");
    }
    else
    {
        emit valueTextChanged(textBrowserLeft->textCursor().selectedText(), textBrowserRight->toPlainText());
    }
}

QComboBox* Window::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(false);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void Window::handleResults(QVector<QPair<int, int>>& result)
{
    wasPatternFoundQMessageBoxInfo(result);
    textBrowserUpdate(result);
}

void Window::wasPatternFoundQMessageBoxInfo(const QVector<QPair<int, int>>& result)
{
    QString info = "The pattern was";
    findsBrowse->clear();
    if(result.empty())
    {
        info += " not found.";
    }
    else
    {
        info += " found at positions nr: ";
        for(const auto& i : result)
        {
            info += "\n";
            info += QString::number(i.first);
            findsBrowse->addItem(QString::number(i.first));
        }
        findsBrowse->item(0)->setSelected(true);
    }
    showMsgBox(info);
}

const auto Window::prepareExtraSelections(const QVector<QPair<int, int>>& result)
{
    QTextCharFormat charFormat;
    charFormat.setBackground(Qt::red);

    QList<QTextEdit::ExtraSelection> extraSelections;
    for(const auto& i : result)
    {
        QTextEdit::ExtraSelection selection;
        selection.format = charFormat;
        selection.cursor = textBrowserRight->textCursor();
        selection.cursor.select(QTextCursor::Document);
        selection.cursor.setPosition(i.first);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, i.second);
        extraSelections.append(selection);
    }
    return extraSelections;
}

void Window::textBrowserUpdate(const QVector<QPair<int, int>>& result)
{
    enableScrollButton(result.size());
    textBrowserRight->textCursor().clearSelection();
    textBrowserRight->setExtraSelections(prepareExtraSelections(result));
}

void Window::enableScrollButton(int size)
{
    if (size > 1)
    {
        nextButton->setEnabled(true);
        previousButton->setEnabled(true);
    }
    else
    {
        nextButton->setEnabled(false);
        previousButton->setEnabled(false);
    }
}

void Window::showMsgBox(const QString& textToShow)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Antyplagiat"));
    msgBox.setText(textToShow);
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.exec();
}

void Window::moveScrollBar(int count)
{
    const double offsetInPercent = (double)textBrowserRight->extraSelections().at(count).cursor.selectionStart()
            / (double)textBrowserRight->toPlainText().size() * 100.00;
    const double offsetForScrollBar = offsetInPercent / 100.00 * textBrowserRight->verticalScrollBar()->maximum();

    textBrowserRight->verticalScrollBar()->setValue(offsetForScrollBar);
    findsBrowse->item(count)->setSelected(true);
    findsBrowse->scrollToItem(findsBrowse->item(count));
    changeColorOfSelection(Qt::green, count);
}

void Window::changeColorOfSelection(const QBrush &brush, const int count)
{
    QTextCharFormat charFormat;
    charFormat.setBackground(brush);
    QList<QTextEdit::ExtraSelection> extraSelection = textBrowserRight->extraSelections();
    extraSelection[count].format = charFormat;
    textBrowserRight->textCursor().clearSelection();
    textBrowserRight->setExtraSelections(extraSelection);
}

void Window::next()
{
    changeColorOfSelection(Qt::red, currentScrollItem);
    currentScrollItem++;
    if(currentScrollItem == textBrowserRight->extraSelections().size())
    {
        currentScrollItem = 0;
    }
    moveScrollBar(currentScrollItem);
}

void Window::prev()
{
    changeColorOfSelection(Qt::red, currentScrollItem);
    currentScrollItem--;
    if(currentScrollItem < 0)
    {
        currentScrollItem = textBrowserRight->extraSelections().size()-1;
    }
    moveScrollBar(currentScrollItem);
}
