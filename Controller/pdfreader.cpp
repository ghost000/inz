#include "pdfreader.h"

#include <cstdlib>
#include <QFile>

PdfReader::PdfReader(const QString& pdfFilename)
    : pdfFileName(pdfFilename)
    , txtFileName(pdfFilename.mid(0,pdfFilename.size()-5) + ".txt")
    , textFromTxt()
    , pdfToTextBashCommand("pdftotext -enc UTF-8 -layout ")
    , removeFileBashCommand("rm -- ")
    , isFirstWithThisPdf(true)
    , errorMessage()
{}

int PdfReader::executeBashCommand(const QString& bashCommand, const QString& fileName)
{
    return std::system(QString(bashCommand + fileName).toLocal8Bit().data());
}

bool PdfReader::wasBashCommandCorrectlyExecuted(int retValue)
{
    if (retValue == -1 || WEXITSTATUS(retValue) != 0)
    {
        return false;
    }
    return true;
}

bool PdfReader::convertPdfToTxt()
{
    return wasBashCommandCorrectlyExecuted(executeBashCommand(pdfToTextBashCommand, pdfFileName));
}

bool PdfReader::readFromTxt()
{
    QFile file(txtFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    while (!file.atEnd())
    {
        textFromTxt.append(file.readLine());
    }

    file.close();

    return true;
}

bool PdfReader::removeTxtFile()
{
    txtFileName.push_front("\"");
    txtFileName.push_back("\"");
    return wasBashCommandCorrectlyExecuted(executeBashCommand(removeFileBashCommand, txtFileName));
}

const QString& PdfReader::getTxt()
{
    textFromTxt.clear();

    if(isFirstWithThisPdf)
    {
        isFirstWithThisPdf = false;
        errorMessage.clear();

        if(!convertPdfToTxt())
        {
            errorMessage += " convert Pdf To Txt was ended in failure \n";
        }

        if(!readFromTxt())
        {
            errorMessage += " read from Txt was ended in failure \n";
        }

        if(!removeTxtFile())
        {
            errorMessage += " remove Txt file was ended in failure \n";
        }
    }

    if(errorMessage.isEmpty())
    {
        return textFromTxt;
    }

    return errorMessage;
}

void PdfReader::changePdfFilename(const QString& pdfFilename)
{
    pdfFileName = pdfFilename;
    pdfFileName.replace(" ", "\\\ ");
    txtFileName = pdfFilename.mid(0, pdfFilename.size()-4) + ".txt";
    isFirstWithThisPdf = true;
}

const QString& PdfReader::getPdfFileName()
{
    return pdfFileName;
}
