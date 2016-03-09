#ifndef TERRAINGEN_H
#define TERRAINGEN_H
#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "string.h" // for the memcpy
//Bullet -comment for lecture
#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "Shapes/OgreBulletCollisionsCapsuleShape.h"
#include "Shapes/OgreBulletCollisionsTerrainShape.h" //terrain
#include "bulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "PhysicsManager.h"
using namespace Ogre;
class TerrainGen {

public:
	TerrainGen(Ogre::Root* _mRoot, Ogre::RenderWindow* _mWindow, Ogre::Camera* _mCamera, Ogre::SceneManager* _mSceneMgr, Ogre::Light* _directionalLight) {
		mRoot = _mRoot;
		mWindow = _mWindow;
		mCamera = _mCamera;
		mSceneMgr = _mSceneMgr;
		directionalLight = _directionalLight;
		//Terrain defs
		terrainSize=513;
		terrainWorldSize=2046.0f;
		inputScale = 300;
		minBatchSize = 33;
		maxBatchSize = 65;
		img.load("map_04.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	void loadTerrain() {
		// Fog
		/*Ogre::ColourValue fadeColour(.9, .9, .9);
		mWindow->getViewport(0)->setBackgroundColour(fadeColour);

		mSceneMgr->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);
		*/
		MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
		MaterialManager::getSingleton().setDefaultAnisotropy(7);
		// Terrain
		mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

		mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
			mSceneMgr,
			Ogre::Terrain::ALIGN_X_Z,
			terrainSize, terrainWorldSize);
		mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
		mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
		//Terrain defaults
		//configureTerrainDefaults(directionalLight);
		mTerrainGlobals->setMaxPixelError(8);
		mTerrainGlobals->setCompositeMapDistance(3000);

		mTerrainGlobals->setLightMapDirection(directionalLight->getDerivedDirection());
		mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
		mTerrainGlobals->setCompositeMapDiffuse(directionalLight->getDiffuseColour());

		Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
		importData.terrainSize = terrainSize;
		importData.worldSize = terrainWorldSize;
		importData.inputScale = inputScale;
		importData.minBatchSize = minBatchSize;
		importData.maxBatchSize = maxBatchSize;

		importData.layerList.resize(3);
		importData.layerList[0].worldSize = 100;
		importData.layerList[0].textureNames.push_back(
			"dirt_grayrocky_diffusespecular.dds");
		importData.layerList[0].textureNames.push_back(
			"dirt_grayrocky_normalheight.dds");
		importData.layerList[1].worldSize = 30;
		importData.layerList[1].textureNames.push_back(
			"grass_green-01_diffusespecular.dds");
		importData.layerList[1].textureNames.push_back(
			"grass_green-01_normalheight.dds");
		importData.layerList[2].worldSize = 200;
		importData.layerList[2].textureNames.push_back(
			"growth_weirdfungus-03_diffusespecular.dds");
		importData.layerList[2].textureNames.push_back(
			"growth_weirdfungus-03_normalheight.dds");


		//
		Ogre::String filename = mTerrainGroup->generateFilename(0, 0);

		bool exists =
			Ogre::ResourceGroupManager::getSingleton().resourceExists(
				mTerrainGroup->getResourceGroup(),
				filename);

		if (exists) {
			mTerrainGroup->defineTerrain(0, 0);
		}
		else
		{
			
			mTerrainGroup->defineTerrain(0, 0, &img);
			mTerrainsImported = true;
		}
		mTerrainGroup->loadAllTerrains(true);
		//
		if (mTerrainsImported)
		{
			TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
			while (ti.hasMoreElements())
			{
				Terrain* t = ti.getNext()->instance;
				initBlendMaps(t);
			}
		}
		//
		mTerrainGroup->freeTemporaryResources();

		//sky
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	}
	void initBlendMaps(Ogre::Terrain* terrain)
	{
		Ogre::Real minHeight0 = 70;
		Ogre::Real fadeDist0 = 40;
		Ogre::Real minHeight1 = 70;
		Ogre::Real fadeDist1 = 15;

		Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
		Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

		float* pBlend0 = blendMap0->getBlendPointer();
		float* pBlend1 = blendMap1->getBlendPointer();

		for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
		{
			for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
			{
				Ogre::Real tx, ty;

				blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
				Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
				Ogre::Real val = (height - minHeight0) / fadeDist0;
				val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
				*pBlend0++ = val;

				val = (height - minHeight1) / fadeDist1;
				val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
				*pBlend1++ = val;
			}
		}

		blendMap0->dirty();
		blendMap1->dirty();
		blendMap0->update();
		blendMap1->update();

	}
	void bulletTerrainshape(PhysicsManager * physicsManager) {
		unsigned page_size = mTerrainGroup->getTerrainSize();
		Ogre::Vector3 terrainScale(terrainWorldSize / (page_size - 1),
			inputScale,
			terrainWorldSize / (page_size - 1));

		float *heights = new float[page_size*page_size];
		for (unsigned y = 0; y < page_size; ++y)
		{
			for (unsigned x = 0; x < page_size; ++x)
			{
				Ogre::ColourValue color = img.getColourAt(x, y, 0);
				heights[x + y * page_size] = color.r;
			}
		}
		//MY
		OgreBulletCollisions::CollisionShape *mTerrainShape;
		OgreBulletDynamics::RigidBody *defaultTerrainBody;
		//
		mTerrainShape = new OgreBulletCollisions::HeightmapCollisionShape(page_size,
			page_size,
			terrainScale,
			heights,
			true);

		defaultTerrainBody = new OgreBulletDynamics::RigidBody("Terrain",
			physicsManager->getMWorld());

		const float terrainBodyRestitution = 0.1f;
		const float terrainBodyFriction = 0.8f;

		Ogre::Vector3 terrainShiftPos(terrainScale.x / (page_size - 1), 0, terrainScale.z / (page_size - 1));
		terrainShiftPos.y = terrainScale.y / 2 * terrainScale.y;

		Ogre::SceneNode* pTerrainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		defaultTerrainBody->setStaticShape(pTerrainNode, mTerrainShape, terrainBodyRestitution, terrainBodyFriction, terrainShiftPos);

		physicsManager->addMbodies(defaultTerrainBody);
		physicsManager->addMshapes(mTerrainShape);

	}
	

private:
	Ogre::Root* mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;	

	bool mTerrainsImported;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::Light* directionalLight;
	Ogre::Image img;
	//Terrain defs
	Ogre::uint16 terrainSize;
	Ogre::Real terrainWorldSize;
	Ogre::Real inputScale;
	Ogre::Real minBatchSize;
	Ogre::Real maxBatchSize;

};
/*Lecture 3 test*/

#endif