#include "Renderer.h"
#include "InitShader.h"
#include <imgui/imgui.h>
#include <array>
#include <iostream>
#include "MeshModel.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
extern float sx;
extern float sy;
extern float scaler;
extern float theta;
extern bool rotatebytheta;
extern bool translating;
extern float tx;
extern float ty;
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

void Renderer::DrawTriangles(const vector<glm::vec3>* vertices, const vector<glm::vec3>* normals)
{
	
	int i = 0;
	int size = vertices->size();	// get size of array
	//std::cout << "size is: " << size << std::endl;

	while (i < size)				// draw triangles of 3 verticies at a time
	{
		glm::vec3 pointA = (*vertices)[i++];	// get first point
		glm::vec3 pointB = (*vertices)[i++];	// get second point
		glm::vec3 pointC = (*vertices)[i++];	// get third point

		glm::mat2x2 matA = glm::mat2x2(sx*scaler, 0, 0, sy*scaler);
		glm::vec2 transA = glm::vec2(pointA.x, pointA.y);
		glm::mat2x2 matB = glm::mat2x2(sx*scaler, 0, 0, sy*scaler);
		glm::vec2 transB = glm::vec2(pointB.x, pointB.y);
		glm::mat2x2 matC = glm::mat2x2(sx*scaler, 0, 0, sy*scaler);
		glm::vec2 transC = glm::vec2(pointC.x, pointC.y);

		glm::vec2 ansA = matA * transA;
		pointA.x = ansA.x;
		pointA.y = ansA.y;

		glm::vec2 ansB = matB * transB;
		pointB.x = ansB.x;
		pointB.y = ansB.y;

		glm::vec2 ansC = matC * transC;
		pointC.x = ansC.x;
		pointC.y = ansC.y;

		if (rotatebytheta)
		{
			glm::mat2x2 rotA = glm::mat2x2(cos(theta), -sin(theta), sin(theta), cos(theta));

			glm::vec2 rotationA = rotA * transA;
			pointA.x = rotationA.x;
			pointA.y = rotationA.y;

			glm::vec2 rotationB = rotA * transB;
			pointB.x = rotationB.x;
			pointB.y = rotationB.y;

			glm::vec2 rotationC = rotA * transC;
			pointC.x = rotationC.x;
			pointC.y = rotationC.y;
		}
		if (translating)
		{
			glm::mat3x3 translate = glm::mat3x3(1, 0, tx, 0, 1, ty, 0, 0, 1);
			glm::vec3 homA = glm::vec3(pointA.x, pointA.y, 1);
			glm::vec3 homB = glm::vec3(pointB.x, pointB.y, 1);
			glm::vec3 homC = glm::vec3(pointC.x, pointC.y, 1);

			glm::vec3 translatedA = translate * homA;
			pointA.x = translatedA.x / translatedA.z;
			pointA.y = translatedA.y / translatedA.z;

			glm::vec3 translatedB = translate * homB;
			pointB.x = translatedB.x / translatedB.z;
			pointB.y = translatedB.y / translatedB.z;

			glm::vec3 translatedC = translate * homC;
			pointC.x = translatedC.x / translatedC.z;
			pointC.y = translatedC.y / translatedC.z;
		}
		// draw 3 lines
		drawLine(pointA, pointB);		// draw the 3 lines
		drawLine(pointB, pointC);
		drawLine(pointC, pointA);
	}
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
			putPixel(x1 + x_correct, (sign_y*y1) + y_correct, white);
		}
		else{
			putPixel(y1+x_correct, (sign_y*x1 + y_correct), white);
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
			putPixel(x1 + x_correct, (sign_y*y1) + y_correct, white);
		}
		else {
			putPixel(y1 + x_correct, (-sign_y*x1 + y_correct), white);
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

// ----------------------------------------------- our added code

// Implement primitive triangle
//void Renderer::PrimMeshModel(glm::vec3 *vertexPosition)
//{
//	int i = 0;
//	//int size = vertexPosition->length;	// get size of array
//
//	while (i < 21)				// draw triangles of 3 verticies at a time
//	{
//		glm::vec3 pointA = vertexPosition[0];		// get first point
//		glm::vec3 pointB = vertexPosition[1];		// get second point
//		glm::vec3 pointC = vertexPosition[2];		// get third point
//
//		// draw 3 lines
//		drawLine(pointA, pointB);		// draw the 3 lines
//		drawLine(pointB, pointC);
//		drawLine(pointC, pointA);
//	}





