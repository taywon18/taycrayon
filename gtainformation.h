#ifndef GTAINFORMATION_H
#define GTAINFORMATION_H

#include <QMap>
#include <QSet>

class GtaInformation
{
public:
    GtaInformation();
    void populate();
    static GtaInformation Instance();

    bool isBike(unsigned short id);
    bool isMotorCycle(unsigned short id);
    bool isCycle(unsigned short id);
    bool isAirplane(unsigned short id);
    bool isChoppa(unsigned short id);
    bool isLandVehicle(unsigned short id);
    bool isBoat(unsigned short id);
    bool isLand(unsigned short id);

    QString getName(unsigned short id);

public:
    QMap<unsigned short, QString> _motorcycles;
    QMap<unsigned short, QString> _bikes;
    QMap<unsigned short, QString> _airplane;
    QMap<unsigned short, QString> _choppa;
    QMap<unsigned short, QString> _boats;
    QMap<unsigned short, QString> _land;

};

#endif // GTAINFORMATION_H
