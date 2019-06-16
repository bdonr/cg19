#include "TorusFactory.h"
#include <cfloat>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace scg {

TorusFactory::TorusFactory() {}

// Stockwerke, Scheiben
GeometryCoreSP TorusFactory::createTorus(GLfloat torusRadius, GLfloat innerRadius, int nStacks, int nSlices) {

	/*Hier wird das VBO genutzt, Elements sagt: rendere die mit den Indices definierten Dreiecke*/
	auto core = GeometryCore::create(GL_TRIANGLES, DrawMode::ELEMENTS);

	/*Definiert wie viele Vertices gesetzt werden*/
	int nVertices = (nStacks+1) * (nSlices+1);

	/*3 x nVertices für die x, y, z Koordinaten im Raum*/
	GLfloat* vertices = new GLfloat[3 * nVertices];
	GLfloat* normals = new GLfloat[3 * nVertices]; //Ausrichtung des Vertices, q(u,v)-p(u)
	GLfloat* tangents = new GLfloat[3 * nVertices]; //Bildet einen orthogonalen Vector in der WZ-Ebene
	GLfloat* binormals = new GLfloat[3 * nVertices]; //Bildet einen rothogonalen Vector auf dem Normalen und Tangenten Vector
	GLfloat* texCoords = new GLfloat[2 * nVertices];



	/*Index zum Hochzaehlen für die Arrays oben*/
	int vertexIndex = 0;

	/*Laufe die Schichten des Torus ab*/
	for (int i = 0; i <= nStacks; i++) { // 0 1 2 ... n
		/*Aktueller Winkel in Radiant*/
		float u = 2.f * PI * ((float) i / (float) nStacks); //0-2PI

		/*Berechnung des Punktes P(u)*/
		glm::vec3 p(((float) torusRadius * (cos(u)) * (float) 1),
				 	((float) torusRadius * (sin(u)) * (float) 1),
					(0 * (float) 1));

// 	    std::cout<<"P: "<<glm::to_string(p).c_str()<<std::endl;

		/*Berechnung der W-Achse: P(u)-Nullpunkt=P(u), alternativ geometrische Loesung: (cos(u), sin(u), 0), wird verwendet*/
//	  std::cout<<"\n Aktuelle Gradzahl="<<u<<std::endl;
//	  std::cout<<"Aktueller Kreismittelpunkt= punkt("<<p[0]<<"|"<<p[1]<<"|"<<p[2]<<"\"KM\")"<<std::endl;
//	  std::cout<<"Aktuelle w-Achse= punkt("<<wAchse[0]<<"|"<<wAchse[1]<<"|"<<wAchse[2]<<"\"w-Achse\")"<<std::endl;
//	  std::cout<<"w-Achse 1= : "<<glm::to_string(w).c_str()<<std::endl;

		/*Laufe die Schichten der einzelnen Kreise ab. Mittelpunkt= P(u), Ebene= ZW*/
		for (int j = 0; j <= nSlices; j++) {
			/*Aktueller Winkel in Radiant*/
			float v = 2.f * (float) PI * ((float) j / (float) nSlices);

			/* Algorithmus Berechnung der Vertices, abhaengig von Winkel u und v in Radiant
			 *
			 * Q(u,v)=(Mittelpunkt Torus = C) + (Radius Torus= d) *
			 * (W-Achse = W(u) = (x=cos(u), y= sin(u), z=0) +
			 * (Radius einzelner Kreis = d) * cos(v) *
			 * (W-Achse = W(u) = (x=cos(u), y= sin(u), z=0) +
			 * (x=0, y=0, z=((Radius einzelner Kreis = d)*sin(v))))
			 * */

			vertices[vertexIndex]     = ((float) torusRadius * cos(u))   + ((float) innerRadius * cos(v) * cos(u)    + (float) 0);  // x
			vertices[vertexIndex + 1] = ((float) torusRadius * sin(u))   + ((float) innerRadius * cos(v) * sin(u)    + (float) 0);  // y
			vertices[vertexIndex + 2] = ((float) torusRadius * (float) 0)+ ((float) innerRadius * cos(v) * (float) 0 + (float) innerRadius * sin(v));   // z


//		    std::cout<<"\nStack= "<<i<<", Slice="<<j<<", vertexIndex="<<vertexIndex/3<<", punkt("<<vertices[vertexIndex]<<"|"<<vertices[vertexIndex+1]<<"|"<<vertices[vertexIndex+2]<<"\"v="<<vertexIndex/3<<"\")"<<std::endl;
//			std::cout << ", punkt(" << vertices[vertexIndex] << "|"	<< vertices[vertexIndex + 1] << "|"	<< vertices[vertexIndex + 2] << "\"vertex="	<< vertexIndex / 3 << "\")" << std::endl;

			/*Berechnung der Normalen(Orientierung Vertex): Q(u,v)-P(u)*/
			glm::vec3 normale = glm::normalize(
					glm::vec3(((float) (vertices[vertexIndex] - p[0])),
							  ((float) (vertices[vertexIndex + 1] - p[1])),
							  ((float) (vertices[vertexIndex + 2] - p[2]))));

			normals[vertexIndex] 	 = normale[0];		// x
			normals[vertexIndex + 1] = normale[1];   	// y
			normals[vertexIndex + 2] = normale[2];    	// z

//		    std::cout<<"Normale= "<<"punkt("<<normals[vertexIndex]<<"|"<<normals[vertexIndex + 1]<<"|"<<normals[vertexIndex + 2]<<")"<<std::endl;

			/*Berechnung der Tangente(Orthogonal auf der Normalen, in WZ-Ebene): t(-sin(v), cos(v), o) fuer den Einheitskreis*/
			glm::vec4 tangente((float) (-sin(v)), (float) (cos(v)), (float) (0), (float) (0));

//		    std::cout<<"-sin(v)= "<<-sin(v)<<"cos(v)="<<cos(v)<<std::endl;
//		    glm::vec4 TESTtangente(-1.0f, 0.0f, 0.0f, 0.0f);  //Zum testen

			/*Transformation der Tangente ins Koordinatensystem des aktuellen Kreises*/
			glm::mat4 rotateX = glm::mat3(1.0f);
			rotateX = glm::rotate(rotateX, (2.f * (float) PI / (float) 4),	glm::vec3(1, 0, 0)); //rotiert fest um 90 Grad

			glm::mat4 rotateZ = glm::mat3(1.0f);
			rotateZ = glm::rotate(rotateZ,(2.f * (float) PI / (float) nStacks * (float) i),	glm::vec3(0, 0, 1));  //rotiert  Z-Achse um u

			tangente = rotateX * tangente;
			tangente = rotateZ * tangente;
			tangente = glm::normalize(tangente);

//		    std::cout<<"Tangente Ergebnis (V5 ) "<<glm::to_string(tangente)<<std::endl;

			tangents[vertexIndex] 		= tangente[0]; // x
			tangents[vertexIndex + 1] 	= tangente[1]; // y
			tangents[vertexIndex + 2]	= tangente[2]; // z

			/*Berechnung der Binormalen(Orthogonal auf Normalen und Tangente, in WZY-Ebene): Entspricht dem Kreuzprodukt von Tangente und Normale*/
			glm::vec3 binormale = glm::normalize(glm::cross(glm::vec3(tangente), glm::vec3(normale)));

			binormals[vertexIndex]      = binormale[0];	// x
			binormals[vertexIndex + 1]  = binormale[1];	// y
			binormals[vertexIndex + 2]  = binormale[2];	// z

//		    std::cout<<"Binormale (V5 ) "<<glm::to_string(binormale)<<std::endl;




		    int texIdx = 2 * (i * (nSlices + 1) + j);

			/*Slice-Teil*/
		    texCoords[texIdx] = j * (1.f / nSlices);       // s
			/*Stack-Teil*/
		    texCoords[texIdx + 1] = i * (1.f / nStacks);   // t
//			std::cout<<"texCoords["<<texIdx<<"] ="<<texCoords[texIdx]<<",   texCoords["<<texIdx+1<<"] ="<<texCoords[texIdx+1]<<std::endl;

			vertexIndex += 3;
		}

		std::cout<<std::endl;
	}


	/*Vertex-Array: Definieren den Koerper*/
	core->addAttributeData(OGLConstants::VERTEX.location, vertices,	3 * nVertices * sizeof(GLfloat), 3, GL_STATIC_DRAW);
	/*Normalen-Array: Definieren die Ausrichtung der Vertices (Reflexionsberechnung/Lichtberechnung/ Helligkeit)*/
	core->addAttributeData(OGLConstants::NORMAL.location, normals,3 * nVertices * sizeof(GLfloat), 3, GL_STATIC_DRAW);
	/*Tangenten-Array und Binormalen-Array bilden mit den Normalen ein Koordinatensystem pro Vertex*/
	core->addAttributeData(OGLConstants::TANGENT.location, tangents,3 * nVertices * sizeof(GLfloat), 3, GL_STATIC_DRAW);
	/*Binormalen-Array*/
	core->addAttributeData(OGLConstants::BINORMAL.location, binormals,3 * nVertices * sizeof(GLfloat), 3, GL_STATIC_DRAW);
	/*Definiert Texture-Elements, Texels: Fuellt eine Bitmap, auf die Texturen "geklebt" werden*/
    core->addAttributeData(OGLConstants::TEX_COORD_0.location, texCoords, 2 * nVertices * sizeof(GLfloat), 2, GL_STATIC_DRAW);

	/* Definiton der Triangles der Faces:
	 *(u, v) -> (u, v+1) -> (u-1, v)
	 *(u-1, v) -> (u, v+1) -> (u-1, v+1)
	 * */




	/*Faces: nSlices * nStacks, pro Faces werden zwei Triangles definiert */
	int nTriangles = 2 * nSlices * nStacks;

	/*3 * nTriangles für die x, y, z Werte */
	GLuint* indices = new GLuint[3 * nTriangles];

	/*Index laeuft von 0 bis nTriangles-1*/
	int indiciesIndex = 0;

	int /*(u,v)=*/p1, /*(u,v+1)=*/p2, /*(u-1,v)=*/p3, /*(u-1,v+1)=*/p4;
	for (int u = 1; u < nStacks; u++) {
//    	std::cout<<"Diese Ebene= "<<u<<std::endl;
		for (int v = 0; v < nSlices; v++) {
			p1 = v + (u * (nSlices+1));
			p2 = p1 + 1;
			p3 = p1 - (nSlices+1);
			p4 = p3 + 1;

//			if ((p1 % nSlices) == (nSlices - 1)) {
//				p2 = p1 - (nSlices - 1);
//				p3 = p1 - nSlices;
//				p4 = p3 - (nSlices - 1);
//			}

			indices[indiciesIndex] = p1;
//    		std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p1<<std::endl;
			indiciesIndex++;
			indices[indiciesIndex] = p2;
//    		std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p2<<std::endl;
			indiciesIndex++;
			indices[indiciesIndex] = p3;
//    		std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p3<<std::endl;
			indiciesIndex++;
//    		std::cout<<"("<<p1<<"->"<<p2<<"->"<<p3<<")"<<std::endl;

			indices[indiciesIndex] = p3;
//    		std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p3<<std::endl;
			indiciesIndex++;
			indices[indiciesIndex] = p2;
//    		std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p2<<std::endl;
			indiciesIndex++;
			indices[indiciesIndex] = p4;
//    		std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p4<<std::endl;
			indiciesIndex++;
//    		std::cout<<"("<<p3<<"->"<<p2<<"->"<<p4<<")"<<std::endl;
//    		std::cout<<std::endl;

//			std::cout << "genutzte Indices: " << p1 << ", " << p2 << ", " << p3	<< ", " << p4 << std::endl;
		}
	}
//    std::cout<<"nTriangles= "<<nTriangles<<std::endl;
	/*Diese Schleife laeuft den ersten Kreis ab und verbindet diesen mit dem letzten Kreis*/
	for (int i = 0; i < nSlices; i++) {
		p1 = i;
		p2 = p1 + 1;
		p3 = ((nStacks+1) * (nSlices+1)) - ((nSlices+1)*2)+ i;
		p4 = p3 + 1;

//		if ((p1 % nSlices) == (nSlices - 1)) {
//			p2 = p1 - (nSlices - 1);
//			p4 = p3 - (nSlices - 1);
//		}

		indices[indiciesIndex] = p1;
//    	std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p1<<std::endl;
		indiciesIndex++;
		indices[indiciesIndex] = p2;
//    	std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p2<<std::endl;
		indiciesIndex++;
		indices[indiciesIndex] = p3;
//    	std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p3<<std::endl;
		indiciesIndex++;
//    	std::cout<<"("<<p1<<"->"<<p2<<"->"<<p3<<")"<<std::endl;

		indices[indiciesIndex] = p3;
//    	std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p3<<std::endl;
		indiciesIndex++;
		indices[indiciesIndex] = p2;
//    	std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p2<<std::endl;
		indiciesIndex++;
		indices[indiciesIndex] = p4;
//    	std::cout<<"indiciesIndex= "<<indiciesIndex<<"   ="<<p4<<std::endl;
		indiciesIndex++;
//    	std::cout<<std::endl;

//		std::cout << "genutzte Indices: " << p1 << ", " << p2 << ", " << p3	<< ", " << p4 << std::endl;

	}

	core->setElementIndexData(indices, 3 * nTriangles * sizeof(GLuint), GL_STATIC_DRAW);
//  std::cout<<"Anzahl Vertices: "<<std::endl;

//	std::cout << "vertexIndex=" << vertexIndex / 3 << ", nVertices="<< nVertices << "indiciesIndex=" << indiciesIndex / 3<< " nTriangles=" << nTriangles << std::endl;

	/*Speicher freigeben, da diese Informationen schon im Core gespeichert wurden*/
	delete [] vertices;
	vertices = nullptr;
	delete [] normals;
	normals = nullptr;
	delete [] tangents;
	tangents = nullptr;
	delete [] binormals;
	binormals = nullptr;
	delete [] indices;
	indices = nullptr;

	return core;
}

TorusFactory::~TorusFactory() {}

}
