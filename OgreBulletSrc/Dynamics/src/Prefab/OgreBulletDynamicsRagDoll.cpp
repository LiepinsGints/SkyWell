/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreBulletDynamics.h"

#include "OgreBulletCollisionsShape.h"
#include "OgreBulletCollisionsObject.h"
#include "OgreBulletCollisionsWorld.h"
#include "OgreBulletCollisionsObjectState.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Prefab/OgreBulletDynamicsRagDoll.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletDynamics
{

	// -------------------------------------------------------------------------
    RagDoll::RagDoll(btDynamicsWorld* ownerWorld, const btVector3& positionOffset)
		: m_ownerWorld (ownerWorld)
	{
		// Setup all the rigid bodies
		btTransform offset; offset.setIdentity();
		offset.setOrigin(positionOffset);

		btTransform transform;

		// for
		{
			transform.setIdentity();
            transform.setOrigin(btVector3(btScalar(0.f), btScalar(1.f), btScalar(0.f)));
            m_bodies.push_back(localCreateRigidBody(btScalar(1.f),
                                                    offset * transform,
                                                    new btCapsuleShape(btScalar(0.15f), btScalar(0.2f))));
		}

		// Setup some damping on the m_bodies
        for(std::vector<btRigidBody* >::iterator i = m_bodies.begin(); i != m_bodies.end(); ++i)
		{
            (*i)->setDamping(btScalar(0.05f), btScalar(0.85f));
            (*i)->setDeactivationTime(btScalar(0.8f));
            (*i)->setSleepingThresholds(btScalar(1.6f), btScalar(2.5f));
		}

		// Now setup the constraints
        btHingeConstraint *hingeC;
		//btConeTwistConstraint* coneC;
		btTransform localA, localB;

		// for
		{
			localA.setIdentity(); localB.setIdentity();
            localA.getBasis().setEulerZYX(0.f, Ogre::Math::TWO_PI, 0.f);
            localA.setOrigin(btVector3(btScalar(0.f), btScalar(0.15f), btScalar(0.f)));
            localB.getBasis().setEulerZYX(0.f, Ogre::Math::TWO_PI, 0.f);
            localB.setOrigin(btVector3(btScalar(0.f), btScalar(-0.15f), btScalar(0.f)));
            hingeC = new btHingeConstraint(*m_bodies[0], *m_bodies[1], localA, localB);
            hingeC->setLimit(btScalar(-Ogre::Math::TWO_PI * 2), btScalar(Ogre::Math::TWO_PI));
			m_joints.push_back(hingeC);
			m_ownerWorld->addConstraint(hingeC, true);
		}
	}
    // -------------------------------------------------------------------------
    RagDoll::~RagDoll()
	{
		std::vector<btCollisionShape* >  m_shapes;
		std::vector<btRigidBody* >		 m_bodies;
		std::vector<btTypedConstraint* > m_joints;
		// Remove all constraints
        for (std::vector<btTypedConstraint* >::iterator i = m_joints.begin(); i != m_joints.end(); ++i)
		{
			m_ownerWorld->removeConstraint(*i);
			delete *i;
		}

		// Remove all bodies and shapes
        for (std::vector<btRigidBody* >::iterator i = m_bodies.begin(); i != m_bodies.end(); ++i)
		{
			m_ownerWorld->removeRigidBody(*i);

			delete (*i)->getMotionState();
			delete *i;
		}
        for (std::vector<btCollisionShape* >::iterator i=m_shapes.begin(); i != m_shapes.end(); ++i)
		{
			delete *i;
		}
	}
	// -------------------------------------------------------------------------
    btRigidBody *RagDoll::localCreateRigidBody(btScalar mass, const btTransform &startTransform, btCollisionShape *shape)
	{
		bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0.f, 0.f, 0.f);
		if (isDynamic)
        {
			shape->calculateLocalInertia(mass,localInertia);
        }

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_ownerWorld->addRigidBody(body);

		return body;
	}
}

