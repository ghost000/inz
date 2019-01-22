#include "window.h"

#include <QtWidgets>
#include <iostream>


Window::Window(QWidget *parent) : QWidget(parent), pdfReader("")
{
    setWindowTitle(tr("Antyplagiat"));

    browseButton1           = new QPushButton(tr("&Browse first pdf"),this);
    browseButton2           = new QPushButton(tr("&Browse second pdf"),this);
    validateButton1         = new QPushButton(tr("&Validate"),this);
    text1TextBrowser        = new QTextBrowser();
    text2TextBrowser        = new QTextBrowser();
    QGridLayout *mainLayout = new QGridLayout(this);

    directoryComboBox1 = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));
    directoryComboBox2 = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));

    text1TextBrowser->setReadOnly(true);
    text2TextBrowser->setReadOnly(true);

    connect(browseButton1, &QAbstractButton::clicked, this, &Window::browse1);
    connect(browseButton2, &QAbstractButton::clicked, this, &Window::browse2);
    connect(validateButton1, &QAbstractButton::clicked, this, &Window::validate);
    connect(directoryComboBox1->lineEdit(), &QLineEdit::returnPressed, this, &Window::animateBrowseClick);
    connect(directoryComboBox2->lineEdit(), &QLineEdit::returnPressed, this, &Window::animateBrowseClick);

    mainLayout->addWidget(directoryComboBox1, 0, 0);
    mainLayout->addWidget(browseButton1, 0, 1);
    mainLayout->addWidget(directoryComboBox2, 0, 2);
    mainLayout->addWidget(browseButton2, 0, 3);
    mainLayout->addWidget(validateButton1, 0, 4);

    mainLayout->addWidget(new QLabel(tr("PDF 1 text:")), 1, 0, 1, 1);
    mainLayout->addWidget(text1TextBrowser, 2, 0, 1, 2);
    mainLayout->addWidget(new QLabel(tr("PDF 2 text:")), 1, 2, 1, 1);
    mainLayout->addWidget(text2TextBrowser, 2, 2, 1, 2);

    connect(new QShortcut(QKeySequence::Quit, this), &QShortcut::activated,
            qApp, &QApplication::quit);
}

void Window::browse1()
{
    QString directory =
        QDir::toNativeSeparators(
                QFileDialog::getOpenFileName(this
                                             , tr("Bowse PDF file")
                                             , QDir::currentPath()
                                             , tr("PDF file (*.pdf)")));
    filename1 = directory;

    if (!directory.isEmpty())
    {
        if (directoryComboBox1->findText(directory) == -1)
            directoryComboBox1->addItem(directory);
        directoryComboBox1->setCurrentIndex(directoryComboBox1->findText(directory));
    }

    pdfReader.changePdfFilename(filename1);

    text1 = pdfReader.getTxt();
    text1TextBrowser->setText(text1);

    QFont serifFont("Times", 10);
    text1TextBrowser->setFont(serifFont);

}

void Window::browse2()
{
    QString directory =
        QDir::toNativeSeparators(
                QFileDialog::getOpenFileName(this
                                             , tr("Bowse PDF file")
                                             , QDir::currentPath()
                                             , tr("PDF file (*.pdf)")));
    filename2 = directory;

    if (!directory.isEmpty())
    {
        if (directoryComboBox2->findText(directory) == -1)
            directoryComboBox2->addItem(directory);
        directoryComboBox2->setCurrentIndex(directoryComboBox2->findText(directory));
    }

    pdfReader.changePdfFilename(filename2);

    text2 = pdfReader.getTxt();
    text2TextBrowser->setText(text2);

    QFont serifFont("Times", 10);
    text2TextBrowser->setFont(serifFont);
}

void Window::validate()
{
    QString error;
    plagiarismChecker = new PlagiarismChecker(text1TextBrowser->textCursor().selectedText()
                                              , text2
                                              , error);

    auto result = plagiarismChecker->getResultOfPlagiarismChecking();
    for(auto i : result)
        std::cout << "\n" << i.first << " : " << i.second << "\n";

    QTextCharFormat charFormat;
    charFormat.setBackground(Qt::red);
    QTextCursor cursor = text2TextBrowser->textCursor();
    cursor.select(QTextCursor::Document);


    QList<QTextEdit::ExtraSelection> extraSelections;
    for(auto i : result)
    {
        QTextEdit::ExtraSelection selection;
        selection.format = charFormat;
        selection.format.setToolTip(QString::number(i.first));
        selection.cursor = cursor;
        selection.cursor.clearSelection();
        selection.cursor.setPosition(i.first);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, i.second);
        selection.cursor.mergeCharFormat(selection.format);
        selection.cursor.setVisualNavigation(true);
        extraSelections.append(selection);
    }

    text2TextBrowser->setExtraSelections(extraSelections);
}

QComboBox *Window::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void Window::animateBrowseClick()
{
    browseButton1->animateClick();
}
