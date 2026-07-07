#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMap>
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_dirButton_clicked();
    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;
    void buildIndex(const QString &directoryPath);
    QString processQuery(const QString &query);

    // Index data
    QMap<QString, QMap<QString, QSet<int>>> index; // word -> filename -> set of line numbers
    QMap<QString, QString> fileContents; // filename -> full content
};

#endif // MAINWINDOW_H
