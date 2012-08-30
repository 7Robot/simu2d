#!/usr/bin/python2
# -*- coding: utf-8 -*-


__doc__ = """
 TestBed de simulation physique d'un robot (pas encore concluant) inspiré de
 http://www.iforce2d.net/b2dtut/top-down-car
 Non, on ne peut pas se contenter d'appliquer les forces de frottement aux roues
 sinon le robot se comporterait comme un bateau (partirait en dérive) ...
 
 NÉCESSITE LA VERSION SVN DE PYBOX2D --> install_pybox.sh


 Utilisation du script :
 ------------------------
	 touche ESC  : quitter
     touches A,P : augmente la vitesse des roues gauche et droite respectivement
     touches Q,L : diminue  la vitesse des roues gauche et droite respectivement
     clic        : applique une force à un corps
     Pression continue d'une touche = une pression
"""


from framework import *
from Box2D import *
from math import cos, sin, pi

maxForwardSpeed = 250
maxBackwardSpeed = -40
maxDriveForce = 500



class TDTire:
	def __init__(self, name, world, location):
		self.name = name
		self.currentTraction = 1
		self.bodyDef = b2BodyDef()
		self.m_body  = world.CreateDynamicBody(
			shapes=b2PolygonShape(box=(0.5*0.5, 0.5*1.25)), 
			position=location,
			angularDamping=2,
            linearDamping=0.1,
            shapeFixture=b2FixtureDef(density=2.0))
		#self.mass = 1000  # pratique pour accentuer les traits durant les tests
		self.v = self.m_body.linearVelocity

	def getLateralVelocity(self):
		currentRightNormal = self.m_body.GetWorldVector( b2Vec2(1,0) )
		#print self.name, "lateral speed", b2Dot(currentRightNormal, self.m_body.linearVelocity)
		return b2Dot(currentRightNormal, self.m_body.linearVelocity) * currentRightNormal
		
	def getForwardVelocity(self):
		currentForwardNormal = self.m_body.GetWorldVector( b2Vec2(0,1) )
		#print self.name, "forward speed", b2Dot( currentForwardNormal, self.m_body.linearVelocity )
		return b2Dot( currentForwardNormal, self.m_body.linearVelocity ) * currentForwardNormal;
		
	def updateFriction(self):
		impulse = 1.7*(self.m_body.mass * - self.getLateralVelocity())
		self.v = self.m_body.linearVelocity
		pt = self.m_body.worldCenter
		self.m_body.ApplyLinearImpulse(impulse=(impulse.x, impulse.y), point=(pt.x, pt.y))
		self.m_body.ApplyAngularImpulse( impulse = 0.9 * self.m_body.inertia * -self.m_body.angularVelocity )
		
	def stop(self):
		impulse = 1.7*(self.m_body.mass * - self.m_body.linearVelocity)
		self.v = self.m_body.linearVelocity
		pt = self.m_body.worldCenter
		self.m_body.ApplyLinearImpulse(impulse=(impulse.x, impulse.y), point=(pt.x, pt.y))
		self.m_body.ApplyAngularImpulse( impulse = 0.9 * self.m_body.inertia * -self.m_body.angularVelocity )
    
	def updateDrive(self, w):
		if w == 0:
			self.stop()
		else:
			currentForwardNormal = self.m_body.GetWorldVector( b2Vec2(0,1) )
			currentSpeed = b2Dot( self.getForwardVelocity(), currentForwardNormal )
			#self.m_body.ApplyForce( self.currentTraction * w * currentForwardNormal, self.m_body.worldCenter )
			impulse = 1*(self.m_body.mass * (-self.getForwardVelocity() + w*currentForwardNormal))
			pt = self.m_body.worldCenter
			self.m_body.ApplyLinearImpulse(impulse=(impulse.x, impulse.y), point=(pt.x, pt.y))

class Robot:
	def __init__(self, world):
		self.largeur = 3
		sy = 3
		y = sy+15
		self.bodyDef = b2BodyDef()
		self.m_body  = world.CreateDynamicBody(
			shapes=b2PolygonShape(box=(self.largeur, sy)), 
			position=(0,y),
			density=1, friction=0.0,
            shapeFixture=b2FixtureDef(density=2.0))
		#print "robot mass", self.m_body.mass
		#self.mass = 200
            
		self.leftWheel = TDTire("left", world, (-3.3, y))
		self.rightWheel = TDTire("right", world, (3.3, y))


		jointR = world.CreateRevoluteJoint(bodyA=self.m_body, 
				bodyB=self.rightWheel.m_body, anchor=(2.7, y),
				lowerAngle = 0, upperAngle = 0, enableLimit = True)
		jointL = world.CreateRevoluteJoint(bodyA=self.m_body, 
				bodyB=self.leftWheel.m_body, anchor=(-2.7, y),
				lowerAngle = 0, upperAngle = 0, enableLimit = True)
				
	def update(self, w_gauche, w_droite):
		self.rightWheel.updateFriction()
		self.leftWheel.updateFriction()
		self.rightWheel.updateDrive(w_droite)
		self.leftWheel.updateDrive(w_gauche)
          
class ApplyForce2 (Framework):
	name="ApplyForce2"
	def __init__(self):
		super(ApplyForce2, self).__init__()
		self.world.gravity = (0.0, 0.0)

		# The boundaries
		ground = self.world.CreateBody(position=(0, 20))
		ground.CreateEdgeChain(
							[ (-20,-20),
							  (-20, 20),
							  ( 20, 20),
							  ( 20,-20),
							  (-20,-20) ]
							)

		self.w_gauche = 0           # Vitesse angulaire de la roue gauche
		self.w_droite = 0

		self.robot = Robot(self.world)
		self.body = self.robot.m_body
		
		self.tire = TDTire("a tire", self.world, (10, 10))

			
		
	 
	def Keyboard(self, key):
		if key==Keys.K_p:
			self.w_droite += 1
		elif key==Keys.K_l:
			self.w_droite -= 1
		elif key==Keys.K_a:
			self.w_gauche += 1
		elif key==Keys.K_q:
			self.w_gauche -= 1
		print ("Vitesse angulaire des roues : w_gauche :", self.w_gauche, 
					" -- w_droite :", self.w_droite)
			
	def Step(self, settings):
		super(ApplyForce2, self).Step(settings)
		#self.robot.update(self.w_gauche, self.w_droite)
		v = 0.5*(self.w_droite + self.w_gauche)
		w = 0.5*(self.w_droite - self.w_gauche)
		self.robot.m_body.linearVelocity = b2Vec2(v * cos(self.body.angle+pi/2), v * sin(self.body.angle+pi/2))
		self.robot.m_body.angularVelocity = w/float(self.robot.largeur+0.6)
		#self.tire.m_body.ApplyForce( b2Vec2(-100,0), self.tire.m_body.worldCenter );
		#self.tire.m_body.linearVelocity = b2Vec2(0,5)
		self.tire.updateFriction()
		


if __name__=="__main__":
     main(ApplyForce2)
