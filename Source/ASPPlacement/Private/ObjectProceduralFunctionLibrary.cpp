// Fill out your copyright notice in the Description page of Project Settings.

#include "ASPPlacement.h"
#include "ObjectProceduralFunctionLibrary.h"
#include <memory>
#include <math.h>

typedef std::unique_ptr<Wall>  UWall;
typedef std::unique_ptr<Worldobject>  UWorldobject;
bool UObjectProceduralFunctionLibrary::StartSession(const int32 val) {
	UWall pwalla = UWall(new Wall(Wall::Name::a, 100));
	return true;
}

bool UObjectProceduralFunctionLibrary::Placement(float unit,float WWidth, float RSx,float RSy,TArray<FSObjectDescription> lobjects, TArray<FSWallConstraint> lwctrs, TArray<FSObjectDescription> &newlobjects) {

	

	std::vector<UWorldobject> vobjects;
	std::vector<UWall> vwalls;

	
	
	int32 id = 0;
	/* Construimos un vector de objetos versión ASPLibrary */
	for (auto ObjectIterator = lobjects.CreateIterator(); ObjectIterator; ObjectIterator++) { // Recorremos la lista de objetos versión UE4 recibida por la función
		
		
		++id; // Identificador para cada objeto ASPLibrary
		
		FSObjectDescription odesc;
		odesc = *ObjectIterator;
		
		newlobjects.Add(odesc); // Copiamos en el objeto en la lista de salida
		newlobjects.Last().liobjects = TArray<FSObjectInstanceDescription>(); // En la nueva lista no hay objetos instanciados
		std::list<int> ldim;
		TArray<FSObjectInstanceDescription> liobjects = odesc.liobjects;
		int32 iid = 0;
		for (auto ObjectInstanceIterator = liobjects.CreateIterator(); (iid<MAXOBJECTINSTANCES) && ObjectInstanceIterator; ObjectInstanceIterator++) {
			
			UWorldobject pwobj = UWorldobject(new Worldobject((id-1)*MAXOBJECTINSTANCES+iid)); //Creamos el objeto
			++iid;
			pwobj->setdim(ceil(odesc.dimensions.X*odesc.scale / unit), ceil(odesc.dimensions.Y*odesc.scale / unit), ceil(odesc.dimensions.Z*odesc.scale / unit)); //Asignamos las dimensiones teniendo en cuenta la escala
			pwobj->settowall(facetolibface(odesc.CtrFaceToWall)); //Asignamos la pared a la que debe circunscribirse el objeto
			vobjects.push_back(std::move(pwobj)); // Agregamos al vector.
		}
		
	}

	int dimh = floor((RSx - 2*WWidth) / unit); // Dimensiones de la habitación en unidades ASPLibrary (restamos anchura del muro)
	int dimv = floor((RSy - 2*WWidth) / unit);
	UWall pwalla = UWall(new Wall(Wall::Name::a, dimh)); // Creamos los muros ASPLibrary de la habitación
	UWall pwallb = UWall(new Wall(Wall::Name::b, dimv));
	UWall pwallc = UWall(new Wall(Wall::Name::c, dimh));
	UWall pwalld = UWall(new Wall(Wall::Name::d, dimv));
	
	// Leemos las restricciones UE4 y se las añadimos al muro ASPLibrary que le corresponda.

	int ctr[3];
	for (auto CtrIterator = lwctrs.CreateIterator(); CtrIterator; CtrIterator++) {
		FSWallConstraint ctrwall = *CtrIterator;
		Wall::Name wname = walltolibwall(ctrwall.wall);
		ctr[0] = (std::floor(ctrwall.placementconstraint.X / unit)); //Inicio de la restricción
		ctr[1] = (std::ceil(ctrwall.placementconstraint.Y / unit)); // Fin de la restricción
		ctr[2] = (std::floor(ctrwall.placementconstraint.Z / unit)); //Altura de la restricción

		switch (wname) {

		case Wall::Name::a:
			pwalla->addnewarea(ctr);
			break;
		case Wall::Name::b:
			pwallb->addnewarea(ctr);
			break;
		case Wall::Name::c:
			pwallc->addnewarea(ctr);
			break;
		case Wall::Name::d:
			pwalld->addnewarea(ctr);
			break;


		}

	}

	// Creamos el vector de muros

	vwalls.push_back(std::move(pwalla));
	vwalls.push_back(std::move(pwallb));
	vwalls.push_back(std::move(pwallc));
	vwalls.push_back(std::move(pwalld));

	// Creamos la habitación asignándoles muros y objetos a ubicar.
	Room myroom(vwalls, vobjects);

	// Asignamos dimensiones generales a la habitación

	myroom.setdimh(dimh);
	myroom.setdimv(dimv);

	// --------------> Aquí es donde se realiza todo el trabajo de ASPLibrary <--------------------------------------
	
	myroom.placeObjects();

	// -------------> Vaciamos la habitación de objetos y volcamos los objetos en el vector vobjects.
	myroom.moveobjects(vobjects);

	// Modificamos la posición de los objetos.
	for (std::vector<std::unique_ptr<Worldobject>>::iterator it = vobjects.begin(); it != vobjects.end(); ++it) {
		FVector pposition = { 0.0,0.0,0.0 };
		FEFaces protation=FEFaces::None;
		FEWalls pwall = FEWalls::None;
		
		int32 idobject = ((*it)->getid() / MAXOBJECTINSTANCES)+1;
		Worldobject::Placement p = (*it)->getplacement();
		int32 x = p.x;
		int32 y = p.y;
		Worldobject::Rotation rot = p.rot;
		Wall::Name wall = p.wall;
		int32 ppos = p.wr; // La distancia es en unidades no en cm.
		pposition.X = x * unit;
		pposition.Y = y * unit;
		switch (rot) {
		case Worldobject::Rotation::a:
			protation = FEFaces::WallA;
			break;
		case Worldobject::Rotation::b:
			protation = FEFaces::WallB;
			break;
		case Worldobject::Rotation::c:
			protation = FEFaces::WallC;
			break;
		case Worldobject::Rotation::d:
			protation = FEFaces::WallD;
			break;
		default:
			protation = FEFaces::None;


		}
		switch (wall) {
		case Wall::Name::a:
			pwall = FEWalls::WallA;
			break;
		case Wall::Name::b:
			pwall = FEWalls::WallB;
			break;
		case Wall::Name::c:
			pwall = FEWalls::WallC;
			break;
		case Wall::Name::d:
			pwall = FEWalls::WallD;
			break;
		default:
			pwall = FEWalls::None;
		}

		//FSObjectInstanceDescription *tempstr = new FSObjectInstanceDescription;
		FSObjectInstanceDescription tempstr;
		tempstr.position.X = pposition.X;
		tempstr.position.Y = pposition.Y;
		tempstr.position.Z = 0;
		tempstr.rotation = protation;
		tempstr.wall = pwall;
		tempstr.distanceFromLeftCorner = ppos;

		newlobjects[idobject - 1].liobjects.Add(tempstr);

		/*
		newlobjects[idobject-1].position.X = pposition.X;
		newlobjects[idobject-1].position.Y = pposition.Y;
		newlobjects[idobject-1].rotation = protation;
		newlobjects[idobject - 1].wall = pwall;
		newlobjects[idobject - 1].distanceFromLeftCorner = ppos;
		*/
	}

	// Destrucción!
	for (auto ObjectIterator = lobjects.CreateIterator(); ObjectIterator; ObjectIterator++) { // Recorremos la lista de objetos versión UE4 recibida por la función



		FSObjectDescription odesc;
		odesc = *ObjectIterator;
		odesc.liobjects.Empty();


	}
	lobjects.Empty();
	return true;
}

void UObjectProceduralFunctionLibrary::fvectortolint(float unit, FVector dim, std::list<int>& lint) {

	lint.clear();
	lint.push_back((int)(std::ceil(dim.X/unit)));
	lint.push_back((int)(std::ceil(dim.Y / unit)));
	lint.push_back((int)(std::ceil(dim.Z / unit)));

}


Wall::Name UObjectProceduralFunctionLibrary::walltolibwall(FEWalls wall) {

	switch (wall) {

	case FEWalls::WallA: return Wall::Name::a;
	case FEWalls::WallB: return Wall::Name::b;
	case FEWalls::WallC: return Wall::Name::c;
	case FEWalls::WallD: return Wall::Name::d;
	default: return Wall::Name::a;
	}

	return Wall::Name::a;


}

Wall::Name UObjectProceduralFunctionLibrary::facetolibface(FEFaces face) {

	switch (face) {

	case FEFaces::WallA: return Wall::Name::a;
	case FEFaces::WallB: return Wall::Name::b;
	case FEFaces::WallC: return Wall::Name::c;
	case FEFaces::WallD: return Wall::Name::d;
	default:
		return Wall::Name::a;

	}

	return Wall::Name::a;


}

