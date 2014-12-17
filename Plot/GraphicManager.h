#ifndef _GRAPHIC_MANAGER_H_
#define _GRAPHIC_MANAGER_H_

#include "Graphics.h"
#include "Field.h"

class GraphicManager
{
public :

	static GraphicManager* GetGraphic(){
		static GraphicManager instance;
		return &instance;
	}

	static void Initialize(int windowX, int windowY, int argc, char** argv);
	void Draw();

	void MakeGrid(Vector3f center, Vector3f size, Vector3i split);
	Field* const MakeField(const Vector3i &lftBtmNear, const Vector3i &size);
	Field* const GetField() const { return field; }

	void AddToRoot(Graphic *child)
	{
		root->AddChild(child);
	}

private:
	GridSpace *grid;
	Field *field;
	GraphicManager();
	Graphic *root;
	std::vector<Graphic*> graphics; 
};

#endif