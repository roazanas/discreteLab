#ifndef SET_H
#define SET_H

#include <QVector>
#include <QDebug>

enum States
{
    Ordinary = -1,
    Empty = 0,
    Universum = 1,
};

class Set
{
private:
    QVector<int> values;
    void deleteRepeated();
    char state = Ordinary;

public:
    Set();
    Set(QString initStr);
    Set(const QVector<int>& initValues, char state = Ordinary);

    Set complement();
    Set intersection(Set& secondSet);
    Set unionOperation(Set& secondSet);
    Set difference(Set& secondSet);
    Set symDifference(Set& secondSet);

    friend QDebug operator<<(QDebug dbg, const Set &set);
    QString toString(bool isFormal=false);
    QVector<int> getValues() { return values; };
    void setState(char state) { this->state = state; };
    void correctState();
};

#endif // SET_H
