#pragma once

#include <string>

class PdfReader
{

public:
    PdfReader(const std::string& pdfFilename);
    ~PdfReader() {}

    const std::string& getTxt();
    void               changePdfFilename(const std::string& pdfFilename);
    const std::string& getPdfFileName();

private:
    bool convertPdfToTxt();
    bool readFromTxt();
    bool removeTxtFile();

    std::string       pdfFileName;
    std::string       txtFileName;
    std::string       textFromTxt;
    const std::string pdfToTextBashCommand;
    const std::string removeFileBashCommand;
    bool              isFirstWithThisPdf;
    std::string       errorMessage;
};

