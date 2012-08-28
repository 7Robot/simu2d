#! /bin/bash

svn checkout http://pybox2d.googlecode.com/svn/trunk/ pybox2d-read-only
cd pybox2d-read-only
python2 setup.py build
sudo python2 setup.py install
