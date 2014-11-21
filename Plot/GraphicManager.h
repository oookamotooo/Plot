#ifndef _GRAPHIC_MANAGER_H_
#define _GRAPHIC_MANAGER_H_

#include "Graphics.h"

class GraphicManager
{
public :
	static GraphicManager* GetGraphic()
	{
		static GraphicManager instance;
		return &instance;
	}

	static void Initialize(int windowX, int windowY, int argc, char** argv);
	void DrawGrid();

	template<class T, typename S>
	void MakeGrid(Vector3<T> center, Vector3<S> size, Vector3i split)
	{
		if(grid == NULL)
			grid = new GridSpace( Vector3<float>(center.x, center.y, center.z), Vector3<float>(size.x, size.y, size.z), split);
	}
private:
	GridSpace *grid;
	GraphicManager();
};

#endif