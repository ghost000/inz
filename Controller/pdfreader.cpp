#include "pdfreader.h"

#include <cstdlib>
#include <QFile>

PdfReader::PdfReader(const QString& pdfFilename)
    : pdfFileName(pdfFilename)
    , txtFileName(pdfFilename.mid(0,pdfFileName.size()-4) + ".txt")
    , textFromTxt()
    , pdfToTextBashCommand("pdftotext -enc UTF-8 -layout ")
    , removeFileBashCommand("rm ")
    , isFirstWithThisPdf(true)
    , errorMessage()
{}

int PdfReader::executeBashCommand(const QString& bashCommand, const QString& fileName)
{
    const QString buff(bashCommand + fileName);
    const QByteArray ba = buff.toLocal8Bit();
    const char *c_str = ba.data();
    return std::system(c_str);
}

bool PdfReader::convertPdfToTxt()
{
    const int retValue = executeBashCommand(pdfToTextBashCommand, pdfFileName);

    if (retValue == -1 || WEXITSTATUS(retValue) != 0)
    {
        return false;
    }

    return true;
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
    const int retValue = executeBashCommand(removeFileBashCommand, txtFileName);

    if (retValue == -1 || WEXITSTATUS(retValue) != 0)
    {
        return false;
    }

    return true;
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

    if(errorMessage == "")
    {
        return textFromTxt;
    }

    return errorMessage;
}

void PdfReader::changePdfFilename(const QString& pdfFilename)
{
    pdfFileName = pdfFilename;
    txtFileName = pdfFilename.mid(0, pdfFileName.size()-4) + ".txt";
    isFirstWithThisPdf = true;
}

const QString& PdfReader::getPdfFileName()
{
    return pdfFileName;
}
