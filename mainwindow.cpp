#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->setInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[ABCDEabcde]"), this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushAButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "A");
    initCalculations();
}

void MainWindow::on_pushBButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "B");
    initCalculations();
}

void MainWindow::on_pushCButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "C");
    initCalculations();
}

void MainWindow::on_pushDButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "D");
    initCalculations();
}

void MainWindow::on_pushEButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "E");
    initCalculations();
}

void MainWindow::on_univButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "𝕌");
    initCalculations();
}

void MainWindow::on_emptyButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "Ø");
    initCalculations();
}

void MainWindow::generateRandomSet(QLineEdit *lineEdit)
{
    QString result = "{";
    int count = QRandomGenerator::global()->bounded(2, 9);
    for (int i = 0; i < count; i++)
    {
        result += QString("%1, ").arg(QRandomGenerator::global()->bounded(-10, 10));
    }
    if (count > 0)
    {
        result.chop(2);
    }
    result += "}";
    lineEdit->setText(result);
}

void MainWindow::on_randAButton_clicked()
{
    generateRandomSet(ui->setAInput);
}

void MainWindow::on_randBButton_clicked()
{
    generateRandomSet(ui->setBInput);
}

void MainWindow::on_randCButton_clicked()
{
    generateRandomSet(ui->setCInput);
}

void MainWindow::on_randDButton_clicked()
{
    generateRandomSet(ui->setDInput);
}

void MainWindow::on_randEButton_clicked()
{
    generateRandomSet(ui->setEInput);
}

void MainWindow::on_complementButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "¬");
}

void MainWindow::on_intersectionButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "⋂");
}

void MainWindow::on_unionButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "⋃");
}

void MainWindow::on_differenceButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "\\");
}

void MainWindow::on_symDifferenceButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text() + "∆");
}

void MainWindow::on_backspaceButton_clicked()
{
    ui->inputWindow->setText(ui->inputWindow->text().removeLast());
    initCalculations();
}

Set getSetByLetter(QWidget* widget, QString letter, bool isRaw=false)
{
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>(QString("set%1Input").arg(letter));
    if (lineEdit)
    {
        QString setStr = lineEdit->text();
        if (isRaw) return setStr;
        return Set(setStr);
    }
    return Set(QVector<int>{}, Empty);
}

QString complimentCheck(QString& text)
{
    int complementIndex = text.indexOf("¬");
    if (complementIndex == -1 || complementIndex + 1 >= text.length()) { return "NONE"; }
    QString complementedSetLetter = text[complementIndex + 1];
    text.remove(complementIndex, 2);
    return complementedSetLetter;
}

QString binaryCheck(QString& text, QString operation)
{
    int operationIndex = text.indexOf(operation);
    if (operationIndex + 1 >= text.length() || operationIndex - 1 < 0) { return "NONE"; }

    QString leftSetLetter = text[operationIndex - 1];
    QString rightSetLetter = text[operationIndex + 1];

    text.remove(operationIndex - 1, 3);

    return leftSetLetter + rightSetLetter;
}

void MainWindow::initCalculations()
{
    QString text = ui->inputWindow->text();
    if (text.length() < 1) qDebug() << Set(QVector<int>{}, Empty);
    if (text.length() == 1 && QString("ABCDEF").contains(text[0]))
    {
        qDebug() << getSetByLetter(ui->setsGroup, text[0]);
        return;
    }

    QRegularExpression regex("(¬*[ABCDE](¬*[\\⋃⋂\\\\∆]¬*[ABCDE])*)+");
    if (!regex.match(text).hasMatch()) { return; }

    QString fingerprint = text;
    for (QString c : QString("ABCDE"))
    {
        QString raw = getSetByLetter(ui->setsGroup, c, true).toString();
        fingerprint += raw.replace("¬", "1").replace("⋃", "2").replace("⋂", "3").replace("∆", "4").replace("\\", "5");
    }

    if (cache.contains(fingerprint))
    {
        qDebug() << "Результат из кэша:" << cache.value(fingerprint);
        return;
    }

    QString complementSet = complimentCheck(text);
    Set work = getSetByLetter(ui->setsGroup, complementSet);
    qDebug() << text;
    if (complementSet != "NONE")
    {
        work = work.complement();
    }
    qDebug() << text;

    QString intersectSets = binaryCheck(text, "⋂");
    if (intersectSets != "NONE")
    {
        work = getSetByLetter(ui->setsGroup, intersectSets[0])
                   .intersection(getSetByLetter(ui->setsGroup, intersectSets[1]));
    }
    qDebug() << text;

    QString unionSets = binaryCheck(text, "⋃");
    if (unionSets != "NONE")
    {
        work = getSetByLetter(ui->setsGroup, unionSets[0])
                   .unionOperation(getSetByLetter(ui->setsGroup, unionSets[1]));
    }
    qDebug() << text;

    QString symDifferenceSets = binaryCheck(text, "∆");
    if (symDifferenceSets != "NONE")
    {
        work = getSetByLetter(ui->setsGroup, symDifferenceSets[0])
                   .symDifference(getSetByLetter(ui->setsGroup, symDifferenceSets[1]));
    }
    qDebug() << text;

    QString differenceSets = binaryCheck(text, "\\");
    if (differenceSets != "NONE")
    {
        work = getSetByLetter(ui->setsGroup, differenceSets[0])
                   .difference(getSetByLetter(ui->setsGroup, differenceSets[1]));
    }
    qDebug() << text;

    cache.insert(fingerprint, work);
    qDebug() << work;

    if (regex.match(text).hasMatch()) initCalculations();
}
