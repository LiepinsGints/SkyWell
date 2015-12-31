#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H
//Bullet
#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "Shapes/OgreBulletCollisionsCapsuleShape.h"
#include "Shapes/OgreBulletCollisionsTerrainShape.h" //terrain
#include "bulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
using namespace Ogre;
class PhysicsManager {

public:
	PhysicsManager(Ogre::Root* _mRoot, Ogre::RenderWindow* _mWindow, Ogre::Camera* _mCamera, 
		Ogre::SceneManager* _mSceneMgr) {
		mRoot = _mRoot;
		mWindow = _mWindow;
		mCamera = _mCamera;
		mSceneMgr = _mSceneMgr;
		
		startBullet();
	}
	void startBullet() {
		gravityVector = Ogre::Vector3 (0, -9.81, 0); // gravity vector for Bullet
		bounds = Ogre::AxisAlignedBox (Ogre::Vector3(-2046, -2046, -2046), //aligned box for Bullet
			Ogre::Vector3(2046, 2046, 2046));
		mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
	}
	void enableDebug(bool _enable) {
		debugDrawer = new OgreBulletCollisions::DebugDrawer();
		debugDrawer->setDrawWireframe(_enable);

		debugDrawer->setDebugMode(OgreBulletCollisions::DebugDrawer::DBG_MAX_DEBUG_DRAW_MODE);
		debugDrawer->setDrawAabb(_enable);
		debugDrawer->setDrawContactPoints(_enable);
		debugDrawer->setDrawFeaturesText(_enable);

		mWorld->setDebugDrawer(debugDrawer);
		mWorld->setShowDebugShapes(_enable);
		SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
		node->attachObject(static_cast <SimpleRenderable *> (debugDrawer));
	}

	void addMbodies(OgreBulletDynamics::RigidBody * _mBody) {
		mBodies.push_back(_mBody);
	}
	void addMshapes(OgreBulletCollisions::CollisionShape * _mShape) {
		mShapes.push_back(_mShape);
	}

	// OgreBullet physic delete - RigidBodies
	void clearMBodies() {
		std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
		while (mBodies.end() != itBody)
		{
			delete *itBody;
			++itBody;
		}
		mBodies.clear();
	}

	// OgreBullet physic delete - Shapes
	void clearMShapes() {
		std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
		while (mShapes.end() != itShape)
		{
			delete *itShape;
			++itShape;
		}

		mShapes.clear();

	}

	//Destroy all physics elements
	void clear() {
		clearMBodies();
		clearMShapes();

		delete mWorld->getDebugDrawer();
		mWorld->setDebugDrawer(0);
		delete mWorld;
		delete mRoot;
	}

	OgreBulletDynamics::DynamicsWorld *getMWorld() {
		return mWorld;
	}

	int getEntitiesInstanced() {
		return entitiesInstanced;
	}

	void setEntitiesInstanced(int _entitiesInstanced) {
		entitiesInstanced = _entitiesInstanced;
	}
private:
	Ogre::Root* mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	//Ogre bullet variables
	OgreBulletCollisions::DebugDrawer *debugDrawer;
	int entitiesInstanced;
	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	//Bullet defs
	Ogre::Vector3 gravityVector;
	Ogre::AxisAlignedBox bounds;
	OgreBulletDynamics::DynamicsWorld *mWorld;
};


#endif