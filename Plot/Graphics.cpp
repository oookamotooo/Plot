#include "Graphics.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
using namespace std;

typedef Vector3<float> Vector3f;

GridSpace::GridSpace(Vector3<float> center, Vector3<float> size, Vector3i split)
{
	//分割数0以下は許されないので底上げする
	split.x = std::max( 1, split.x);
	split.y = std::max( 1, split.y);
	split.z = std::max( 1, split.z);

	//頂点分の領域を予約しておく
	vertex.reserve( 8 + 4*((split.x-1) + (split.y-1) + (split.z-1)) );

	//座標軸
	vertex.push_back( Vector3f(center.x, center.y, center.z - size.z/2) );	//x軸
	vertex.push_back( Vector3f(center.x, center.y, center.z + size.z/2) );
	vertex.push_back( Vector3f(center.x, center.y - size.y/2, center.z) );	//y軸
	vertex.push_back( Vector3f(center.x, center.y + size.y/2, center.z) );
	vertex.push_back( Vector3f(center.x - size.x/2, center.y, center.z) );	//z軸
	vertex.push_back( Vector3f(center.x + size.x/2, center.y, center.z) );

	//先に外枠の頂点を保存
	int d1[] = {0, 0, 1, 1};
	int d2[] = {0, 1, 1, 0};

	//分割部分の頂点を保存
	for(int i=0; i <= split.x; i++){
		for( int d=0; d<4; d++){
			vertex.push_back( Vector3<float>( center.x + size.x*( 1.0*i / split.x - 0.5),
				center.y + size.y*(d1[d] - 0.5),
				center.z + size.z*(d2[d] - 0.5)
				) );
		}
	}

	for(int j=0; j<=split.y; j++){
		for( int d=0; d<4; d++){
			vertex.push_back( Vector3<float>( center.x + size.x*( d1[d] - 0.5 ),
				center.y + size.y*( 1.0*j / split.y - 0.5),
				center.z + size.z*( d2[d] - 0.5)
				) );
		}
	}

	
	for(int k=1; k<split.z; k++){
		for( int d=0; d<4; d++){
			vertex.push_back( Vector3<float>( center.x + size.x*( d1[d] - 0.5 ),
				center.y + size.y*( d2[d] - 0.5),
				center.z + size.z*( 1.0*k / split.z - 0.5)
				) );
		}
	}
	
	//bufferを生成
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (GLuint)(vertex.size() * sizeof(Vector3<float>)), reinterpret_cast<GLfloat*>(&vertex[0]), GL_STATIC_DRAW);
}

#define BUFFER_OFFSET(bytes) ( (GLubyte*)NULL + (bytes))

void GridSpace::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector3f), BUFFER_OFFSET(0));

	for(int i=0; i<6; i+=2)
	{
		glColor3f( i==0, i==2, i==4);
		glDrawArrays(GL_LINES, i, 2);
	}

	glColor3d( 0, 0, 0);
	for(int i=6; i<vertex.size(); i+=4)
		glDrawArrays(GL_LINE_LOOP, i, 4 );

	glEnableClientState(GL_VERTEX_ARRAY);
}
