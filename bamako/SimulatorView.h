#ifndef SIMULATORVIEW_H
#define SIMULATORVIEW_H

#include <QtGui>
#include "SimulatorScene.h"

class SimulatorView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SimulatorView(SimulatorScene *scene, QWidget *parent = 0);
    int heightForWidth(int w) const;
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    QSize sizeHint() const;

private:
    SimulatorScene *scene;
    QMap<int, bool> keyStates;
};

#endif // SIMULATORVIEW_H
