# Install script for directory: F:/Source/OgreBullet

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/OgreBullet")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/Debug/OgreBulletCollisions.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/Release/OgreBulletCollisions.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/MinSizeRel/OgreBulletCollisions.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/RelWithDebInfo/OgreBulletCollisions.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/Debug/OgreBulletDynamics.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/Release/OgreBulletDynamics.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/MinSizeRel/OgreBulletDynamics.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "G:/Projects/OgreBullet/RelWithDebInfo/OgreBulletDynamics.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Collisions" TYPE FILE FILES
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisionsRay.h"
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisions.h"
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisionsShape.h"
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisionsObjectState.h"
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisionsWorld.h"
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisionsObject.h"
    "F:/Source/OgreBullet/Collisions/include/OgreBulletCollisionsPreRequisites.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Collisions/Shapes" TYPE FILE FILES
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsMultiSphereShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsTriangleShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsCompoundShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsTerrainShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsStaticPlaneShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsSphereShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsConvexHullShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsMinkowskiSumShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsCapsuleShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsConeShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsTrimeshShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsBoxShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsGImpactShape.h"
    "F:/Source/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsCylinderShape.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Collisions/Debug" TYPE FILE FILES
    "F:/Source/OgreBullet/Collisions/include/Debug/OgreBulletCollisionsDebugContact.h"
    "F:/Source/OgreBullet/Collisions/include/Debug/OgreBulletCollisionsDebugDrawer.h"
    "F:/Source/OgreBullet/Collisions/include/Debug/OgreBulletCollisionsDebugLines.h"
    "F:/Source/OgreBullet/Collisions/include/Debug/OgreBulletCollisionsDebugShape.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Collisions/Utils" TYPE FILE FILES
    "F:/Source/OgreBullet/Collisions/include/Utils/OgreBulletCollisionsMeshToShapeConverter.h"
    "F:/Source/OgreBullet/Collisions/include/Utils/OgreBulletConverter.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Dynamics" TYPE FILE FILES
    "F:/Source/OgreBullet/Dynamics/include/OgreBulletDynamics.h"
    "F:/Source/OgreBullet/Dynamics/include/OgreBulletDynamicsWorld.h"
    "F:/Source/OgreBullet/Dynamics/include/OgreBulletDynamicsRigidBody.h"
    "F:/Source/OgreBullet/Dynamics/include/OgreBulletDynamicsConstraint.h"
    "F:/Source/OgreBullet/Dynamics/include/OgreBulletDynamicsObjectState.h"
    "F:/Source/OgreBullet/Dynamics/include/OgreBulletDynamicsPreRequisites.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Dynamics/Constraints" TYPE FILE FILES
    "F:/Source/OgreBullet/Dynamics/include/Constraints/OgreBulletDynamicsRaycastVehicle.h"
    "F:/Source/OgreBullet/Dynamics/include/Constraints/OgreBulletDynamics6DofSpringConstraint.h"
    "F:/Source/OgreBullet/Dynamics/include/Constraints/OgreBulletDynamicsConeTwistConstraint.h"
    "F:/Source/OgreBullet/Dynamics/include/Constraints/OgreBulletDynamics6DofConstraint.h"
    "F:/Source/OgreBullet/Dynamics/include/Constraints/OgreBulletDynamicsHingeConstraint.h"
    "F:/Source/OgreBullet/Dynamics/include/Constraints/OgreBulletDynamicsPoint2pointConstraint.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OgreBullet/Dynamics/Prefab" TYPE FILE FILES "F:/Source/OgreBullet/Dynamics/include/Prefab/OgreBulletDynamicsRagDoll.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/OgreBullet/cmake" TYPE FILE FILES "F:/Source/OgreBullet/FindOgreBullet.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "G:/Projects/OgreBullet/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
