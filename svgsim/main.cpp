#include <QtCore/QCoreApplication>

#include <QtXml>
#include <Box2D.h>
#include <QtNetwork>
#include <iostream>

b2World * world;
const float PI = 3.14159265359;
QMap<QString, b2Body*> dynamicBodies;

void ParseBody(QDomElement element, b2Body * body)
{
    if(element.hasChildNodes()) { // This is a group.
        element = element.firstChildElement();
        while(!element.isNull()) {
            ParseBody(element, body);
            element = element.nextSiblingElement();
        }
    }
    else { // Attach a fixture.
        b2FixtureDef fixtureDef;
        b2PolygonShape polygonShape;
        b2ChainShape chainShape;
        if(element.nodeName() == "rect") {
            float32 w = element.attribute("width").toFloat() / 2;
            float32 h = element.attribute("height").toFloat() / 2;
            b2Vec2 pos(element.attribute("x").toFloat() + w,
                       element.attribute("y").toFloat() + h);

            polygonShape.SetAsBox(w, h, pos, 0);
            fixtureDef.shape = &polygonShape;
        }
        else if(element.nodeName() == "path") {
            QRegExp regexp("fill\\s*:\\s*none");
            regexp.setCaseSensitivity(Qt::CaseInsensitive);

            b2Vec2 vertices[b2_maxPolygonVertices];
            QString data = element.attribute("d");
            QString lastCmd = "";

            for(int i = 0; i < 8; i++) {
                QString cmd = data[0].toLower();
                if()
                if(cmd == 'm') {
                    lastCmd = 'l';
                }
                else if(cmd == 'l') {
                }
                else {
                    qDebug() << "Unexpected path command" << data[0] << "in" << element.attribute("d");
                    break;
                }
            }

            if(regexp.indexIn(element.attribute("style", "fill:none")) >= 0) { // Chain shape.
                // TODO
                fixtureDef.shape = &chainShape;
            }
            else { // Filled polygon.
                polygonShape.
                fixtureDef.shape = &polygonShape;
            }
            float32 w = element.attribute("width").toFloat() / 2;
            float32 h = element.attribute("height").toFloat() / 2;
            b2Vec2 pos(element.attribute("x").toFloat() + w,
                       element.attribute("y").toFloat() + h);
        }
        else {
            qDebug() << "Unexpected shape" << element.nodeName();
            return;
        }
        // TODO other shapes

        // TODO density
        b2Fixture* fixture = body->CreateFixture(&fixtureDef);
    }
}

void ParseSvg(QString path)
{
    QFile file(path);
    QDomDocument doc("board");
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;
    file.close();


    QDomElement layer = doc.documentElement().firstChildElement("g");
    while(!layer.isNull()) { // Cycle on all Inkscape layers.
        QString layerName = layer.attribute("inkscape:label");

        if(layerName == "static" || layerName == "dynamic") {
            QDomElement element = layer.firstChildElement();

            while(!element.isNull()) { // Cycle on layer bodies.
                QString * bodyId = new QString(element.attribute("id"));

                b2BodyDef bodyDef;
                if(layerName == "dynamic")
                    bodyDef.type = b2_dynamicBody;
                bodyDef.position.Set(1.2f, 0.7f); // TODO
                bodyDef.angle = PI; // TODO
                b2Body * body = world->CreateBody(&bodyDef);
                body->SetUserData((void*)bodyId);
                ParseBody(element, body);

                if(layerName == "dynamic")
                    dynamicBodies[*bodyId] = body;
                element = element.nextSiblingElement();
            }
        }

        layer = layer.nextSiblingElement("g");
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Start.";

    world = new b2World(b2Vec2(0.0f, 0.0f)); // Zero gravity.

    ParseSvg("../board.svg");

    qDebug() << "Done.";

    QString key;
    foreach(key, dynamicBodies.keys()) {
        qDebug() << key << dynamicBodies[key]->GetPosition().x;
    }

    // TODO socket

    return a.exec();
}
