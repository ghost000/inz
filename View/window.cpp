#include "window.h"

#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QThread>
#include <QFuture>

Window::Window(QWidget *parent)
    : QWidget(parent)
    , pdfReader("")
    //, plagiarismChecker(PlagiarismChecker("",""))
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

    connect(directoryComboBox1->lineEdit(), &QLineEdit::returnPressed, this, &Window::animateBrowseClick);
    connect(directoryComboBox2->lineEdit(), &QLineEdit::returnPressed, this, &Window::animateBrowseClick);

    connect(this, &Window::valueChanged, plagiarismChecker, &PlagiarismChecker::setValue);
    connect(plagiarismChecker, &PlagiarismChecker::resultReady, this, &Window::handleResults);

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

    pdfReader.changePdfFilename(directory);

    textTextBrowser->setText(pdfReader.getTxt());
}

void Window::validate()
{
    if(text1TextBrowser->textCursor().selectedText().isEmpty() || text2TextBrowser->toPlainText().isEmpty())
    {
        text2TextBrowser->setText("Empty !!!");
    }
    else
    {
        emit valueChanged(text1TextBrowser->textCursor().selectedText(), text2TextBrowser->toPlainText());
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

void Window::handleResults(const QVector<QPair<int, int>>& result)
{
    wasPatternFoundQMessageBoxInfo(result);
    textBrowserUpdate(result);
}

void Window::wasPatternFoundQMessageBoxInfo(const QVector<QPair<int, int>>& result)
{
    QMessageBox msgBox;
    QString     info = "The pattern was";

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

    msgBox.setText(info);
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.exec();
}

void Window::textBrowserUpdate(const QVector<QPair<int, int>>& result)
{
    text2TextBrowser->textCursor().clearSelection();

    QList<QTextEdit::ExtraSelection> extraSelections;
    for(const auto& i : result)
    {
        QTextEdit::ExtraSelection selection;
        selection.format = QTextCharFormat().setBackground(Qt::Red);
        selection.cursor = text2TextBrowser->textCursor();
        selection.cursor.select(QTextCursor::Document);
        selection.cursor.setPosition(i.first);
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, i.second);
        extraSelections.append(selection);
    }

    text2TextBrowser->setExtraSelections(extraSelections);
}
