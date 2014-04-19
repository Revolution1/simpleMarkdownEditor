#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QDialog>
#include <QUrl>
#include <QFile>
#include <QtWebKitWidgets/QWebView>
#include <QWebPage>
#include <QWebPage>
#include <QWebFrame>
#include <QString>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QStringList>
#include <QFileDialog>
#include <QAction>
#include <QEvent>
#include <QMimeData>
#include "mdrenderer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->hLayout);
    mdRenderer rdr("#Title \n test");
    qDebug()<<rdr.read();
    renderToView();
    setupStylesheetActions();
    connect(ui->mdEdit,SIGNAL(textChanged()),this,SLOT(renderToView()));

    connect(ui->about,SIGNAL(triggered()),this,SLOT(about()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_MarkdownSyntax_triggered()
{
    //QDesktopServices::openUrl(QUrl("http://wowubuntu.com/markdown/basic.html"));

    QFile infi("://markdownSyntax.md");
    infi.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data=infi.readAll();
    infi.close();

    //    mdRenderer md;
    md.render(data);
    QDialog *s =new QDialog(this);
    s->setWindowTitle(tr("Markdown语法教程"));

    QVBoxLayout *v =new QVBoxLayout(s);

    QWebView  *wv=new QWebView();
    v->addWidget(wv);
    wv->setHtml(md.read());
    wv->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
    connect(wv,SIGNAL(linkClicked(QUrl)),this,SLOT(OpenExternalBrowser(QUrl)));

    QPushButton *bt=new QPushButton(tr("确定"));
    v->addWidget(bt,0,Qt::AlignRight);
    connect(bt,SIGNAL(clicked()),s,SLOT(close()));

    s->show();
}

void MainWindow::OpenExternalBrowser(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

void MainWindow::renderToView()
{
    QString text=ui->mdEdit->toPlainText();
    md.render(text);
    ui->view->setHtml(md.read());
}

void MainWindow::about()
{
    QMessageBox::aboutQt(this);
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),strippedName(windowFilePath()),tr("Markdown 文档 (*.md *.markdown);;任何文件 (*.*)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);

}

void MainWindow::newFile()
{
    if (maybeSave()) {
        ui->mdEdit->clear();
        setCurrentFile("");
    }
}

bool MainWindow::exportAsHtml()
{
    QString fileName= QFileDialog::getSaveFileName(this,tr("保存为HTML"),strippedName(windowFilePath()),tr("HTML 文档 (*.html)"));
    if (fileName.isEmpty())
        return false;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("错误"),
                             tr("无法写入文件 %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << ui->view->page()->currentFrame()->toHtml();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    ui->statusBar->showMessage(tr("文件已导出"), 2000);
    return true;
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,tr("打开"),QFileInfo(windowFilePath()).absoluteFilePath(),tr("Markdown 文档 (*.md *.markdown);;任何文件 (*.*)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::maybeSave()
{
    if (ui->mdEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("保存"),
                                   tr("文档的内容已经更改\n"
                                      "是否保存？"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("错误"),
                             tr("无法读取文件 %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

//    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->mdEdit->setPlainText(file.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    ui->statusBar->showMessage(tr("文件已载入"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("错误"),
                             tr("无法写入文件 %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

//    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
//    out << ui->mdEdit->toPlainText().toUtf8();
    file.write(ui->mdEdit->toPlainText().toUtf8());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    ui->statusBar->showMessage(tr("文件已保存"), 2000);
    return true;
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    ui->mdEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowTitle(shownName+" -Qt Markdown Editor");
    setWindowFilePath(shownName);
}

void MainWindow::setupStylesheetActions()
{
    QActionGroup *cssActions=new QActionGroup(ui->styleMenu);
    QString css="github,byword-dark,clearness,clearness-dark,markdown,solarized-dark,solarized-light";
    QStringList csses=css.split(",");
    for (int i=0;i<csses.size();i++)
    {
        QAction *action=new QAction(this);
        action->setText(csses.at(i));
        action->setCheckable(true);
        if(!i)
            action->setChecked(true);
        cssActions->addAction(action);
    }
    connect(cssActions,SIGNAL(triggered(QAction*)),this,SLOT(cssTriggered(QAction*)));
    ui->styleMenu->addActions(cssActions->actions());

}

void MainWindow::cssTriggered(QAction* action)
{
    qDebug()<<"style set to :"<<action->text();
    md.setStyle("://styles/"+action->text()+".css");
    renderToView();
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
    qDebug()<<"dragEnter\n";
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }
    loadFile(fileName);
}
