#include "pdfreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

PdfReader::PdfReader(const std::string& pdfFilename)
    : pdfFileName(pdfFilename)
    , txtFileName(pdfFileName.substr(0,pdfFileName.size()-4)+".txt")
    , textFromTxt()
    , pdfToTextBashCommand("pdftotext ")
    , removeFileBashCommand("rm ")
    , isFirstWithThisPdf(true)
    , errorMessage()
{}

bool PdfReader::convertPdfToTxt()
{
    const std::string buff(pdfToTextBashCommand + pdfFileName);
    const int retValue = system(buff.c_str());

    if (retValue == -1 || WEXITSTATUS(retValue) != 0)
    {
        return false;
    }

    return true;
}

bool PdfReader::readFromTxt()
{
    std::ifstream myReadFile;
    myReadFile.open(txtFileName);

    if (!myReadFile.is_open())
    {
      return false;
    }
    else
    {
        std::string line;

        while (std::getline(myReadFile, line))
        {
            textFromTxt.append(line + "\n");
        }
    }

    myReadFile.close();
    return true;
}

bool PdfReader::removeTxtFile()
{
    const std::string buff(removeFileBashCommand + txtFileName);
    const int retValue = system(buff.c_str());

    if (retValue == -1 || WEXITSTATUS(retValue) != 0)
    {
        return false;
    }

    return true;
}

const std::string &PdfReader::getTxt()
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

void PdfReader::changePdfFilename(const std::string& pdfFilename)
{
    pdfFileName = pdfFilename;
    txtFileName = pdfFileName.substr(0,pdfFileName.size()-4)+".txt";
    isFirstWithThisPdf = true;
}

const std::string& PdfReader::getPdfFileName()
{
    return pdfFileName;
}
