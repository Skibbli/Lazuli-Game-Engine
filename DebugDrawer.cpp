#include "stdafx.h"

#include "DebugDrawer.h"
#include "Resources.h"


GLDebugDrawer::GLDebugDrawer() : m_debugMode(0)
{
	
}

void GLDebugDrawer::Initialise()
{
	weak<Shader> shader = Resources::LoadShader("monolith");

	m_modelMat = Mat4(1.0f);
	//Linking variables in the program with the locations of the corresponding uniforms in the shaders
	m_modelMatLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "modelMatrix");
}

void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	///Send model, view and projection matrices to OpenGL
	glUniformMatrix4fv(m_modelMatLoc, 1, GL_FALSE, glm::value_ptr(m_modelMat));

	glBegin(GL_LINES);
            glColor3f(color.getX(), color.getY(), color.getZ());
            glVertex3d(from.getX(), from.getY(), from.getZ());
            glColor3f(color.getX(), color.getY(), color.getZ());
            glVertex3d(to.getX(), to.getY(), to.getZ());
    glEnd();
}

void GLDebugDrawer::setDebugMode(int debugMode)
{
   m_debugMode = debugMode;
}

void GLDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
   //glRasterPos3f(location.x(),  location.y(),  location.z());
   //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void GLDebugDrawer::reportErrorWarning(const char* warningString)
{
   printf(warningString);
}

void GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
   {
      //btVec3 to=pointOnB+normalOnB*distance;
      //const btVec3&from = pointOnB;
      //glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   
      
      //GLDebugDrawer::drawLine(from, to, color);
      
      //glRasterPos3f(from.x(),  from.y(),  from.z());
      //char buf[12];
      //sprintf(buf," %d",lifeTime);
      //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
   }
}