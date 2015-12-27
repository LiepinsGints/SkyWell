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
#include "Shapes/OgreBulletCollisionsCapsuleShape.h"

#include "Math3D.h"

using namespace Ogre;
class Controls {

public:
	Controls(Ogre::Root* _mRoot, Ogre::RenderWindow* _mWindow, Ogre::Camera* _mCamera,
		std::deque<OgreBulletDynamics::RigidBody *> _mBodies, std::deque<OgreBulletCollisions::CollisionShape *>  _mShapes,
		OgreBulletDynamics::DynamicsWorld *_mWorld, int  _mNumEntitiesInstanced, Ogre::SceneManager* _mSceneMgr
		) {
		mRoot = _mRoot;
		mWindow = _mWindow;
		mCamera = _mCamera;
		mBodies = _mBodies;
		mShapes = _mShapes;
		mWorld = _mWorld;
		mSceneMgr = _mSceneMgr;
		mNumEntitiesInstanced = _mNumEntitiesInstanced;
		createCharacter();
		/*player = _player;
		origin = _origin;
		ninjaNode = _ninjaNode;*/
		up = false;
		back = false;
		left = false;
		right = false;
		jump = false;
		lock = false;
		animationId = 0;
		speed = 20;
		
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
	void createCharacter() {
		Vector3 position(0., 200, 0.);
		playerEntity = mSceneMgr->createEntity("Player", "Sinbad.mesh");
		//Animation
		// Set animation blend mode to additive / cumulative.
		playerEntity->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

		// Get the two halves of the idle animation.
		animationId = 0;
		mAnimationStateBase = playerEntity->getAnimationState("IdleBase");
		mAnimationStateTop = playerEntity->getAnimationState("IdleTop");

		// Enable both of them and set them to loop.
		mAnimationStateBase->setLoop(true);
		mAnimationStateBase->setEnabled(true);
		mAnimationStateTop->setLoop(true);
		mAnimationStateTop->setEnabled(true);
		//Sinbad.mesh ninja.mesh
		// we need the bounding box of the box to be able to set the size of the Bullet-box
		AxisAlignedBox boundingB = playerEntity->getBoundingBox();
		float height = (boundingB.getSize().y * (1.0f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor())) / 2.0f;
		float width = (boundingB.getSize().x * (1.0f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor())) / 2.0f;

		origin = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		origin->attachObject(playerEntity);
		//origin->attachObject(mCamera);
		camNode = origin->createChildSceneNode();
		camNode->attachObject(mCamera);
		camNode->rotate(Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3(0, 1, 0)), Ogre::Node::TransformSpace::TS_WORLD);

		origin->scale(0.5f, 0.5f, 0.5f);	// the cube is too big for us
		OgreBulletCollisions::CapsuleCollisionShape *sceneCapsuleShape
			= new OgreBulletCollisions::CapsuleCollisionShape(width*0.5f,
				(height*0.5f)*0.55f,
				Vector3(0, 1, 0));

		player = new OgreBulletDynamics::RigidBody(
			"PlayerRigidBody" + StringConverter::toString(mNumEntitiesInstanced),
			mWorld);

		player->setShape(origin,
			sceneCapsuleShape,
			0.1f,			// dynamic body restitution
			0.7f,			// dynamic body friction
			5.0f, 			// dynamic bodymass
			Vector3(position.x, position.y, position.z),		// starting position of the box
			Quaternion(1, 0, 0, 0));// orientation of the box

		mNumEntitiesInstanced++;

		player->setLinearVelocity(mCamera->getDerivedDirection().normalisedCopy() * 1.0f);
		//Prevent falling over
		player->getBulletRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
		//Prevent rotation after hitting objects
		player->getBulletRigidBody()->setRestitution(0.0f);
		player->getBulletRigidBody()->setAngularFactor(0.0f);
		//

		//playerBody
		mShapes.push_back(sceneCapsuleShape);
		mBodies.push_back(player);

	}
	
	void setAnimation(int _animationId) {
		//disable previous
		if (animationId != _animationId) {

				playerEntity->getAnimationState("RunBase")->setEnabled(false);
				playerEntity->getAnimationState("RunTop")->setEnabled(false);

				playerEntity->getAnimationState("IdleBase")->setEnabled(false);
				playerEntity->getAnimationState("IdleTop")->setEnabled(false);

				

			//Set new animation
			switch (_animationId) {
			case 0://idle
				mAnimationStateBase = playerEntity->getAnimationState("IdleBase");
				mAnimationStateTop = playerEntity->getAnimationState("IdleTop");
				break;
			case 1://run
				mAnimationStateBase = playerEntity->getAnimationState("RunBase");
				mAnimationStateTop = playerEntity->getAnimationState("RunTop");
				break;
			case 2://run
				
				break;
			}
			animationId = _animationId;
			// Enable both of them and set them to loop.
			mAnimationStateBase->setLoop(true);
			mAnimationStateBase->setEnabled(true);
			mAnimationStateTop->setLoop(true);
			mAnimationStateTop->setEnabled(true);
		}
	}
	void updateAnimations(Ogre::Real elapsedTime) {
		Ogre::Real time = elapsedTime;
		if (back == true) {
			time /= 2;
		}
		if ((left || right) && (up == false && back == false)) {
			time /= 6;
		}

		mAnimationStateBase->addTime(time);
		mAnimationStateTop->addTime(time);
	}

	bool listen(const Ogre::FrameEvent& fe) {
		mKeyboard->capture();
		mMouse->capture();

		if (player->getLinearVelocity().y > -0.05 && player->getLinearVelocity().y < 0.05) {
			lock = false;
		}
		else
		{
			lock=true;
		}

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
				100.0f, 			// dynamic bodymass
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
		
		//	mCamera->getDerivedPosition() + mCamera->getDerivedDirection().normalisedCopy() * 10
		
		if (mKeyboard->isKeyDown(OIS::KC_UP) && lock==false) {
			if (jump != true) {
				up = true;	
				setAnimation(1);
			}
				
		}
		if (mKeyboard->isKeyDown(OIS::KC_DOWN) && lock == false) {
			if (jump != true) {
				back = true;
				setAnimation(1);
			}
			
		}
		if (mKeyboard->isKeyDown(OIS::KC_LEFT) && lock == false) {
			if (jump != true) {
				left = true;
				setAnimation(1);
			}
			
		}
		if (mKeyboard->isKeyDown(OIS::KC_RIGHT) && lock == false) {
			if (jump != true) {
				right = true;
				setAnimation(1);
			}
			
		}
		if (mKeyboard->isKeyDown(OIS::KC_NUMPAD0) && lock == false) {
			if(jump!=true){
				jump = true;
				player->enableActiveState();
				Vector3  direction = mCamera->getDerivedDirection().normalisedCopy();
				player->setLinearVelocity(Vector3(direction.x*speed / 3, speed*0.5, direction.z*speed / 3));
				setAnimation(2);
			}
		}
		//screen size change
		if (mKeyboard->isKeyDown(OIS::KC_O)) {
			mWindow->setFullscreen(false, 1024, 768);
		}
		if (mKeyboard->isKeyDown(OIS::KC_P)) {
			mWindow->setFullscreen(true, 1024, 768);
		}

		//Key release
		if (!mKeyboard->isKeyDown(OIS::KC_UP)) {
			if (up == true) {
				player->setLinearVelocity(Vector3(0, 0, 0));
				up = false;
				setAnimation(0);
			}
		}
		if (!mKeyboard->isKeyDown(OIS::KC_DOWN)) {
			if(back==true){
				player->setLinearVelocity(Vector3(0, 0, 0));
				back = false;
				setAnimation(0);
			}
		}
		if (!mKeyboard->isKeyDown(OIS::KC_LEFT)) {
			if(left==true){
				player->setAngularVelocity(Vector3(0, 0, 0));
				left = false;
				if(up==false && back==false)setAnimation(0);
			}
		}
		if (!mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
			if(right==true){
				player->setAngularVelocity(Vector3(0, 0, 0));
				right = false;
				if (up == false && back == false)setAnimation(0);
			}
		}
		if (!mKeyboard->isKeyDown(OIS::KC_NUMPAD0)) {
			if (player->getLinearVelocity().y > -0.01 && player->getLinearVelocity().y< 0.01){
				jump = false;
			}

		}
		//States
		if (up==true && lock==false) {
			player->enableActiveState();
			Vector3  direction = mCamera->getDerivedDirection().normalisedCopy() * speed;		
			player->setLinearVelocity(Vector3(direction.x, player->getGravity().y,direction.z) );
		}

		if(back==true && lock == false){
			player->enableActiveState();
			Vector3  direction = mCamera->getDerivedDirection().normalisedCopy() * (-speed/2);
			player->setLinearVelocity(Vector3(direction.x, player->getGravity().y, direction.z));

		}
		if (left == true) {
			player->enableActiveState();
			player->setAngularVelocity(Vector3(0, 1, 0));
		}
		if (right == true) {
			player->enableActiveState();
			player->setAngularVelocity(Vector3(0, -1, 0));
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
	SceneNode* camNode;
	bool up;
	bool back;
	bool left;
	bool right;
	bool jump;
	bool lock;

	Ogre::Real speed;

	std::deque<OgreBulletDynamics::RigidBody *> mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	OgreBulletDynamics::DynamicsWorld *mWorld;
	int  mNumEntitiesInstanced;
	OgreBulletDynamics::RigidBody *player;
	Math3D math3D;
	//Animations
	static const int NUM_ANIMS = 13;
	Ogre::AnimationState* mAnimationStateTop;
	Ogre::AnimationState* mAnimationStateBase;
	Ogre::Entity* playerEntity;
	int animationId;
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,

		ANIM_RUN_BASE,
		ANIM_RUN_TOP,

		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,

		ANIM_DRAW_SWORDS,

		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,

		ANIM_DANCE,

		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

	
};


#endif