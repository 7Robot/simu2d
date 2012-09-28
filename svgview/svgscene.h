#ifndef SVGSCENE_H
#define SVGSCENE_H

#include <QtGui>
#include <QtSvg>
#include <QtNetwork>
#include <QtXml>

class SvgScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SvgScene(QWidget *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    bool keyEvent(int key, bool newState);

private slots:
    void readSocket();
    
private:
    void ParseSvg(QString path);
    QGraphicsSvgItem * AddSvgItem(QDomElement element);

    QMap<int, bool> keyStates;
    QMap<QString, QGraphicsSvgItem*> dynamicBodies;
    QSvgRenderer * renderer;

    QTcpSocket * simulatorSocket;
};

#endif // SVGSCENE_H
