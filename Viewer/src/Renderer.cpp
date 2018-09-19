#include "Renderer.h"
#include "InitShader.h"
#include <imgui/imgui.h>
#include <array>
#include <iostream>
#include "MeshModel.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
extern glm::vec3 ObjColor; // = glm::vec4(1.0f, 0.0f, 1.0f, 1.00f);
extern bool rotateX, rotateY, rotateZ;
extern float sx;
extern float sy, sz;
extern float scaler;
extern float theta_x, theta_y, theta_z;
extern bool rotatebytheta, showNormals;
extern bool translating;
extern float tx;
extern float ty;
extern float tz;
Renderer::Renderer() : width(1280), height(720)
{
	initOpenGLRendering();
	createBuffers(1280,720);
}

Renderer::Renderer(int w, int h) : width(w), height(h)
{
	initOpenGLRendering();
	createBuffers(w,h);
}

Renderer::~Renderer()
{
	delete[] colorBuffer;
}

void Renderer::DrawTriangles(const vector<glm::vec4>* vertices, const vector<glm::vec3>* normals)
{
	int size = vertices->size();	// get size of array
	//std::cout << "size is: " << size << std::endl;
	
	float *Zdepth = new float[width*height];
	for (int i = width * height; i--; Zdepth[i] = std::numeric_limits<float>::max());



	for (int i = 0; i < size/3; i++)				// draw triangles of 3 verticies at a time
	{
		glm::vec3 pointA = (*vertices)[3*i];	// get first point
		glm::vec3 pointB = (*vertices)[3*i+1];	// get second point
		glm::vec3 pointC = (*vertices)[3*i+2];	// get third point
		
		pointA.x *= (float)height / width;	// correct the ratio according to window change
		pointB.x *= (float)height / width;
		pointC.x *= (float)height / width;

		// draw 3 lines
		drawLine(pointA, pointB);		// draw the 3 lines
		drawLine(pointB, pointC);
		drawLine(pointC, pointA);


		glm::vec3 V1 = (pointA - pointB);
		glm::vec3 V2 = (pointC - pointB);
		glm::vec3 surfaceNormal;
		
		surfaceNormal = glm::normalize(glm::cross(V1, V2));

	


		//surfaceNormal.x = (V1.y * V2.z) - (V1.z * V2.y);
		//surfaceNormal.y = ((V1.z * V2.x) - (V1.x * V2.z));
		//surfaceNormal.z = (V1.x * V2.y) - (V1.y * V2.x);

		//surfaceNormal.x =    ((pointB.y - pointA.y)*(pointC.z- pointA.z)) - ((pointC.y - pointA.y)*(pointB.z - pointA.z));
		//surfaceNormal.y = ((pointB.z - pointA.z)*(pointC.x - pointA.x)) - ((pointB.x - pointA.x)*(pointC.z - pointA.z));
		//surfaceNormal.z = ((pointB.x - pointA.x)*(pointC.y - pointA.y)) - ((pointC.x - pointA.x)*(pointB.y - pointA.y));

		glm::vec3 center;

		center.x = ((pointA.x + pointB.x + pointC.x) / 3);
		center.y = ((pointA.y + pointB.y + pointC.y) / 3);
		center.z = ((pointA.z + pointB.z + pointC.z) / 3);		// can we do as 1 line for center?

		//float normX = abs(surfaceNormal.x);
		//float normY = abs(surfaceNormal.y);
		//float normZ = abs(surfaceNormal.z);
		//
		//glm::vec3 normalResult = surfaceNormal / (sqrt(normX*normX + normY * normY + normZ * normZ));
		//glm::vec3 normSubtract = normalResult - center;
		//float normSize = (sqrt(normSubtract.x*normSubtract.x + normSubtract.y*normSubtract.y + normSubtract.z*normSubtract.z));
		//
		//glm::vec3 B = normalResult;
		//B *= 1 - (1.1 / normSize);


		if (showNormals) 
		{
			surfaceNormal.x *= (float)height / width ;	// screen ratio
			center.x *= (float)height / width ;	// screen ratio
			drawLine(center, (surfaceNormal+center));
		}

		// should we normalize??

	


		glm::vec3 bottomLeft, upperRight;
		float mostleft=pointA.x, mostright=pointA.x, mostbottom=pointA.y, mostupper=pointA.y;
		//mostleft
		mostleft = ((pointA.x < pointB.x) ? pointA.x : pointB.x);
		mostleft = ((mostleft < pointC.x) ? mostleft : pointC.x);


		//mostright
		mostright = ((pointA.x > pointB.x) ? pointA.x : pointB.x);
		mostright = (mostright > pointC.x) ? mostright : pointC.x;
		
		//mostbottom
		mostbottom = ((pointA.y < pointB.y) ? pointA.y : pointB.y);
		mostbottom = (mostbottom < pointC.y) ? mostbottom : pointC.y;
		
		//mostupper
		mostupper = ((pointA.y > pointB.y) ? pointA.y : pointB.y);
		mostupper = ((mostupper > pointC.y) ? mostupper : pointC.y);

		//setting the rictangle dots
		bottomLeft.x = mostleft; bottomLeft.y = mostbottom; bottomLeft.z = 0;
		upperRight.x = mostright; upperRight.y = mostupper; upperRight.z= 0;


		int ml = ((mostleft + 1) * width / 2);
		int mu = ((mostupper + 1) * height / 2);
		int mr = ((mostright + 1) * width / 2);
		int mb = ((mostbottom + 1) * height / 2);

		int Ax = ((pointA.x + 1) * width / 2);
		int Ay = ((pointA.y + 1) * height / 2);
		int Bx = ((pointB.x + 1) * width / 2);
		int By = ((pointB.y + 1) * height / 2);
		int Cx = ((pointC.x + 1) * width/ 2);
		int Cy = ((pointC.y + 1) * height / 2);
		float myZ;
		glm::vec4 magenta = glm::vec4(1, 0, 1, 1);

		float L1, L2, L3, det;
		det = (By - Cy)*(Ax - Cx) + (Cx - Bx)*(Ay - Cy);
		
		for (int y = mb; y < mu; y++)				//y value
			for (int x = ml; x < mr; x++)			//x value
			{
				L1 = (By - Cy)*(x - Cx) + (Cx - Bx)*(y - Cy);
				L1 /= det;

				L2 = (Cy - Ay)*(x - Cx) + (Ax - Cx)*(y - Cy);
				L2 /= det;

				L3 = 1 - L1 - L2;
				if ((L1 >= 0 && L1 <= 1) && (L2 >= 0 && L2 <= 1) && (L3 >= 0 && L3 <= 1))
				{
					
					//putPixel(x, y, ObjColor);
					myZ = L1 * pointA.z + L2 * pointB.z + L3 * pointC.z;
					
					if ((x + (y*width) >= 0) && ((x+(y*width)) < width*height) && (myZ < Zdepth[x + (y*width)]))
					  {
						
						putPixel(x, y, ObjColor);
					  	Zdepth[x+y*width] = myZ;
					  }
					 
				}
			}

	}
	delete Zdepth;
}



float Renderer::getZ(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, int x, int y)
{
	glm::vec3 v1, v2, n;
	float k, t, z;

	v1 = p1 - p2;
	v2 = p1 - p3;
	n.x = v1.y*v2.z - v1.z*v2.y;
	n.y = -(v1.x*v2.z - v1.z*v2.x);
	n.z = v1.x*v2.y - v1.y*v2.x;
	k = n.x*p1.x + n.y*p1.y + n.z*p1.z;
	t = 1 / n.z;
	z = t * (k - n.x*x - n.y*y);
	return z;
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	colorBuffer[INDEX(width, i, j, 0)] = color.x;
	colorBuffer[INDEX(width, i, j, 1)] = color.y;
	colorBuffer[INDEX(width, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int w, int h)
{
	createOpenGLBuffer(); //Do not remove this line.
	colorBuffer = new float[3*w*h];
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			putPixel(i, j, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::SetDemoBuffer()
{
	int r = 5;
	// Wide red vertical line
	glm::vec4 red = glm::vec4(1, 0, 0, 1);
	for (int i = 0; i<height; i++)
	{
		for (int r0 = 0; r0 < r; r0++)
		{
			putPixel((width / 2) + r0, i, red);
			putPixel((width / 2) - r0, i, red);
		}
	}
	
	drawLine(glm::vec3(0, 0, 0), glm::vec3(width, height, 0));


	
	// Wide magenta horizontal line
	glm::vec4 magenta = glm::vec4(1, 0, 1, 1);
	for (int i = 0; i<width; i++)
	{
		for (int r0 = 0; r0 < r; r0++)
		{
			putPixel(i, (height / 2) + r0, magenta);
			putPixel(i, (height / 2) - r0, magenta);
		}

	}
}

// Draw a line using bresenham algorithm
void Renderer::drawLine(const glm::vec3 &p1, const glm::vec3 &p2)
{
	auto swap = [](int &a, int &b) {int temp = a; a = b; b = temp; };

	glm::vec3 white = glm::vec3(1, 1, 1);
	glm::vec3 black = glm::vec3(0, 0, 0);

	int x1 = ((p1.x + 1) * width / 2);
	int y1 = ((p1.y + 1) * height / 2);
	int x2 = ((p2.x + 1) * width / 2);
	int y2 = ((p2.y + 1) * height / 2);
	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	int x = x1;
	int y = y1;
	int y22 = y2;
	
	int sign_y = 1;

	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int e = -deltaX;

	int x_correct = x1;
	int y_correct = y1;

	bool swapflag = false;
	bool algoflag = false;

	if (deltaY < 0)		// for slope between o to -45.
		sign_y = -1;
	if (deltaX < abs(deltaY))
	{
		swap(x1, y1);
		swap(x2, y2);
		if (x1 > x2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		swap(deltaX, deltaY);
		//e = -deltaX;
		swapflag = true;
	}

	y2 += -y1;
	x2 += -x1;
	x1 = 0;
	y1 = 0;
	
	if ((deltaY > deltaX) && (x > 0 && y > 0))
		algoflag = true;

	if (x2 < 0)
	{
		x2 = abs(x2);
	}
	
	if(!algoflag)
		while (x1 <= (x2))
	{
		if (e > 0)
		{
			y1 = (y1 + 1);
			e = e - 2 * abs(deltaX);
		}

		if (!swapflag) {
			putPixel(x1 + x_correct, (sign_y*y1) + y_correct, black);
		}
		else{
			putPixel(y1+x_correct, (sign_y*x1 + y_correct), black);
			if (deltaY > deltaX)
				sign_y = 1;
		}

		x1 = x1 + 1;
		e = e + (2 * (deltaY)*sign_y);
	}

	else	
		while (x1 <= (x2))
	{
		if (e > 0)
		{
			y1 = (y1 + 1);
			e = e - 2 * abs(deltaX);
		}

		if (!swapflag) {
			putPixel(x1 + x_correct, (sign_y*y1) + y_correct, black);
		}
		else {
			putPixel(y1 + x_correct, (-sign_y*x1 + y_correct), black);
			if (deltaY > deltaX)
				sign_y = 1;
		}

		x1 = x1 + 1;
		e = e + (2 * (deltaY)*sign_y);
	}
}




void Renderer::SetBuffer()
{
	
}




//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);
	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);
	GLuint buffer;
	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);
	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);
	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);
	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	// Make this program the current one.
	glUseProgram( program );
	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );
	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);
	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);
	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, width, height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);
	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);
	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, colorBuffer);
	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);
	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);
	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::Viewport(int w, int h)
{
	if (w == width && h == height)
	{
		return;
	}
	width = w;
	height = h;
	delete[] colorBuffer;
	colorBuffer = new float[3 * h*w];
	createOpenGLBuffer();
}
