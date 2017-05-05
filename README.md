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

12. Recordar que las mallas estáticas utilizadas como objetos deben tender definidos dos sockets de nombres Point1 y Point2, para establecer la orientación. Estos sockets deben estar en esquinas opuestas del prisma.
