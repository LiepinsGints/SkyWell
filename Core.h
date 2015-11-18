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
	Ogre::SceneNode* origin;
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
	/***********Create scene******************/
	void createScene() {
		// Start Bullet
		Ogre::Vector3 gravityVector(0, -9.81, 0); // gravity vector for Bullet
		Ogre::AxisAlignedBox bounds(Ogre::Vector3(-10000, -10000, -10000), //aligned box for Bullet
			Ogre::Vector3(10000, 10000, 10000));
		mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);

		// add Debug info display tool
		debugDrawer = new OgreBulletCollisions::DebugDrawer();
		debugDrawer->setDrawWireframe(false);	// we want to see the Bullet containers

		mWorld->setDebugDrawer(debugDrawer);
		mWorld->setShowDebugShapes(false);		// enable it if you want to see the Bullet containers
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
		Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0, 1, 0), 0); // (normal vector, distance)
																								// a body is needed for the shape
		OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody("BasePlane",
			mWorld);
		defaultPlaneBody->setStaticShape(Shape, 0.1, 0.8);// (shape, restitution, friction)
														  // push the created objects to the deques
		mShapes.push_back(Shape);
		mBodies.push_back(defaultPlaneBody);
		//---------------------->Ninja
		Vector3 size = Vector3::ZERO;	// size of the box
										// starting position of the box
		//Vector3 position = (mCamera->getDerivedPosition() + mCamera->getDerivedDirection().normalisedCopy() * 10);
		Vector3 position(0.,200,0.);
		Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("Player","ninja.mesh");
		// we need the bounding box of the box to be able to set the size of the Bullet-box
		AxisAlignedBox boundingB = ninjaEntity->getBoundingBox();
		size = boundingB.getSize(); 
		size /= 2.0f; // only the half needed
		size *= 0.80f;	// Bullet margin is a bit bigger so we need a smaller size

		origin = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		origin->attachObject(ninjaEntity);
		SceneNode* ninjaNode = origin->createChildSceneNode();
		ninjaNode->attachObject(mCamera);

		origin->scale(0.05f, 0.05f, 0.05f);	// the cube is too big for us
		size *= 0.05f;						// don't forget to scale down the Bullet-box too
		OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);
		// and the Bullet rigid body
		//btRigidBody::setAngularFactor(btVector3(Yaw, Pitch, Roll));
		ninjaBody = new OgreBulletDynamics::RigidBody(
			"NinjaRigidBody" + StringConverter::toString(mNumEntitiesInstanced),
			mWorld);
		

		ninjaBody->setShape(origin,
			sceneBoxShape,
			0.6f,			// dynamic body restitution
			0.6f,			// dynamic body friction
			2.0f, 			// dynamic bodymass
			position,		// starting position of the box
			Quaternion(1, 0, 0, 0));// orientation of the box
		mNumEntitiesInstanced++;
		//defaultBody->setOrientation(btQuaternion().z.angle(40));
		//ninjaNode->rotate(Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3(1, 1, 0)), Ogre::Node::TransformSpace::TS_WORLD);

		ninjaBody->setLinearVelocity(
			mCamera->getDerivedDirection().normalisedCopy() * 1.0f);
		//Prevent falling over
		ninjaBody->getBulletRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

		mShapes.push_back(sceneBoxShape);
		mBodies.push_back(ninjaBody);

		//ninjaBody->setLinearVelocity(Ogre::Vector3(0.0, 0.0, 100.0));
			
		//Camera behind ninja
		//mCamera->setPosition(ninjaNode->getPosition().x, ninjaNode->getPosition().y+10, ninjaNode->getPosition().z+20);
		//mCamera->lookAt(ninjaNode->getPosition().x, ninjaNode->getPosition().y + 5, ninjaNode->getPosition().z);

	}//<-- end
	/******************CREATE CAMERA******************************/
	void createCamera() {
		mCamera = mSceneMgr->createCamera("MainCam");
		mCamera->setPosition(0, 20, 20);
		mCamera->lookAt(0, 0, 0);
		mCamera->setNearClipDistance(5);

		Ogre::Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

		mCamera->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) /
			Ogre::Real(vp->getActualHeight()));
	}
	
};

#endif