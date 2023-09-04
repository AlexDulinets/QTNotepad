#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFontDialog>
#include <QToolBar>
#include <QFile>
#include <QSaveFile>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
      void closeEvent(QCloseEvent *event) override;

private:
    QPlainTextEdit *textEdit;
    QAction * saveAct;



    QString cFile;
    Ui::MainWindow *ui;
private slots:
    bool SaveFile(const QString &fileName);
    void setFile(const QString &fileName);
    bool Save();
    bool SaveAs();
    bool IfSaved();
    void lFile(const QString &fileName);
    void Open();
    void NewFile();
    void Program();
    void Author();
    void vkl();
    void zoomIn();
    void zoomOut();
    void undo();
    void redo();
    void selectFont();
};
#endif // MAINWINDOW_H
