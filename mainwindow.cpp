#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->setInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[ABCDEabcdeФИСВУфисву]"), this));
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
    int count = QRandomGenerator::global()->bounded(3, 10);
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
    auto temp = set.toString(true);
    ui->outputWindow->setText(temp);
}

bool MainWindow::checkSubsetInSet(Set subset, Set set)
{
    for (const int i : subset.getValues())
    {
        if (!set.getValues().contains(i)) return false;
    }
    return true;
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
    if (ui->inputWindow->text().endsWith("𝕌"))
        ui->inputWindow->setText(ui->inputWindow->text().removeLast());
    ui->inputWindow->setText(ui->inputWindow->text().removeLast());
    initCalculations();
}

Set getSetByLetter(QWidget* widget, QString letter)
{
    if (letter == "O")
        return Set(QVector<int>{}, Empty);
    if (letter == "U")
        return Set(QVector<int>{}, Universum);
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>(QString("set%1Input").arg(letter));
    if (lineEdit)
    {
        QString setStr = lineEdit->text();
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
                if (elements[i + 1] != "¬")
                {
                    Set set = Set(elements[i + 1]);
                    elements[i + 1] = set.complement().toString();
                    elements.removeAt(i);
                    --i;
                }
                else
                {
                    int j = i;
                    while (elements[j + 1] == "¬")
                    {
                        ++j;
                        if (j + 1 == elements.size()) return;
                    }
                    while (j != i)
                    {
                        Set set = Set(elements[j + 1]);
                        elements[j + 1] = set.complement().toString();
                        elements.removeAt(j);
                        --j;
                    }
                    --i;
                }
            }
        }
    }
    qDebug() << elements;

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

    Set result = Set(elements[0]);
    cache.insert(fingerprint, result);

    qDebug() << "Кэшировано:" << result;
    updateOutput(result);
}

void MainWindow::on_clearButton_clicked()
{
    ui->inputWindow->clear();
    initCalculations();
}

void MainWindow::on_setAInput_textChanged()
{
    initCalculations();
    on_elemInput_textChanged("");
}

void MainWindow::on_setBInput_textChanged()
{
    initCalculations();
    on_elemInput_textChanged("");
}

void MainWindow::on_setCInput_textChanged()
{
    initCalculations();
    on_elemInput_textChanged("");
}
void MainWindow::on_setDInput_textChanged()
{
    initCalculations();
    on_elemInput_textChanged("");
}

void MainWindow::on_setEInput_textChanged()
{
    initCalculations();
    on_elemInput_textChanged("");
}

void MainWindow::on_elemInput_textChanged(const QString)
{
    QString up = ui->elemInput->text();
    Set down = getSetByLetter(ui->setsGroup, ui->setInput->text());
    bool result;
    if (ui->operatorCB->currentText() == "∈")
    {
        result = checkElemInSet(up.toInt(), down);
        if (result) ui->progressLabel->setText("Принадлежит");
        else ui->progressLabel->setText("Не принадлежит");
    } else {
        result = checkSubsetInSet(Set(up), down);
        if (result) ui->progressLabel->setText("Входит");
        else ui->progressLabel->setText("Не входит");
    }
    ui->progressBar->setValue(result);
}

void MainWindow::on_setInput_textChanged(const QString)
{
    QString text = ui->setInput->text();
    if      (text.toUpper() == "Ф") text = "A";
    else if (text.toUpper() == "И") text = "B";
    else if (text.toUpper() == "С") text = "C";
    else if (text.toUpper() == "В") text = "D";
    else if (text.toUpper() == "У") text = "E";
    ui->setInput->setText(text.toUpper());
    on_elemInput_textChanged("");
}

void MainWindow::on_operatorCB_currentIndexChanged(int)
{
    on_elemInput_textChanged("");
}

void MainWindow::on_actionHotkeys_triggered()
{
    QMessageBox::information(this, "Горячие клавиши",
                             "A, B, C, D, E - ввод множеств в окно\n"
                             "1, 2, 3, 4, 5 - ввод операций в окно\n"
                             "U, O - ввод универсума и пустого множества в окно\n"
                             "Alt+A - задать случайные данные во множество A\n"
                             "Alt+B - задать случайные данные во множество B\n"
                             "Alt+C - задать случайные данные во множество C\n"
                             "Alt+D - задать случайные данные во множество D\n"
                             "Alt+E - задать случайные данные во множество E\n"
                             "Alt+R - задать все множества случайно\n"
                             "Backspace - стереть последний символ в окне ввода\n"
                             "Ctrl+Backspace - стереть всё в окне ввода");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "roazanas / Nikita Zhulanov"
                             "\n\n",
                       "Этот проект разработан студентом 2 курса в качестве реализации лабораторной работы №1 по дискретной математике.\n"
                       "ПНИПУ, ЭТФ, РИС-23-1б (лучшая группа)");
}

void MainWindow::on_randomAllButton_clicked()
{
    on_randAButton_clicked();
    on_randBButton_clicked();
    on_randCButton_clicked();
    on_randDButton_clicked();
    on_randEButton_clicked();
}

