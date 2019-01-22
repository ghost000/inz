#include "pdfreader.h"

#include <stdio.h>
#include <stdlib.h>
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

bool PdfReader::convertPdfToTxt()
{
    const QString buff(pdfToTextBashCommand + pdfFileName);
    const QByteArray ba = buff.toLocal8Bit();
    const char *c_str = ba.data();
    const int retValue = system(c_str);

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
        QString line = file.readLine();
        textFromTxt.append(line);
    }

    file.close();
    return true;
}

bool PdfReader::removeTxtFile()
{
    const QString buff(removeFileBashCommand + txtFileName);
    const QByteArray ba = buff.toLocal8Bit();
    const char *c_str = ba.data();
    const int retValue = system(c_str);

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
