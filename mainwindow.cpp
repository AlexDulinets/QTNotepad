#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , textEdit(new QPlainTextEdit)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(textEdit);
    setWindowTitle("Новый файл");
    QMenu * fileMenu = menuBar() ->addMenu(tr("&Файл"));
    QToolBar *fileToolBar = addToolBar(tr("Файл"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":newLumine.png"));
    QAction *newAct = new QAction(newIcon, tr("&Новый файл"), this);
    newAct->setStatusTip(tr("Создать новый файл"));
    connect(newAct, &QAction::triggered, this, &MainWindow::NewFile);
    fileMenu->addAction(newAct);
    fileToolBar -> addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-new", QIcon(":openLenochka.png"));
    QAction *openAct = new QAction (openIcon, tr("&Вскрыть"), this);
    openAct -> setStatusTip (tr("Вскрыть файл"));
    connect(openAct, &QAction::triggered, this, &MainWindow::Open);
    fileMenu -> addAction (openAct);
    fileToolBar -> addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-new", QIcon(":saveRaiden.png"));
    saveAct = new QAction (saveIcon, tr("&Сохранить"), this);
    saveAct -> setStatusTip (tr("Сохранить"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::Save);
    fileMenu -> addAction (saveAct);
    fileToolBar -> addAction(saveAct);

    saveAct->setEnabled(false);

    connect(textEdit,SIGNAL(textChanged()),this,SLOT(vkl()));

    QMenu *helpMenu = menuBar()->addMenu(tr("&О программе"));

     const QIcon VersionIcon = QIcon::fromTheme("document-new", QIcon(":version.png"));
     QAction * programAct = new QAction(VersionIcon,tr("Версия"),this);
     connect (programAct,&QAction::triggered, this,&MainWindow::Program);
     programAct->setStatusTip(tr("Показывает версию программы"));
     helpMenu -> addAction(programAct);

     //const QIcon AuthorIcon = QIcon::fromTheme("document-new", QIcon(":author.png"));
     const QIcon AuthorIcon = QIcon::fromTheme("document-new", QIcon(":DIO.png"));
     QAction * authorAct = new QAction(AuthorIcon,tr("Автор"), this);
     connect (authorAct,&QAction::triggered, this,&MainWindow::Author);
     authorAct->setStatusTip(tr("Показывает автора"));
     helpMenu -> addAction(authorAct);

     QAction *saveasAct = new QAction (tr("Сохранить &как..."), this);
     saveasAct -> setShortcuts (QKeySequence::SaveAs);
     connect(saveasAct, &QAction::triggered, this, &MainWindow::SaveAs);
     saveasAct -> setStatusTip (tr("Сохранить в выбранную папку"));
     fileMenu -> addAction (saveasAct);
     fileToolBar->addSeparator();

     const QIcon undoIcon = QIcon::fromTheme("document-new", QIcon(":undo.png"));
     QAction * undoAct = new QAction(undoIcon,tr("Отменить"), this);
     connect (undoAct,&QAction::triggered, this,&MainWindow::undo);
     undoAct->setStatusTip(tr("Отмена последнего действия"));
     fileToolBar -> addAction(undoAct);

     const QIcon redoIcon = QIcon::fromTheme("document-new", QIcon(":redo.png"));
     QAction * redoAct = new QAction(redoIcon,tr("Повторить"), this);
     connect (redoAct,&QAction::triggered, this,&MainWindow::redo);
     redoAct->setStatusTip(tr("Повтор последнего действия"));
     fileToolBar -> addAction(redoAct);
     fileToolBar->addSeparator();

     const QIcon inIcon = QIcon::fromTheme("document-new", QIcon(":plusChilde.png"));
     QAction * inAct = new QAction(inIcon,tr("Увеличить масштаб"), this);
     connect (inAct,&QAction::triggered, this,&MainWindow::zoomIn);
     inAct->setStatusTip(tr("Увеличивает масштаб"));
     fileToolBar -> addAction(inAct);

     const QIcon outIcon = QIcon::fromTheme("document-new", QIcon(":minusKeqing.png"));
     QAction * outAct = new QAction(outIcon,tr("Уменьшить масштаб"), this);
     connect (outAct,&QAction::triggered, this,&MainWindow::zoomOut);
     outAct->setStatusTip(tr("Уменьшает масштаб"));
     fileToolBar -> addAction(outAct);

     const QIcon fontIcon = QIcon::fromTheme("document-new", QIcon(":font.png"));
     QAction * fontAct = new QAction(fontIcon,tr("Изменить шрифт"), this);
     connect (fontAct,&QAction::triggered, this,&MainWindow::selectFont);
     fontAct->setStatusTip(tr("Изменяет шрифт"));
     fileToolBar -> addAction(fontAct);
}

MainWindow::~MainWindow()
{

    delete ui;
}



void MainWindow::NewFile()

{
    if (IfSaved()) {
        textEdit->clear();
        setFile(QString());
    }

}

void MainWindow::setFile (const QString &fileName) {

    cFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = cFile;
    if (cFile.isEmpty())
        shownName = "Новый файл";
    QStringList parts = shownName.split("/");
    QString name = parts.at(parts.size()-1);
    setWindowTitle(name);
    saveAct->setEnabled(false);

}

void MainWindow::lFile(const QString &fileName)

{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {

        QMessageBox::warning(this, tr("Программа"),
                             tr("Невозможно прочитать файл %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setFile(fileName);
    statusBar()->showMessage(tr("Файл загружен"), 2000);
    saveAct->setEnabled(false);
}

void MainWindow::Open(){
    if(IfSaved()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            lFile(fileName);
    }
}

bool MainWindow::SaveFile(const QString &fileName){

    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Невозможно редактировать файл %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Невозможно открыть файл %1 для редактирования:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Программа"), errorMessage);
        return false;
    }

    setFile(fileName);
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
    saveAct->setEnabled(false);
    return true;
}

bool MainWindow::Save()
{
    if(cFile.isEmpty())
    return SaveAs();
    else
        return SaveFile(cFile);

}
bool MainWindow::SaveAs(){
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return SaveFile(dialog.selectedFiles().first());
}
bool MainWindow::IfSaved(){
    if (!textEdit->document()->isModified())
        return true;
QMessageBox::StandardButton msgbox  = QMessageBox::warning(this, tr("Внимание"),tr("Этот файл уже изменён.\n" "Не хотите сохранить файл"), QMessageBox::Save | QMessageBox::Discard |QMessageBox::Cancel);
switch (msgbox) {
case QMessageBox::Save:
    return Save();
case QMessageBox::Cancel:
    return false;
default:
    break;
}
return true;

}

void MainWindow::Program(){
    QMessageBox::about(this ,"Версия","Версия программы: 0.0.1.");
    }
void MainWindow::Author(){
    QMessageBox::about(this,"Автор","Автор программы: Дулинец Александр.");
    }

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (IfSaved()) {
        event->accept();
    } else {
        event->ignore();
    }
}
void MainWindow::vkl()
{
    saveAct->setEnabled(true);
}

void MainWindow::zoomIn(){
    textEdit->zoomIn(2);
}
void MainWindow::zoomOut(){
    textEdit->zoomOut(2);
}
void MainWindow::undo(){
    textEdit->undo();
}
void MainWindow::redo(){
    textEdit->redo();
}
void MainWindow::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        textEdit->setFont(font);
}
