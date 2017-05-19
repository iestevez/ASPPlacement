# ASPPlacement
Experimentos con ASP y Unreal Engine
------------------------------------

1. Objetivo: Probar librer�a Clingo para resoluci�n de problemas definidos bajo Answer Set Programming (ASP) en la generaci�n procedural de escenarios en Unreal Engine 4. En particular se trata de la colocaci�n de objetos que deben cumplir condiciones l�gicas estrictas.

2. Notas sobre migraci�n de los blueprints a otro proyecto:
-------------------------------------------------------------

Pasos a realizar en el caso de que se deseen usar las librer�as y Blueprints incluidos en este proyecto en otro proyecto de UE4:

1. En el proyecto destino necesitamos que exista al menos un clase C++, para que el editor UE4 cree la estructura adecuada para proyectos con clases C++. Por ejemplo, agregar una clase vac�a.

2. En el proyecto destino copiar en la carpeta Source los archivos ObjectProceduralFunctionLibrary.cpp y ObjectProceduralFuncionLibrary.h, que se encuentran en la carpeta correspondiente de este proyecto.

3. En el proyecto destino modificar en ObjectProceduralFuncionLibrary.cpp la cabecera #include "ASPPlacement.h": Cambiar a #include "Nombredelproyecto.h".

4. En el proyecto destino modificar en ObjectProceduralFunctionLibrary.h la primera l�nea de la definici�n de la clase UObjectProceduralFunctionLibrary. Debe pasar de:

class ASPPLACEMENT_API UObjectProceduralFunctionLibrary : public UBlueprintFunctionLibrary

a

class NOMBRENUEVOPROYECTO UObjectProceduralFunctionLibrary : public UBlueprintFunctionLibrary

5. Copiar en el proyecto destino la carpeta thirdpartylibraries del proyecto ASPPlacement.

6. Copiar en el proyecto destino el archivo clingo.dll a la carpeta Binaries.

7. En el proyecto destino, dentro de la subcarpeta del m�dulo en Source, modificar el archivo Nombredelproyecto.build.cs. Debe a�adirse lo que incluye adicionalmente ASPPlacement.Build.cs.

8. Abrir la soluci�n de Visual Studio del nuevo proyecto y compilar para comprobar que todo est� OK. Una vez comprobado cerrar el proyecto.

9. Dentro de la carpeta Config del proyecto destino hay que modificar el archivo DefaultEngine.ini, a�adiendo las siguientes l�neas (sustituyendo Nomredelproyecto por el nombre correcto del proyecto de destino):

[/Script/Engine.Engine] 
+ActiveClassRedirects=(OldClassName="FEFaces",NewClassName="/Script/Nombredelproyecto.FEFaces")
+ActiveClassRedirects=(OldClassName="FEWalls",NewClassName="/Script/Nombredelproyecto.FEWalls")
+ActiveClassRedirects=(OldClassName="SObjectInstanceDescription",NewClassName="/Script/Nombredelproyecto.SObjectInstanceDescription")
+ActiveClassRedirects=(OldClassName="SObjectDescription",NewClassName="/Script/Nombredelproyecto.SObjectDescription")
+ActiveClassRedirects=(OldClassName="SWallConstraint",NewClassName="/Script/Nombredelproyecto.SWallConstraint")

10. En el proyecto ASPPlacemente, desde el Content Browser del editor, pulsar con el bot�n derecho del rat�n sobre la carpeta Blueprints y elegir Migrate. En el proceso de migraci�n especificar la carpeta Content del proyecto de destino.

11. Abrir el proyecto de destino y abrir el blueprint Roomwithobjects para comprobar que la compilaci�n es correcta.

3. Instrucciones de uso: preparaci�n de las mallas est�ticas.
-------------------------------------------------------------
1. Hay mallas est�ticas correctamente definidas en la carpeta Mesh de Content, con los sockets establecidos.
2. Las mallas est�ticas utilizadas como objetos deben tener definidos tres sockets de nombres Point1, PointF y Point2.
3. Point1 establece sobre el plano el punto cuyas coordenadas son establecidas por el algoritmo. En una visi�n "desde arriba", veremos el prisma demarcador del objeto como un rect�ngulo. Asumiendo un sistema de coordenadas x-y donde el (0,0) est� en la esquina superior izquierda de la habitaci�n, con el eje x de izquierda a derecha y el eje y de arriba abajo, Point1 debe marcar la esquina superior izquierda de este rect�ngulo cuando el objeto no est� rotado.
4. El socket PointF estar� en el mizmo plano (igual z que Point1)) de modo que el vector Point1->PointF marca la direcci�n de la arista inferior de la cara A del objeto. El resto de lados del objeto (B,C,D) se determinan en el sentido de las agujas del reloj. Importante: el vector Point 1 - Point F debe estar en el plano XY (Z=0) en coordenadas locales de la malla.
5. Point2, debe marcar el v�rtice opuesto  a Point1 en el prisma, determinando as� (junto con la direcci�n de la cara A), el volumen del prisma.

4. Instrucciones de uso: la estructura de los blueprints.
--------------------------------------------------------

1. El blueprint Floor como clase padre de RoomFromFloor y este �ltimo es el padre de Roomwithobjects.
2. En Floor hay un grafo de construcci�n que permite crear un suelo parametrizado a partir de NxM z�calos, con iluminaci�n y techo opcional.
3. En RoomFromFloor se a�aden las paredes. Actualmente se permiten tres tipos de paredes. Las mallas utilizadas son la pared b�sica, pared con puerta y pared con ventana.
4. RoomFromFloor permite la definici�n de las restricciones de altura. Una restricci�n establece para un lado de la habitaci�n una zona en la que no se puden situar objetos que sobrepasen cierta altura. La zona se define con una distancia de inicio de la zona, una distancia final y una altura. Las distancias se miden de izquierda a derecha, si observamos la pared desde dentro de la habitaci�n. 
5. RoomFromFloor incluye la definici�n de restricciones de dos formas. La primera parte son las restriccions obtenidas a partir de los tipos mencionados. Estas restricciones se pueden acumular autom�ticamente en el array de restricciones del lado de la habitac�on donde se cree la pared.
6. Cada pared admite tambi�n la definici�n manual de una restricci�n.
7. RoomwithObjets incluye la colocaci�n de objetos manual o autom�ticamente.
8. La lista de objetos es una lista de estructuras donde cada una representa las caracter�sticas del objeto que son independientes de su ubicaci�n y una lista de instancias de ese objeto donde para cada instancia se especifica su ubicaci�n.
9. Los par�metros de ubicaci�n manejados como "entrada" son: distancia desde el lado izquierdo de la pared, pared de la habitaci�n y rotaci�n del objeto.
10. Las distancias son valores enteros que al multiplicarse por el par�metro Unit determinan la distancia en unidades UE4.
11. Las paredes posibles son: WallA, WallB, WallC y WallD, y las rotaci�nes lo mismo.
12. La posici�n en cambio se da en unidades UE4, y se calcula autom�ticamente por los blueprints a partir los datos necesarios.
13. En la carpeta Blueprints dentro de la carpeta BlueprintFunctions en la librer�a PlacementFunctions se definen las funciones relativas  a los c�lculos de geometr�a.
14. CalculateReferenceCornerPosition obtiene la posici�n de la esquina de referencia del rect�ngulo sobre el que se situar� el objeto. Este c�lculo parte de las entradas mencionadas: distancia de la esquina izquierda, pared y rotaci�n. Se asume que primero se rota el objeto y luego se ubica en la posici�n. Resultado en unidades UE4.
15. GetStaticMeshComponentDimensions calcula el tama�o del objeto usando los sockets Point1 y Point2, adem�s devuelve un vector unitario en la direcci�n Point1 y PointF. El vector de dimensiones debe interpretarse as�: la primera componente es el tama�o de la cara A (y C) del objeto, la segunda componente es el tama�o de las caras B y D, y la tercera componente es la altura del objeto. Todo en unidades UE4.
16. CorrectSMComponent rota el objeto y luego lo situa teniendo en cuenta que el pivote de la malla (usado por UE4 como referencia para establecer la posici�n) no tiene que coincidir necesariamente con la esquina tomada como referencia.
17. CalculateFaceToWall. Simplemente calcula la cara que queda contra la pared.

5. Control del algoritmo de b�squeda de soluciones ASP.
-------------------------------------------------------

En la categor�a Placement Algorithm del blueprint Roomwithobjects, podemos encontrar algunos par�metros para controlar el algoritmo.

1. Automatic Generation: Si se marca, el algoritmo se ejecutar� al actualizar el blueprint. Se tratar� de ubicar la lista de objetos en la habitaci�n teniendo en cuenta las restricciones.
2. Solve Limit Conflicts: Establece un n�mero m�ximo de "conflictos" procesados por el algoritmo. Sirve para establecer un l�mite a la ejecuci�n de la b�squeda. Si se sobrepasa el l�mite de conflictos establecido aqu�, y el n�mero de objetos ubicados es menor que el n�mero de objetos, todos los objetos son marcados como "No ubicables" y la b�squeda termina. En ese caso no aparecer� ning�n objeto en la habitaci�n.
3. Random frequency: Se trata de un n�mero entr 0.0 y 1.0. El algoritmo de b�squeda utiliza una heur�stica por defecto para decidir la asignaci�n de verdad de las variables booleanas en el problema. Random frequency es la probabilidad con la que esta heur�stica no se aplica y la asignaci�n de verdad de la variable se realiza a partir de un experimento aleatorio.
4. Seed: Semilla en la generaci�n pseudoaleatoria.

Con los par�metros Random frequency y seed podemos generar soluciones aleatorias que cumplan con las restricciones impuestas.


TODO:

1. Corregir el no funcionamiento de las restricciones de altura. Aunque los objetos son situados correctamente dentro de la habitaci�n, sin solaparse y cumpliendo la restricci�n sobre la cara que debe situarse contra la pared.
2. Mejorar la geometr�a, para que toda la habitaci�n pueda rotarse sin problemas. Actualmente esto no est� contemplado.
