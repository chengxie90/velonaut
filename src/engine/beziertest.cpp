#include "beziertest.h"

BezierTest::BezierTest()
{
    p0_ = Ogre::Vector3();
    p1_ = Ogre::Vector3();
    p2_ = Ogre::Vector3();
    p3_ = Ogre::Vector3();
}

BezierTest::BezierTest(Ogre::Vector3 p0, Ogre::Vector3 p1, Ogre::Vector3 p2, Ogre::Vector3 p3)
{
    p0_ = p0;
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
}

Ogre::Vector3 BezierTest::getPoint(double t)
{
    return Ogre::Vector3 ((1-t)*(1-t)*(1-t)*p0_) + (3*(1-t)*(1-t)*t*p1_) + (3*(1-t)*t*t*p2_) + (t*t*t*p3_);
}

Ogre::Vector3 BezierTest::getDerPoint(double t)
{
    return Ogre::Vector3(3*(1-t)*(1-t)*(p1_-p0_)) + (6*(1-t)*t*(p2_-p1_)) + (3*t*t*(p3_-p2_));
}
