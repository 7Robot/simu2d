#!/usr/bin/python2
# -*- coding: utf-8 -*-

__doc__ = """
 Vérification et visualisation des équations du mouvement d'un robot à 2 roues
 ----------------------------------------------
 
 vd = vitesse_angulaire_roue_droite * rayon
 vg = vitesse_angulaire_roue_gauche * rayon
 Vitesses :
 v = (vd+vg)/2
 w = (vd-vg)/2 (vitesse angulaire)
 
 
 
 
 Utilisation du script :
 ------------------------
	 touche ESC  : quitter
     touches A,P : augmente la vitesse des roues gauche et droite respectivement
     touches Q,L : diminue  la vitesse des roues gauche et droite respectivement
     Pression continue d'une touche = une pression
"""

from math import sin, cos, pi
import pygame
from pygame.locals import *


print __doc__

# --- Initiliations de l'affichage

clock = pygame.time.Clock() 
SIZE = 798, 603
pygame.init()
screen = pygame.display.set_mode(SIZE)
screen.fill((255, 255, 255))

# --- Initiliations des propriétés du robot


dist_roues = float(40) # Dist inter-roue
w_gauche = 0           # Vitesse angulaire de la roue gauche
w_droite = 0


th = 0 # theta
x = 400
y = 300
dt = 0.1


done = False
while not done:
	# -- Gestion des évènements clavier
	
	for e in pygame.event.get():
		if e.type == QUIT or (e.type == KEYDOWN and e.key == K_ESCAPE):
			done = True
			break  
		elif e.type == KEYDOWN:
			if e.key == K_a:
				w_gauche +=1
			elif e.key == K_q:
				w_gauche -=1
			elif e.key == K_p:
				w_droite +=1
			elif e.key == K_l:
				w_droite -=1
			print ("Vitesse angulaire des roues : w_gauche :", w_gauche, 
					" -- w_droite :", w_droite)
			
	# --- Calcul des nouvelles coordonnées du robot
	
	dv = w_gauche + w_droite
	dw = w_droite - w_gauche
	
	x += 0.5*dv * cos(th) * dt
	y += 0.5*dv * sin(th) * dt
	th += dw*dt/dist_roues

	# --- Actualisation de l'affichage
	
	screen.fill((255, 255, 255))
	
	pygame.draw.circle(screen, (0,0,0), (int(x),int(y)), int(0.5*dist_roues), 1)
	pygame.draw.line(screen,   (0,0,0), (int(x),int(y)), 
	     (0.5*dist_roues*cos(th)+int(x),0.5*dist_roues*sin(th)+int(y)))
	pygame.display.flip()
	clock.tick(30)
