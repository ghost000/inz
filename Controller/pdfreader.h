#pragma once
#include <QString>

class PdfReader
{

public:
    PdfReader(const QString& pdfFilename);
    ~PdfReader() {}

    const QString& getTxt();
    void           changePdfFilename(const QString& pdfFilename);
    const QString& getPdfFileName();

private:
    bool convertPdfToTxt();
    bool readFromTxt();
    bool removeTxtFile();

    QString       pdfFileName;
    QString       txtFileName;
    QString       textFromTxt;
    const QString pdfToTextBashCommand;
    const QString removeFileBashCommand;
    bool          isFirstWithThisPdf;
    QString       errorMessage;
};

