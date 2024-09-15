#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QRandomGenerator>
#include <QLineEdit>
#include "set.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushAButton_clicked();
    void on_pushBButton_clicked();
    void on_pushCButton_clicked();
    void on_pushDButton_clicked();
    void on_pushEButton_clicked();
    void on_univButton_clicked();
    void on_emptyButton_clicked();
    void on_randAButton_clicked();
    void on_randBButton_clicked();
    void on_randCButton_clicked();
    void on_randDButton_clicked();
    void on_randEButton_clicked();
    void on_complementButton_clicked();
    void on_intersectionButton_clicked();
    void on_unionButton_clicked();
    void on_differenceButton_clicked();
    void on_symDifferenceButton_clicked();
    void on_backspaceButton_clicked();
    void on_clearButton_clicked();
    void on_setAInput_textChanged();
    void on_setBInput_textChanged();
    void on_setCInput_textChanged();
    void on_setDInput_textChanged();
    void on_setEInput_textChanged();
    void on_elemInput_textChanged(const QString);
    void on_setInput_textChanged(const QString);

private:
    Ui::MainWindow *ui;
    void initCalculations();
    void generateRandomSet(QLineEdit *lineEdit);
    QMap<QString, Set> cache;
    void updateOutput(Set set);
    bool checkElemInSet(int elem, Set set)
    { return set.getValues().contains(elem); };
    bool checkSubsetInSet(Set subset, Set set);
};
#endif // MAINWINDOW_H
