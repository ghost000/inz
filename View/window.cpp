#include "window.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
    , pdfReader("")
    , plagiarismChecker(PlagiarismChecker("",""))
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
        Window::browse(filename1, text1, text1TextBrowser, directoryComboBox1);
    });

    connect(browseButton2, &QAbstractButton::clicked, this, [this]()
    {
        Window::browse(filename2, text2, text2TextBrowser, directoryComboBox2);
    });
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

void Window::browse(QString& filename, QString& text, QTextBrowser* textTextBrowser, QComboBox* directoryComboBox)
{
    QString directory =
        QDir::toNativeSeparators(
            QFileDialog::getOpenFileName(this
                                         , tr("Bowse PDF file")
                                         , QDir::currentPath()
                                         , tr("PDF file (*.pdf)")));
    filename = directory;

    if (!directory.isEmpty())
    {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }

    pdfReader.changePdfFilename(filename);

    text = pdfReader.getTxt();
    textTextBrowser->setText(text);
}

void Window::validate()
{
    text2TextBrowser->textCursor().clearSelection();

    plagiarismChecker.setPattern(text1TextBrowser->textCursor().selectedText());
    plagiarismChecker.setText(text2);

    QString error = plagiarismChecker.validate();
    if(error != "")
    {
        text2TextBrowser->setText(error);
    }
    else
    {
        plagiarismChecker.checker();
        auto result = plagiarismChecker.getResultOfPlagiarismChecking();

        if(result.empty())
        {
            QMessageBox msgBox;
            msgBox.setText("The pattern was not found.");
            msgBox.setStandardButtons(QMessageBox::Cancel);
            msgBox.exec();
        }
        else
        {

            QMessageBox msgBox;
            QString string = "The pattern was found at positions nr: ";
            for(auto i : result)
            {
                string += "\n";
                string += QString::number(i.first);
            }
            msgBox.setText(string);
            msgBox.setStandardButtons(QMessageBox::Cancel);
            msgBox.exec();
            QTextCharFormat charFormat;
            charFormat.setBackground(Qt::red);
            QTextCursor cursor = text2TextBrowser->textCursor();
            cursor.select(QTextCursor::Document);

            QList<QTextEdit::ExtraSelection> extraSelections;
            for(auto i : result)
            {
                QTextEdit::ExtraSelection selection;
                selection.format = charFormat;
                selection.cursor = cursor;
                selection.cursor.setPosition(i.first);
                selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, i.second);
                extraSelections.append(selection);
            }
            text2TextBrowser->setExtraSelections(extraSelections);
        }
    }
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
{}
