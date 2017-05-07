# ASPPlacement
Experimentos con ASP y Unreal Engine

Notas sobre migración de los blueprints a otro proyecto:

1. En el proyecto destino necesitamos que exista al menos un clase C++, para que el editor UE4 cree la estructura adecuada para proyectos con clases C++. Por ejemplo, agregar una clase vacía.

2. En el proyecto destino copiar en la carpeta Source los archivos ObjectProceduralFunctionLibrary.cpp y ObjectProceduralFuncionLibrary.h, que se encuentran en la carpeta correspondiente de este proyecto.

3. En el proyecto destino modificar en ObjectProceduralFuncionLibrary.cpp la cabecera #include "ASPPlacement.h": Cambiar a #include "Nombredelproyecto.h".

4. En el proyecto destino modificar en ObjectProceduralFunctionLibrary.h la primera línea de la definición de la clase UObjectProceduralFunctionLibrary. Debe pasar de:

class ASPPLACEMENT_API UObjectProceduralFunctionLibrary : public UBlueprintFunctionLibrary

a

class NOMBRENUEVOPROYECTO UObjectProceduralFunctionLibrary : public UBlueprintFunctionLibrary

5. Copiar en el proyecto destino la carpeta thirdpartylibraries del proyecto ASPPlacement.

6. Copiar en el proyecto destino el archivo clingo.dll a la carpeta Binaries.

7. En el proyecto destino, dentro de la subcarpeta del módulo en Source, modificar el archivo Nombredelproyecto.build.cs. Debe añadirse lo que incluye adicionalmente ASPPlacement.Build.cs.

8. Abrir la solución de Visual Studio del nuevo proyecto y compilar para comprobar que todo está OK. Una vez comprobado cerrar el proyecto.

9. Dentro de la carpeta Config del proyecto destino hay que modificar el archivo DefaultEngine.ini, añadiendo las siguientes líneas (sustituyendo Nomredelproyecto por el nombre correcto del proyecto de destino):

[/Script/Engine.Engine] 
+ActiveClassRedirects=(OldClassName="FEFaces",NewClassName="/Script/Nombredelproyecto.FEFaces")
+ActiveClassRedirects=(OldClassName="FEWalls",NewClassName="/Script/Nombredelproyecto.FEWalls")
+ActiveClassRedirects=(OldClassName="SObjectInstanceDescription",NewClassName="/Script/Nombredelproyecto.SObjectInstanceDescription")
+ActiveClassRedirects=(OldClassName="SObjectDescription",NewClassName="/Script/Nombredelproyecto.SObjectDescription")
+ActiveClassRedirects=(OldClassName="SWallConstraint",NewClassName="/Script/Nombredelproyecto.SWallConstraint")

10. En el proyecto ASPPlacemente, desde el Content Browser del editor, pulsar con el botón derecho del ratón sobre la carpeta Blueprints y elegir Migrate. En el proceso de migración especificar la carpeta Content del proyecto de destino.

11. Abrir el proyecto de destino y abrir el blueprint Roomwithobjects para comprobar que la compilación es correcta.

Notas sobre la preparación de las mallas estáticas.

1. Hay mallas estáticas correctamente definidas en la carpeta Mesh de Content, con los sockets establecidos.
2. Las mallas estáticas utilizadas como objetos deben tener definidos tres sockets de nombres Point1, PointF y Point2.
3. Point1 establece sobre el plano el punto cuyas coordenadas son establecidas por el algoritmo. En una visión "desde arriba", veremos el prisma demarcador del objeto como un rectángulo. Asumiendo un sistema de coordenadas x-y donde el (0,0) está en la esquina superior izquierda de la habitación, con el eje x de izquierda a derecha y el eje y de arriba abajo, Point1 debe marcar la esquina superior izquierda de este rectángulo cuando el objeto no está rotado.
4. El socket PointF estará en el mizmo plano (igual z que Point1)) de modo que el vector Point1->PointF marca la dirección de la arista inferior de la cara A del objeto. El resto de lados del objeto (B,C,D) se determinan en el sentido de las agujas del reloj.
5. Point2, debe marcar el vértice opuesto  a Point1 en el prisma, determinando así (junto con la dirección de la cara A), el volumen del prisma.

Notas sobre la estructura de los blueprint.

1. El blueprint Floor como clase padre de RoomFromFloor y este último es el padre de Roomwithobjects.
2. En Floor hay un grafo de construcción que permite crear un suelo parametrizado a partir de NxM zócalos, con iluminación y techo opcional.
3. En RoomFromFloor se añaden las paredes. Actualmente se permiten tres tipos de paredes. Las mallas utilizadas son la pared básica, pared con puerta y pared con ventana.
4. RoomFromFloor permite la definición de las restricciones de altura. Una restricción establece para un lado de la habitación una zona en la que no se puden situar objetos que sobrepasen cierta altura. La zona se define con una distancia de inicio de la zona, una distancia final y una altura. Las distancias se miden de izquierda a derecha, si observamos la pared desde dentro de la habitación. 
5. RoomFromFloor incluye la definición de restricciones de dos formas. La primera parte son las restriccions obtenidas a partir de los tipos mencionados. Estas restricciones se pueden acumular automáticamente en el array de restricciones del lado de la habitacíon donde se cree la pared.
6. Cada pared admite también la definición manual de una restricción.
7. RoomwithObjets incluye la colocación de objetos manual o automáticamente.
8. La lista de objetos es una lista de estructuras donde cada una representa las características del objeto que son independientes de su ubicación y una lista de instancias de ese objeto donde para cada instancia se especifica su ubicación.
9. Los parámetros de ubicación manejados como "entrada" son: distancia desde el lado izquierdo de la pared, pared de la habitación y rotación del objeto.
10. Las distancias son valores enteros que al multiplicarse por el parámetro Unit determinan la distancia en unidades UE4.
11. Las paredes posibles son: WallA, WallB, WallC y WallD, y las rotaciónes lo mismo.
12. La posición en cambio se da en unidades UE4, y se calcula automáticamente por los blueprints a partir los datos necesarios.
13. En la carpeta Blueprints dentro de la carpeta BlueprintFunctions en la librería PlacementFunctions se definen las funciones relativas  a los cálculos de geometría.
14. CalculateReferenceCornerPosition obtiene la posición de la esquina de referencia del rectángulo sobre el que se situará el objeto. Este cálculo parte de las entradas mencionadas: distancia de la esquina izquierda, pared y rotación. Se asume que primero se rota el objeto y luego se ubica en la posición. Resultado en unidades UE4.
15. GetStaticMeshComponentDimensions calcula el tamaño del objeto usando los sockets Point1 y Point2, además devuelve un vector unitario en la dirección Point1 y PointF. El vector de dimensiones debe interpretarse así: la primera componente es el tamaño de la cara A (y C) del objeto, la segunda componente es el tamaño de las caras B y D, y la tercera componente es la altura del objeto. Todo en unidades UE4.
16. CorrectSMComponent rota el objeto y luego lo situa teniendo en cuenta que el pivote de la malla (usado por UE4 como referencia para establecer la posición) no tiene que coincidir necesariamente con la esquina tomada como referencia.
17. CalculateFaceToWall. Simplemente calcula la cara que queda contra la pared.

15. 


TODO:

1. Corregir el no funcionamiento de las restricciones de altura. Aunque los objetos son situados correctamente dentro de la habitación, sin solaparse y cumpliendo la restricción sobre la cara que debe situarse contra la pared.
2. Mejorar la geometría, para que toda la habitación pueda rotarse sin problemas. Actualmente esto no está contemplado.
