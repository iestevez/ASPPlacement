# ASPPlacement
Experimentos con ASP y Unreal Engine

Notas sobre migraci�n de los blueprints a otro proyecto:

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

12. Recordar que las mallas est�ticas utilizadas como objetos deben tender definidos dos sockets de nombres Point1 y Point2, para establecer la orientaci�n. Estos sockets deben estar en esquinas opuestas del prisma.
