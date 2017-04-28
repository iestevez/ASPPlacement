#include "Wall.h"
#include <list>

#ifndef DEF_WORLDOBJECT_CLASS
#define DEF_WORLDOBJECT_CLASS

constexpr int Minsize=1;
constexpr int Maxsize=1;
constexpr int Defaultheight=1;
constexpr int Defaultdim=1;

class Worldobject {

int id; // Unique identification (problem context)
Wall::Name towall=Wall::Name::none; // Constraint: this side must be against wall
int dimx=Defaultdim; // X Dimension 
int dimy=Defaultdim; // Y Dimension
int height=Defaultheight; // Constraint: height considered for height constraints
public:	
	enum class Rotation {none=0,a=1,b=2,c=3,d=4};
	Worldobject(int id);	
	void settowall(Wall::Name w);
	void setdim(std::initializer_list<int> l);
	void setdim(int dx, int dy, int dz);
	void setplacement(int x, int y, Rotation  rot,  Wall::Name wall, int wr);
	void setunplaced();
	int getdimx() const;
	int getdimy() const;
	int getid() const;
	int getheight() const;
	Wall::Name gettowall() const;
	static const char* RotationStrings[];
	void print() const;
	static Rotation getrotationfromstring(std::string srot);
	struct Placement {
		int x;
		int y;
		Rotation rot;
		int wr; // Wall relative
		Wall::Name wall; // Wall relative
	};
	Placement getplacement() const;


private:
// Placement private attributes
bool placement=false; // If true, placement was possible.
int x=0; // x position after placement
int y=0; // y position after placement
Rotation rotation=Rotation::none;
int wr = 0; // Wall relative placement
Wall::Name wall; //Wall relative


 };

#endif //DEF_WORLDOBJECT_CLASS
