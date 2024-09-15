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
    Set(const QString initStr);
    Set(const QVector<int>& initValues, char state = Ordinary);

    Set complement() const;
    Set intersection(const Set& secondSet) const;
    Set unionOperation(const Set& secondSet) const;
    Set difference(const Set& secondSet) const;
    Set symDifference(const Set& secondSet) const;

    friend QDebug operator<<(QDebug dbg, const Set &set);
    QString toString();
};

#endif // SET_H
