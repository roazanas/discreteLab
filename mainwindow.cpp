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

void MainWindow::updateOutput(Set set)
{

    ui->outputWindow->setText(set.toString(true));
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
    if (letter == "O")
        return Set(QVector<int>{}, Empty);
    if (letter == "U")
        return Set(QVector<int>{}, Universum);
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>(QString("set%1Input").arg(letter));
    if (lineEdit)
    {
        QString setStr = lineEdit->text();
        if (isRaw) return setStr;
        return Set(setStr);
    }
    return Set(QVector<int>{}, Empty);
}

// QString complimentCheck(QString& text)
// {
//     int complementIndex = text.indexOf("¬");
//     if (complementIndex == -1 || complementIndex + 1 >= text.length()) { return "NONE"; }
//     QString complementedSetLetter = text[complementIndex + 1];
//     text.remove(complementIndex, 2);
//     return complementedSetLetter;
// }

// QString binaryCheck(QString& text, QString operation)
// {
//     int operationIndex = text.indexOf(operation);
//     if (operationIndex + 1 >= text.length() || operationIndex - 1 < 0) { return "NONE"; }

//     QString leftSetLetter = text[operationIndex - 1];
//     QString rightSetLetter = text[operationIndex + 1];

//     text.remove(operationIndex - 1, 3);

//     return leftSetLetter + rightSetLetter;
// }

void MainWindow::initCalculations()
{
    QString text = ui->inputWindow->text().replace("𝕌", "U").replace("Ø", "O");
    if (text.length() < 1)
    {
        qDebug() << Set(QVector<int>{}, Empty);
        updateOutput(Set(QVector<int>{}, Empty));
        return;
    }
    if (text.length() == 1 && QString("ABCDEFUO").contains(text[0]))
    {
        qDebug() << getSetByLetter(ui->setsGroup, text[0]);
        updateOutput(getSetByLetter(ui->setsGroup, text[0]));
        return;
    }

    QStringList elements;

    for (int i = 0; i < text.length(); ++i)
    {
        QChar current = text[i];

        if (QString("ABCDEUO").contains(current))
        {
            Set currentSet = getSetByLetter(ui->setsGroup, QString(current));
            elements.append(currentSet.toString());
        }
        else if (QString("¬⋂⋃∆\\").contains(current))
        {
            elements.append(QString(current));
        }
    }

    qDebug() << elements;
    QString fingerprint = elements.join("");

    if (cache.contains(fingerprint))
    {
        qDebug() << "Результат из кэша:" << cache.value(fingerprint);
        updateOutput(cache.value(fingerprint));
        return;
    }

    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i] == "¬")
        {
            if (i + 1 < elements.size())
            {
                Set set = Set(elements[i + 1]);
                elements[i + 1] = set.complement().toString();
                elements.removeAt(i);
                --i;
            }
        }
    }

    QStringList binaryOperations = {"⋂", "⋃", "∆", "\\"};

    for (const QString &operation : binaryOperations)
    {
        for (int i = 0; i < elements.size(); ++i)
        {
            if (elements[i] == operation && i > 0 && i < elements.size() - 1)
            {
                Set leftSet = Set(elements[i - 1]);
                Set rightSet = Set(elements[i + 1]);
                Set resultSet;

                if (operation == "⋂")
                {
                    resultSet = leftSet.intersection(rightSet);
                }
                else if (operation == "⋃")
                {
                    resultSet = leftSet.unionOperation(rightSet);
                }
                else if (operation == "∆")
                {
                    resultSet = leftSet.symDifference(rightSet);
                }
                else if (operation == "\\")
                {
                    resultSet = leftSet.difference(rightSet);
                }

                elements[i - 1] = resultSet.toString();
                elements.removeAt(i + 1);
                elements.removeAt(i);
                --i;
                qDebug() << elements;
            }
        }
    }
    cache.insert(fingerprint, Set(elements[0]));

    qDebug() << "Кэшировано:" << elements[0];
    updateOutput(elements[0]);
}

void MainWindow::on_clearButton_clicked()
{
    ui->inputWindow->clear();
    initCalculations();
}

