#include "generationenvironment.h"
#include <QDateTime>

GenerationEnvironment::GenerationEnvironment(FieldCollection& fc, Character& c)
    : GenerationEnvironment(fc)
{
    _char = &c;
}

GenerationEnvironment::GenerationEnvironment(FieldCollection& fc)
    : _col(fc)
{

}

Character &GenerationEnvironment::character()
{
    if(!HasCharacter())
        throw std::exception();

    return *_char;
}

QString GenerationEnvironment::parse(QString name, QString format)
{
    static QString charstr("perso.");
    static QString datestr("date.");

    if(name.startsWith(charstr))
    {
        if(_char == nullptr)
            return "";

        name.remove(0, charstr.length());

        if(name == "prénom" || name == "prenom")
            return _char->firstName();
        else if(name == "prénoms" || name == "prenoms")
            return _char->firstNames().join(" ");
        else if(name == "nom")
            return _char->lastName();
        else if(name == "bureau")
            return _char->office();
        else if(name == "profession")
            return _char->profession();
        else if(name == "matricule")
            return _char->matricle();
		else if(name == "p" || name == "P")
        {
            QString firstname = _char->firstName();
            if(!firstname.isEmpty())
                return firstname.at(0).toUpper();
        }

        return "";
    }

    if(name.startsWith(datestr))
    {
        name.remove(0, datestr.length());
        QDateTime now = QDateTime::currentDateTime();

        if(name == "dd")
            return now.toString("dd");
		else if(name == "month" || name == "mm")
			return now.toString("month");
        else if(name == "yy")
            return now.toString("yy");
        else if(name == "yyyy")
            return now.toString("yyyy");
        else if(name == "hh")
            return now.toString("hh");
		else if(name == "minute")
            return now.toString("mm");
        else if(name == "ss")
            return now.toString("ss");



        return "";
    }

    return "";
}
