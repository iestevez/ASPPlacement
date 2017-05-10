#include "Wall.h"
#include <string>
#include <iostream>

Wall::Wall(Name ori,int l):	length(l), orientation(ori) {};

Wall::Wall(Name ori,int l, std::vector<Maxarea_t>& v):	length(l),orientation(ori) {

for(auto el: v){

VMaxareas.push_back(el);

}
}

Wall::Wall(Name ori, int l, std::initializer_list<std::list<int>> ma_data): length(l), orientation(ori) {
		for(std::list<int> sl : ma_data){
			std::vector<int> na;
			for(auto x : sl)
				na.push_back(x);
			VMaxareas.push_back(na);
		}
}

Wall::~Wall(){
	//delallareas();
}

void Wall::addnewarea(Maxarea_t a) { VMaxareas.push_back(a); };

void Wall::set_length(int l){ length=l; };

void Wall::set_orientation(Wall::Name ori){orientation=ori;};

void Wall::delallareas() {
	VMaxareas.empty();
		

}

int Wall::get_length() const {return length;};

Wall::Name Wall::get_orientation() const {return orientation;};

void Wall::print() const {



std::cout<<"Wall orientation: "<<OrientationStrings[static_cast<std::size_t>(orientation)]<<std::endl;
std::cout<<"Wall dimensions: "<<length<<std::endl;
if(VMaxareas.size()>0){
	std::cout<<"Max. height constraints"<<std::endl;
	for(auto m : VMaxareas)
		std::cout<<"Start: "<<m.at(0)<<"End: "<<m.at(1) <<"Max. height: "<<m.at(2)<<std::endl;

}
}


const char* Wall::OrientationStrings[]={"none","a","b","c","d"};

Wall::Name Wall::getwallnamefromstring(std::string strwall){

Wall::Name ret = Wall::Name::a;

if(strwall=="a")
		ret=Wall::Name::a;
	else if(strwall=="b")
		ret=Wall::Name::b;
	else if(strwall=="c")
		ret=Wall::Name::c;
	else if(strwall=="d")
		ret=Wall::Name::d;
	else
		ret=Wall::Name::a;

return ret;
}


