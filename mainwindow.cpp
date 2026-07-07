#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Mini Search Engine");
    ui->resultTextEdit->setReadOnly(true);
    ui->queryTextEdit->setPlaceholderText("Enter number of queries first\nThen one query per line");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dirButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Select Directory");
    if (!directory.isEmpty()) {
        ui->dirLineEdit->setText(directory);
        buildIndex(directory);
    }
}

void MainWindow::buildIndex(const QString &directoryPath)
{
    index.clear();
    fileContents.clear();

    QDir directory(directoryPath);
    QStringList files = directory.entryList(QDir::Files, QDir::Name);

    for (const QString &fileName : files) {
        QFile file(directory.filePath(fileName));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }

        QTextStream in(&file);
        QString content;
        int lineNumber = 0;

        while (!in.atEnd()) {
            QString line = in.readLine();
            content += line + "\n";
            QStringList words = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

            for (const QString &word : words) {
                QString lowerWord = word.toLower();
                index[lowerWord][fileName].insert(lineNumber);
            }

            lineNumber++;
        }

        fileContents[fileName] = content.trimmed();
        file.close();
    }
}

void MainWindow::on_searchButton_clicked()
{
    QString queriesText = ui->queryTextEdit->toPlainText().trimmed();
    if (queriesText.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter queries first.");
        return;
    }

    QStringList lines = queriesText.split('\n', Qt::SkipEmptyParts);
    if (lines.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Invalid query format.");
        return;
    }

    bool ok;
    int queryCount = lines[0].toInt(&ok);
    if (!ok || queryCount <= 0 || queryCount + 1 > lines.size()) {
        QMessageBox::warning(this, "Warning", "First line should be a valid number of queries.");
        return;
    }

    QString result;
    for (int i = 1; i <= queryCount && i < lines.size(); i++) {
        QString query = lines[i].trimmed();
        if (query.isEmpty()) continue;

        QString queryResult = processQuery(query);
        result += queryResult + "\n";
    }

    ui->resultTextEdit->setPlainText(result.trimmed());
}

QString MainWindow::processQuery(const QString &query)
{
    QStringList words = query.split(' ', Qt::SkipEmptyParts);
    if (words.isEmpty()) {
        return "Invalid query";
    }

    // Convert all words to lowercase
    for (QString &word : words) {
        word = word.toLower();
    }

    // Find files that contain all query words
    QMap<QString, int> fileMatchCounts; // filename -> match count
    QMap<QString, QSet<int>> matchedLines; // filename -> set of line numbers

    for (const QString &word : words) {
        if (!index.contains(word)) {
            // If any word is not found, no files will match
            fileMatchCounts.clear();
            break;
        }

        const QMap<QString, QSet<int>> &filesForWord = index[word];

        if (fileMatchCounts.isEmpty()) {
            // First word - initialize with all files containing this word
            for (auto it = filesForWord.begin(); it != filesForWord.end(); ++it) {
                fileMatchCounts[it.key()] = 1;
                matchedLines[it.key()].unite(it.value());
            }
        } else {
            // Subsequent words - intersect with existing files
            QMap<QString, int> newFileMatchCounts;
            QMap<QString, QSet<int>> newMatchedLines;

            for (auto it = filesForWord.begin(); it != filesForWord.end(); ++it) {
                const QString &filename = it.key();
                if (fileMatchCounts.contains(filename)) {
                    newFileMatchCounts[filename] = fileMatchCounts[filename] + 1;
                    newMatchedLines[filename].unite(matchedLines[filename]);
                    newMatchedLines[filename].unite(it.value());
                }
            }

            fileMatchCounts = newFileMatchCounts;
            matchedLines = newMatchedLines;

            if (fileMatchCounts.isEmpty()) {
                break; // No files match all words so far
            }
        }
    }

    // Prepare the result string
    QString result;
    int totalFiles = fileMatchCounts.size();

    if (totalFiles == 0) {
        result = "0\nNot Found";
    } else {
        result = QString::number(totalFiles);

        // Sort files by match count (descending) and then by filename (ascending)
        QList<QString> sortedFiles = fileMatchCounts.keys();
        std::sort(sortedFiles.begin(), sortedFiles.end(), [&](const QString &a, const QString &b) {
            if (fileMatchCounts[a] != fileMatchCounts[b]) {
                return fileMatchCounts[a] > fileMatchCounts[b];
            }
            return a < b;
        });

        // Add matched lines for each file
        for (const QString &filename : sortedFiles) {
            result += "\n" + filename;

            QList<int> linesList = matchedLines[filename].values();
            std::sort(linesList.begin(), linesList.end());

            QStringList fileLines = fileContents[filename].split('\n');
            for (int lineNum : linesList) {
                if (lineNum >= 0 && lineNum < fileLines.size()) {
                    result += "\n" + fileLines[lineNum];
                }
            }
        }
    }

    return result;
}
