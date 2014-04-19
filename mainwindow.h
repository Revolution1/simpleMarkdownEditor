#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mdrenderer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void on_MarkdownSyntax_triggered();    
    void OpenExternalBrowser(const QUrl & url);
    void renderToView();
    void about();
    void open();
    bool save();
    bool saveAs();
    void newFile();
    bool exportAsHtml();
    void cssTriggered(QAction* action);
private:
    Ui::MainWindow *ui;
    void setupStylesheetActions();
    mdRenderer md;
    QString curFile;

    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
};

#endif // MAINWINDOW_H
