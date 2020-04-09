#ifndef NEIGHBOORFINDER_H
#define NEIGHBOORFINDER_H

#include <QObject>

class NeighboorFinder : public QObject
{
    Q_OBJECT
public:
    explicit NeighboorFinder(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NEIGHBOORFINDER_H
