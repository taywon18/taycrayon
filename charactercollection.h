#ifndef CHARACTERCONTAINER_H
#define CHARACTERCONTAINER_H
#include <QList>
#include "character.h"

class CharacterCollection : public QList<Character>{
    public:
    inline QVariantList ToVariant() const{
        QVariantList ret;
        for(auto i : *this)
            ret.append(i.ToVariant());

        return ret;
    }

    inline static CharacterCollection FromVariant(QVariant var)
    {
        CharacterCollection ret;
        for(auto i : var.toList())
            ret.append(Character::FromVariant(i));
        return ret;
    }
};

#endif // CHARACTERCONTAINER_H
