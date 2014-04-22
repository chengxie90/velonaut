#ifndef BEZIERTEST_H
#define BEZIERTEST_H

#include <OGRE/Ogre.h>

class BezierTest
{
public:
    BezierTest();
    BezierTest(Ogre::Vector3 p0, Ogre::Vector3 p1, Ogre::Vector3 p2, Ogre::Vector3 p3);

    Ogre::Vector3 p0_;
    Ogre::Vector3 p1_;
    Ogre::Vector3 p2_;
    Ogre::Vector3 p3_;

    Ogre::Vector3 getPoint(double t);
    Ogre::Vector3 getDerPoint(double t);

};

#endif // BEZIERTEST_H
