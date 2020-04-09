#ifndef CHARACTER_H
#define CHARACTER_H
#include <QString>
#include <QStringList>
#include <QVariant>

class Character{
    private:
        QStringList _firstNames;
        QString _lastName;
        QString _matricle;
        QString _profession;
        QString _office;

    public:
        inline QVariant ToVariant()
        {
            QVariantMap ret;
            ret["firstnames"] = QVariant(_firstNames);
            ret["lastname"] = _lastName;
            ret["matricle"] = _matricle;
            ret["profession"] = _profession;
            ret["office"] = _office;

            return ret;
        }

        inline static Character FromVariant(QVariant& v)
        {
            QVariantMap map = v.toMap();

            Character ret;
            ret._firstNames = map["firstnames"].toStringList();
            ret._lastName = map["lastname"].toString();
            ret._matricle = map["matricle"].toString();
            ret._profession = map["profession"].toString();
            ret._office = map["office"].toString();

            return ret;
        }

        inline QString lastName() const
        {
            return _lastName;
        }

        inline void setLastName(const QString &lastName)
        {
            _lastName = lastName;
        }

        inline QString matricle() const
        {
        return _matricle;
        }

        inline void setMatricle(const QString &matricle)
        {
        _matricle = matricle;
        }

        inline QString profession() const
        {
        return _profession;
        }

        inline void setProfession(const QString &profession)
        {
        _profession = profession;
        }

        inline QString office() const
        {
        return _office;
        }

        inline void  setOffice(const QString &office)
        {
        _office = office;
        }

        inline QString firstName() const
        {
            if(_firstNames.length())
                return _firstNames[0];

            return "";
        }

        inline QStringList firstNames() const
        {
            return _firstNames;
        }

        inline void setFirstNames(const QStringList &firstNames)
        {
            _firstNames = firstNames;
        }

        inline QString toString()
        {
            QStringList lst(firstName());
            lst << lastName();

            return lst.join(" ");
        }
};

#endif // CHARACTER_H

