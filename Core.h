#ifndef CORE_H
#define CORE_H

#include "Controls.h"
#include "TerrainGen.h"
#include "PhysicsManager.h"

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
		
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);
	/************custom****************/
	Controls* controls;
	OgreBulletDynamics::RigidBody *ninjaBody;
	SceneNode* camNode;

	PhysicsManager * physicsManager = new PhysicsManager(mRoot, mWindow, mCamera,mSceneMgr);
	TerrainGen * terrainGen;
	/***********Create scene******************/
	void createScene() {
		// Set ambient light
		mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
		// Create a light
		Light* l = mSceneMgr->createLight("MainLight");
		l->setPosition(20, 80, 50);
		terrainGen = new TerrainGen(mRoot, mWindow, mCamera, mSceneMgr, l);
		
		//Create terrain		
		terrainGen->loadTerrain();
		terrainGen->bulletTerrainshape(physicsManager);
		// Define a floor plane mesh
		
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