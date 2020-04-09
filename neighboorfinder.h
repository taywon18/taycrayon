#ifndef NEIGHBOORFINDER_H
#define NEIGHBOORFINDER_H

#include <QObject>

class NeighboorFinder : public QObject
{
    Q_OBJECT

    struct Pos3d{
        float x,y,z;
    };

    struct Neighboor{
        QString name;
        Pos3d min;
        Pos3d max;
    };

    QList<Neighboor> _neighboors;

public:
    explicit NeighboorFinder(QObject *parent = nullptr);
    void initFromBase();
    void add(QString name, double Xmin, double Ymin,double Zmin,double Xmax,double Ymax,double Zmax);
    QString find(float x, float y);

    QString degreeToDirection(double degree);
    QString gtaAngleToDirection(double rad);

signals:

public slots:
};

#endif // NEIGHBOORFINDER_H
