#include "set.h"

Set::Set(const QVector<int>& initValues, char state)
{
    this->values = initValues;
    this->state = state;
    deleteRepeated();
}

QDebug operator<<(QDebug dbg, const Set &set)
{
    switch (set.state)
    {
    case Empty:
        dbg.nospace() << "Set(Empty)";
        break;
    case Universum:
        dbg.nospace() << "Set(Universum\\{";
        for (int i = 0; i < set.values.size(); ++i)
        {
            dbg.nospace() << set.values[i];
            if (i != set.values.size() - 1)
                dbg.nospace() << ", ";
        }
        dbg.nospace() << "})";
        break;
    default:
        dbg.nospace() << "Set(";
        for (int i = 0; i < set.values.size(); ++i)
        {
            dbg.nospace() << set.values[i];
            if (i != set.values.size() - 1)
                dbg.nospace() << ", ";
        }
        dbg.nospace() << ")";
        break;
    }
    return dbg.space();
}

Set Set::complement() const
{
    switch (state)
    {
    case Empty:
        return Set(QVector<int>{}, Universum);

    case Universum:
        return Set(values, Ordinary);

    case Ordinary:
        return Set(QVector<int>{}, Universum).difference(*this);
    }
    return Set(QVector<int>{}, Empty);
}

Set Set::intersection(const Set& secondSet) const
{
    if (state == Empty || secondSet.state == Empty)
    {
        return Set(QVector<int>{}, Empty);
    }

    if (state == Universum) return secondSet;
    if (secondSet.state == Universum) return *this;

    QVector<int> result;
    for (const int& val : values)
    {
        if (secondSet.values.contains(val))
        {
            result.append(val);
        }
    }

    if (result.isEmpty()) return Set(QVector<int>{}, Empty);
    return Set(result, Ordinary);
}

Set Set::unionOperation(const Set& secondSet) const
{
    if (state == Universum || secondSet.state == Universum)
    {
        return Set(QVector<int>{}, Universum);
    }

    if (state == Empty) return secondSet;
    if (secondSet.state == Empty) return *this;

    QVector<int> result = values;
    for (const int& val : secondSet.values)
    {
        if (!result.contains(val))
        {
            result.append(val);
        }
    }

    return Set(result, Ordinary);
}

Set Set::difference(const Set& secondSet) const
{
    if (this->state == Empty || secondSet.state == Universum)
    {
        return Set(QVector<int>{}, Empty);
    }

    if (this->state == Universum)
    {
        if (secondSet.state == Universum)
        {
            return Set(QVector<int>{}, Empty);
        }
        else if (secondSet.state == Empty)
        {
            return Set(QVector<int>{}, Universum);
        }
        else
        {
            return Set(secondSet.values, Universum);
        }
    }

    QVector<int> result = this->values;
    for (int i = 0; i < result.length(); i++)
    {
        if (secondSet.values.contains(result[i]))
        {
            result.removeAt(i);
            i--;
        }
    }

    return Set(result, Ordinary);
}

Set Set::symDifference(const Set& secondSet) const
{
    Set intersect = intersection(secondSet);
    Set unionSet = unionOperation(secondSet);
    return unionSet.difference(intersect);
}

QString Set::toString(bool isFormal)
{
    QString result = "";
    switch (state)
    {
    case Empty:
        if (isFormal) result += "Ø";
        else result += "Set(Empty)";
        break;

    case Universum:
        if (isFormal) result += "𝕌\\{";
        else result += "Set(Universum\\{";
        for (int i = 0; i < values.size(); ++i)
        {
            result += QString("%1").arg(values[i]);
            if (i != values.size() - 1)
                result += ", ";
        }
        if (isFormal) result += "}";
        else result += "})";
        break;

    default:
        if (isFormal) result += "{";
        else result += "Set(";
        for (int i = 0; i < values.size(); ++i)
        {
            result += QString("%1").arg(values[i]);
            if (i != values.size() - 1)
                result += ", ";
        }
        if (isFormal) result += "}";
        else result += ")";
        break;

    }
    return result;
}

void Set::deleteRepeated()
{
    for (int i = 0; i < values.length() - 1; i++)
    {
        for (int j = i + 1; j < values.length(); j++)
        {
            if (values[i] == values[j])
            {
                values.removeAt(j);
                j--;
            }
        }
    }
}

Set::Set()
{
    Set(QVector<int>{}, Empty);
}

Set::Set(QString initStr)
{
    initStr.replace("Set(", "").replace(")", "");
    initStr.replace("{", "").replace("}", "").replace(",", "");
    QStringList items = initStr.split(" ");
    for (const QString &item : items) {
        bool ok;
        int value = item.toInt(&ok);
        if (ok) {
            values.append(value);
        }
    }
}
