#include "window.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
    , pdfReader("")
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

    connect(browseButton1, &QAbstractButton::clicked, this, [this]()
    {
        Window::browse(text1TextBrowser, directoryComboBox1);
    });

    connect(browseButton2, &QAbstractButton::clicked, this, [this]()
    {
        Window::browse(text2TextBrowser, directoryComboBox2);
    });

    connect(validateButton1, &QAbstractButton::clicked, this, &Window::validate);

    connect(this, SIGNAL(valueTextChanged(const QString&, const QString&)), &plagiarismChecker, SLOT(setValue(const QString&, const QString&)));
    connect(&plagiarismChecker, SIGNAL(resultReady(QVector<QPair<int, int>> &)), this, SLOT(handleResults(QVector<QPair<int, int>> &)));

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

    //ale moloch
}

void Window::browse(QTextBrowser* textTextBrowser, QComboBox* directoryComboBox)
{
    QString directory =
            QDir::toNativeSeparators(
                QFileDialog::getOpenFileName(this
                                             , tr("Bowse PDF file")
                                             , QDir::currentPath()
                                             , tr("PDF file (*.pdf)")));
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
    if(text1TextBrowser->toPlainText().isEmpty() || text2TextBrowser->toPlainText().isEmpty())
    {
        showMsgBox("Please select two pdf files !!!");
    }
    else if(text1TextBrowser->textCursor().selectedText().isEmpty())
    {
        showMsgBox("Please select text in first pdf file !!!");
    }
    else
    {
        emit valueTextChanged(text1TextBrowser->textCursor().selectedText(), text2TextBrowser->toPlainText());
    }
}

QComboBox *Window::createComboBox(const QString &text)
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
        }
    }

    showMsgBox(info);
}

void Window::textBrowserUpdate(const QVector<QPair<int, int>>& result)
{
    text2TextBrowser->textCursor().clearSelection();
    QTextCharFormat charFormat;
    charFormat.setBackground(Qt::red);

    QList<QTextEdit::ExtraSelection> extraSelections;
    for(const auto& i : result)
    {
        QTextEdit::ExtraSelection selection;
        selection.format = charFormat;
        selection.cursor = text2TextBrowser->textCursor();
        selection.cursor.select(QTextCursor::Document);
        selection.cursor.setPosition(i.first);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, i.second);
        extraSelections.append(selection);
    }

    text2TextBrowser->setExtraSelections(extraSelections);
}

void Window::showMsgBox(const QString& textToShow)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Antyplagiat"));
    msgBox.setText(textToShow);
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.exec();
}
