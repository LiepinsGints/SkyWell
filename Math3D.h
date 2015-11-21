#ifndef MATH3D_H
#define MATH3D_H
#include <OgreRoot.h>
#include <math.h> 
#include <Ogre.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
/*#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>*/
class Math3D
{
public:
	void init() {

	}
	float pythagor(float a, float b, float c) {
		return sqrt(pow(a, 2) + pow(b, 2) + pow(c, 2));
	}
	Ogre::Vector3 directionVector(Ogre::Vector3 a, Ogre::Vector3 b) {
		Ogre::Vector3 vector;
		vector.x = a.x - b.x;
		vector.y = a.y - b.y;
		vector.z = a.z - b.z;
		float divider = pythagor(vector.x, vector.y, vector.z);
		vector.x /= divider;
		vector.y /= divider;
		vector.z /= divider;


		return vector;
	}
	Ogre::Vector3 nomaliseVector(Ogre::Vector3 vec) {
		float divider = pythagor(vec.x, vec.y, vec.z);
		vec.x /= divider;
		vec.y /= divider;
		vec.z /= divider;

		return vec;
	}
private:


};

#endif