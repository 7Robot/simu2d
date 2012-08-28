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

maxForwardSpeed = 250
maxBackwardSpeed = -40
maxDriveForce = 500

class TDTire:
	def __init__(self, world, location):
		self.currentTraction = 1
		self.bodyDef = b2BodyDef()
		self.m_body  = world.CreateDynamicBody(
			shapes=b2PolygonShape(box=(0.5, 1.25)), 
			position=location,
			angularDamping=2,
            linearDamping=0.1,
            shapeFixture=b2FixtureDef(density=2.0))

	def getLateralVelocity(self):
		currentRightNormal = self.m_body.GetWorldVector( b2Vec2(1,0) )
		return b2Dot(currentRightNormal, self.m_body.linearVelocity) * currentRightNormal
		
	def getForwardVelocity(self):
		currentForwardNormal = self.m_body.GetWorldVector( b2Vec2(0,1) )
		return b2Dot( currentForwardNormal, self.m_body.linearVelocity ) * currentForwardNormal;
		
	def updateFriction(self):
		impulse = 1*(self.m_body.mass * - self.getLateralVelocity())
		pt = self.m_body.worldCenter
		self.m_body.ApplyLinearImpulse(impulse=(impulse.x, impulse.y), point=(pt.x, pt.y))
		self.m_body.ApplyAngularImpulse( impulse = 0.8 * self.m_body.inertia * -self.m_body.angularVelocity )
    
	def updateDrive(self, controlState):
		desiredSpeed = 0;
		if controlState == 1:
			desiredSpeed = maxForwardSpeed
		elif controlState == -1:
			desiredSpeed = maxBackwardSpeed

		currentForwardNormal = self.m_body.GetWorldVector( b2Vec2(0,1) )
		currentSpeed = b2Dot( self.getForwardVelocity(), currentForwardNormal )
		force = 0;
		if ( desiredSpeed > currentSpeed ):
			force = maxDriveForce
		elif ( desiredSpeed < currentSpeed ):
			force = -maxDriveForce
		else:
			return
		self.m_body.ApplyForce( self.currentTraction * force * currentForwardNormal, self.m_body.worldCenter )

class Robot:
	def __init__(self, world):
		sy = 3
		y = sy+1
		self.bodyDef = b2BodyDef()
		self.m_body  = world.CreateDynamicBody(
			shapes=b2PolygonShape(box=(3, sy)), 
			position=(0,y),
            shapeFixture=b2FixtureDef(density=2.0))
		#print "robot mass", self.m_body.mass
		#self.mass = 200
            
		self.leftWheel = TDTire(world, (-3.3, y))
		self.rightWheel = TDTire(world, (3.3, y))


		jointR = world.CreateRevoluteJoint(bodyA=self.m_body, 
				bodyB=self.rightWheel.m_body, anchor=(2.7, y),
				lowerAngle = 0, upperAngle = 0, enableLimit = True)
		jointL = world.CreateRevoluteJoint(bodyA=self.m_body, 
				bodyB=self.leftWheel.m_body, anchor=(-2.7, y),
				lowerAngle = 0, upperAngle = 0, enableLimit = True)
				
	def update(self, action):
		self.rightWheel.updateFriction()
		self.leftWheel.updateFriction()
		if action[0] == 1:
			self.rightWheel.updateDrive(action[1])
		elif action[0] == -1:
			self.leftWheel.updateDrive(action[1])
          
class ApplyForce2 (Framework):
	name="ApplyForce2"
	def __init__(self):
		super(ApplyForce2, self).__init__()
		self.world.gravity = (0.0, 0.0)
		self.action = (0,0)

		# The boundaries
		ground = self.world.CreateBody(position=(0, 20))
		ground.CreateEdgeChain(
							[ (-20,-20),
							  (-20, 20),
							  ( 20, 20),
							  ( 20,-20),
							  (-20,-20) ]
							)

		self.robot = Robot(self.world)
		self.body = self.robot.m_body

			
		
	 
	def Keyboard(self, key):
		if key==Keys.K_p:
			self.action = (1,1)
		elif key==Keys.K_l:
			self.action = (1,-1)
		elif key==Keys.K_a:
			self.action = (-1,1)
		elif key==Keys.K_q:
			self.action = (-1,-1)
		else:
			self.action = (0, 0)
			
	def Step(self, settings):
		super(ApplyForce2, self).Step(settings)
		self.robot.update(self.action)
		print self.action
		self.action = (0, 0)

if __name__=="__main__":
     main(ApplyForce2)
