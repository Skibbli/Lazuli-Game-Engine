#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include "stdafx.h"

#include "LinearMath/btIDebugDraw.h"

/// Draws the collider for physics colliders
class GLDebugDrawer : public btIDebugDraw
{
        int m_debugMode;
      
   public:
      
        GLDebugDrawer();
            
        virtual void   drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
      
        virtual void   drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
      
        virtual void   reportErrorWarning(const char* warningString);
      
        virtual void   draw3dText(const btVector3& location,const char* textString);
      
        virtual void   setDebugMode(int debugMode);
      
        virtual int      getDebugMode() const { return m_debugMode;}

		void Initialise();

	private:
		GLuint m_modelMatLoc;
		glm::mat4 m_modelMat;   
};


//class MyDebugDraw: public btIDebugDraw
//{
//    int m_debugMode;
//
//public:
//    virtual void drawLine(const btVec3& from, const btVec3& to, const btVec3& color);
//    virtual void drawContactPoint(const btVec3& PointOnB, const btVec3& normalOnB,btScalar distance, int lifeTime,const btVec3& color);
//    virtual void reportErrorWarning(const char* warningString);
//    virtual void draw3dText(const btVec3& location,const char* textString);
//    virtual void setDebugMode(int debugMode);
//    virtual int  getDebugMode() const;
//};

#endif