#pragma once
#include <QString>

class PdfReader
{
public:
    explicit PdfReader(const QString& pdfFilename);
    ~PdfReader() = default;

    const QString& getTxt();
    void           changePdfFilename(const QString& pdfFilename);
    const QString& getPdfFileName();

private:
    int  executeBashCommand(const QString& bashCommand, const QString& fileName);
    bool wasBashCommandCorrectlyExecuted(int retValue);
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

