#ifndef CORE_H
#define CORE_H

#include "Controls.h"

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
//Bullet
#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "Shapes/OgreBulletCollisionsCapsuleShape.h"



using namespace Ogre;
class Core
	: public Ogre::WindowEventListener,
	public Ogre::FrameListener
{
public:
	Core();
	virtual ~Core();

	bool go();

private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;
	
	/**********Ogre bullet**************/
	OgreBulletDynamics::DynamicsWorld *mWorld;	// OgreBullet World
	OgreBulletCollisions::DebugDrawer *debugDrawer;
	int mNumEntitiesInstanced;

	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);
	/************custom****************/
	Controls* controls;
	OgreBulletDynamics::RigidBody *ninjaBody;
	SceneNode* camNode;
	/***********Create scene******************/
	void createScene() {
		// Start Bullet
		Ogre::Vector3 gravityVector(0, -9.81, 0); // gravity vector for Bullet
		Ogre::AxisAlignedBox bounds(Ogre::Vector3(-10000, -10000, -10000), //aligned box for Bullet
			Ogre::Vector3(10000, 10000, 10000));
		mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);

		/************DEBUG*************/
		debugDrawer = new OgreBulletCollisions::DebugDrawer();
		debugDrawer->setDrawWireframe(true);	// we want to see the Bullet containers
		//
		debugDrawer->setDebugMode(OgreBulletCollisions::DebugDrawer::DBG_MAX_DEBUG_DRAW_MODE);
		debugDrawer->setDrawAabb(true);
		debugDrawer->setDrawContactPoints(true);
		debugDrawer->setDrawFeaturesText(true);
		//
		mWorld->setDebugDrawer(debugDrawer);
		mWorld->setShowDebugShapes(true);		// enable it if you want to see the Bullet containers

		


		SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
		node->attachObject(static_cast <SimpleRenderable *> (debugDrawer));
		// Set ambient light
		mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

		// Create a light
		Light* l = mSceneMgr->createLight("MainLight");
		l->setPosition(20, 80, 50);

		// Define a floor plane mesh
		Entity *ent;
		Plane p;
		p.normal = Vector3(0, 1, 0); p.d = 0;
		MeshManager::getSingleton().createPlane("FloorPlane",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			p, 200000, 200000, 20, 20, true, 1, 9000, 9000,
			Vector3::UNIT_Z);
		// Create an entity (the floor)
		ent = mSceneMgr->createEntity("floor", "FloorPlane");
		ent->setMaterialName("Examples/BumpyMetal");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

		// add collision detection to it
		OgreBulletCollisions::CollisionShape *Shape;
		Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0, 1, 0), 0); 
		OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody("BasePlane",
			mWorld);
		defaultPlaneBody->setStaticShape(Shape, 0.1, 0.8);// (shape, restitution, friction)
														  // push the created objects to the deques
		mShapes.push_back(Shape);
		mBodies.push_back(defaultPlaneBody);
		
		//Create sky
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8); 

	}//<-- end
	/******************CREATE CAMERA******************************/
	void createCamera() {
		mCamera = mSceneMgr->createCamera("MainCam");
		mCamera->setPosition(0, 7, 20);
		mCamera->lookAt(0, 3, 0);
		mCamera->setNearClipDistance(5);

		Ogre::Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

		mCamera->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) /
			Ogre::Real(vp->getActualHeight()));
	}
	
};

#endif