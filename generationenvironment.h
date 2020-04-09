#ifndef GENERATIONENVIRONMENT_H
#define GENERATIONENVIRONMENT_H

#include "character.h"
#include "fieldcollection.h"

class GenerationEnvironment
{
    Character* _char = nullptr;
    FieldCollection& _col;

public:
    GenerationEnvironment(FieldCollection& fc);
    GenerationEnvironment(FieldCollection& fc,Character& c);

    inline bool HasCharacter() {return _char != nullptr;}
    Character& character();

    QString parse(QString name, QString format = QString());
};

#endif // GENERATIONENVIRONMENT_H
