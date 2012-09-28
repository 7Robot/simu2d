#include "svgscene.h"

SvgScene::SvgScene(QWidget *parent) :
    QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    // TODO setSceneRect

    QString path("../board.svg");
    renderer = new QSvgRenderer(path, this);
    ParseSvg(path);

    simulatorSocket = new QTcpSocket(this);
    connect(simulatorSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    simulatorSocket->connectToHost("localhost", 1234);


    keyStates['Z'] = false;
    keyStates['Q'] = false;
    keyStates['S'] = false;
    keyStates['D'] = false;
}

void SvgScene::readSocket()
{
    while(simulatorSocket->canReadLine()) {
        QString id;
        qreal x, y, angle;

        QTextStream line(simulatorSocket->readLine(), QIODevice::ReadOnly);
        line >> id >> x >> y >> angle;
        if(dynamicBodies.contains(id)) {
            QGraphicsSvgItem * item =  dynamicBodies[id];
            item->setPos(x, y);
            item->setRotation(angle);
        }
        else
            qDebug() << "Unexpected ID" << id;
    }
}

void SvgScene::ParseSvg(QString path)
{
    QFile file(path);
    QDomDocument doc("board");
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;
    file.close();


    QDomElement layer = doc.documentElement().firstChildElement("g");
    while(!layer.isNull()) { // Cycle on all Inkscape layers.
        QString layerName = layer.attribute("inkscape:label");

        if(layerName == "background" || layerName == "static") {
            AddSvgItem(layer);
        }
        else if(layerName == "dynamic") {
            QDomElement element = layer.firstChildElement();

            while(!element.isNull()) { // Add elements independantly.
                dynamicBodies[element.attribute("id")] = AddSvgItem(element);;

                element = element.nextSiblingElement();
            }
        }

        layer = layer.nextSiblingElement("g");
    }
}

QGraphicsSvgItem * SvgScene::AddSvgItem(QDomElement element)
{
    QGraphicsSvgItem * item = new QGraphicsSvgItem();
    item->setSharedRenderer(renderer);
    item->setElementId(element.attribute("id"));

    QPointF center = item->boundingRect().center();
    qreal offsetX = element.attribute("inkscape:transform-center-x").toFloat() - center.x();
    qreal offsetY = element.attribute("inkscape:transform-center-y").toFloat() - center.y();

    QTransform transform;
    transform.translate(offsetX, offsetY);
    item->setTransform(transform);
    item->setTransformOriginPoint(-offsetX, -offsetY); // For rotations.

    addItem(item);
    return item;
}

void SvgScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    // TODO send mouseEvent->scenePos().
}

void SvgScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    // TODO send mouseEvent->scenePos().
}

void SvgScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    // TODO send release.
}

bool SvgScene::keyEvent(int key, bool newState)
{
    if(keyStates.contains(key)) {

        keyStates[key] = newState;

        // TODO send states.

        return true;
    }
    else
        return false;
}

void SvgScene::keyPressEvent(QKeyEvent * event)
{
    if(!keyEvent(event->key(), true))
        QGraphicsScene::keyPressEvent(event);
}

void SvgScene::keyReleaseEvent(QKeyEvent * event)
{
    if(!keyEvent(event->key(), false))
        QGraphicsScene::keyReleaseEvent(event);
}

