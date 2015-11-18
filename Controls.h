#ifndef CONTROLS_H
#define CONTROLS_H
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreException.h>
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <Ogre.h>

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes

using namespace Ogre;
class Controls {

public:
	Controls(Ogre::Root* _mRoot, Ogre::RenderWindow* _mWindow, Ogre::Camera* _mCamera,
		std::deque<OgreBulletDynamics::RigidBody *> _mBodies, std::deque<OgreBulletCollisions::CollisionShape *>  _mShapes,
		OgreBulletDynamics::DynamicsWorld *_mWorld, int  _mNumEntitiesInstanced, Ogre::SceneManager* _mSceneMgr,
		Ogre::SceneNode* _origin, OgreBulletDynamics::RigidBody *_player
		) {
		mRoot = _mRoot;
		mWindow = _mWindow;
		mCamera = _mCamera;
		mBodies = _mBodies;
		mShapes = _mShapes;
		mWorld = _mWorld;
		mSceneMgr = _mSceneMgr;
		mNumEntitiesInstanced = _mNumEntitiesInstanced;
		player = _player;
		origin = _origin;
		up = false;
		back = false;
		left = false;
		right = false;
		
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHandle = 0;
		std::ostringstream windowHandleStr;

		mWindow->getCustomAttribute("WINDOW", &windowHandle);
		windowHandleStr << windowHandle;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

		mInputMgr = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(
			mInputMgr->createInputObject(OIS::OISKeyboard, false));
		mMouse = static_cast<OIS::Mouse*>(
			mInputMgr->createInputObject(OIS::OISMouse, false));

	}
	
	bool listen() {
		mKeyboard->capture();
		mMouse->capture();
		if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
		else if (mKeyboard->isKeyDown(OIS::KC_B))
		{
			Vector3 size = Vector3::ZERO;	// size of the box
											// starting position of the box
			Vector3 position = (mCamera->getDerivedPosition() + mCamera->getDerivedDirection().normalisedCopy() * 10);
			// create an ordinary, Ogre mesh with texture
			Entity *entity = mSceneMgr->createEntity(
				"Box" + StringConverter::toString(mNumEntitiesInstanced),
				"cube.mesh");
			entity->setCastShadows(true);
			// we need the bounding box of the box to be able to set the size of the Bullet-box
			AxisAlignedBox boundingB = entity->getBoundingBox();
			size = boundingB.getSize(); size /= 2.0f; // only the half needed
			size *= 0.95f;	// Bullet margin is a bit bigger so we need a smaller size
							// (Bullet 2.76 Physics SDK Manual page 18)
			entity->setMaterialName("Examples/BumpyMetal");
			SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entity);
			node->scale(0.05f, 0.05f, 0.05f);	// the cube is too big for us
			size *= 0.05f;						// don't forget to scale down the Bullet-box too

											// after that create the Bullet shape with the calculated size
			OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);
			// and the Bullet rigid body
			OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
				"defaultBoxRigid" + StringConverter::toString(mNumEntitiesInstanced),
				mWorld);
			defaultBody->setShape(node,
				sceneBoxShape,
				0.6f,			// dynamic body restitution
				0.6f,			// dynamic body friction
				1.0f, 			// dynamic bodymass
				position,		// starting position of the box
				Quaternion(0, 0, 0, 1));// orientation of the box
			mNumEntitiesInstanced++;

			defaultBody->setLinearVelocity(
				mCamera->getDerivedDirection().normalisedCopy() * 7.0f); // shooting speed
																		 // push the created objects to the dequese
			mShapes.push_back(sceneBoxShape);
			mBodies.push_back(defaultBody);

		}

		//Basic camera movement
		if (mKeyboard->isKeyDown(OIS::KC_UP)) {
			player->enableActiveState();
			player->setLinearVelocity(Ogre::Vector3(0.0, 0.0, -10.0));
		}
		if (mKeyboard->isKeyDown(OIS::KC_DOWN)) {
			player->enableActiveState();
			player->setLinearVelocity(Ogre::Vector3(0.0, 0.0, 10.0));
		}
		if (mKeyboard->isKeyDown(OIS::KC_LEFT)) {
			player->enableActiveState();
			player->setLinearVelocity(Ogre::Vector3(+10.0, 0.0, 0));
		}
		if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
			player->enableActiveState();
			player->setLinearVelocity(Ogre::Vector3(-10.0, 0.0, 0));
		}
		//screen size change
		if (mKeyboard->isKeyDown(OIS::KC_O)) {
			mWindow->setFullscreen(false, 1024, 768);
		}
		if (mKeyboard->isKeyDown(OIS::KC_P)) {
			mWindow->setFullscreen(true, 1024, 768);
		}
		return true;
	}
private:
	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;
	Ogre::Vector3 position;
	Ogre::Root* mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneNode* objectNode;
	Ogre::SceneNode* origin;
	Ogre::SceneNode* cameraNode;
	bool up;
	bool back;
	bool left;
	bool right;

	std::deque<OgreBulletDynamics::RigidBody *> mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	OgreBulletDynamics::DynamicsWorld *mWorld;
	int  mNumEntitiesInstanced;
	OgreBulletDynamics::RigidBody *player;


};


#endif