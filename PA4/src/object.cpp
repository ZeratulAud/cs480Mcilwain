#include "object.h"
#include "engine.h"

Object::Object()
{  
	/*
	# Blender File for a Cube
	o Cube
	v 1.000000 -1.000000 -1.000000
	v 1.000000 -1.000000 1.000000
	v -1.000000 -1.000000 1.000000
	v -1.000000 -1.000000 -1.000000
	v 1.000000 1.000000 -0.999999
	v 0.999999 1.000000 1.000001
	v -1.000000 1.000000 1.000000
	v -1.000000 1.000000 -1.000000
	s off
	f 2 3 4
	f 8 7 6
	f 1 5 6
	f 2 6 7
	f 7 8 4
	f 1 4 8
	f 1 2 4
	f 5 8 6
	f 2 1 6
	f 3 2 7
	f 3 7 4
	f 5 1 8
	*/

	Vertices = {
	{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
	{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
	{{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
	{{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
	{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
	{{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
	};

	Indices = {
	2, 3, 4,
	8, 7, 6,
	1, 5, 6,
	2, 6, 7,
	7, 8, 4,
	1, 4, 8,
	1, 2, 4,
	5, 8, 6,
	2, 1, 6,
	3, 2, 7,
	3, 7, 4,
	5, 1, 8
	};

	orbitAngle = 0.0f;
	spinAngle = 0.0f;

	std::string filename;
	std::cout << "Enter in a filename from the Models folder" << std::endl;
	std::cout << "possible chices are ICO_Planet, board or, dragon" << std::endl;
	std::cin >> filename;
	Load("../Models/"+filename+".obj");
	// The index works at a 0th index
	for(unsigned int i = 0; i < Indices.size(); i++)
	{
		Indices[i] = Indices[i] - 1;
	}

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

}

Object::Object(float speed, float scale, float radius) : Object()
{
	orbitSpeed = 0.1 * speed;
	spinSpeed = 1 * speed;
	planetScale = scale;
	orbitRadius = radius;
	//Load("../Models/"+filename+".obj");

} 

Object::~Object()
{
	Vertices.clear();
	Indices.clear();
}

void Object::Update(unsigned int dt, glm::mat4 orbitOrigin)
{

	if (!stopOrbit)
		orbitAngle += dt * M_PI/1000 * orbitSpeed;
	if (!stopSpin)
		spinAngle += dt * M_PI/1000 * spinSpeed;
	

	model = glm::rotate(orbitOrigin, (orbitAngle), glm::vec3(0.0, 1.0, 0.0))*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, orbitRadius));
	model = glm::rotate(model, (-orbitAngle), glm::vec3(0.0, 1.0, 0.0));
	
	//TODO udate children
	for (auto &i : children) {
		i->Update(dt, model);
	}

	model = glm::rotate(model, (spinAngle), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(planetScale,planetScale,planetScale));

	
}


glm::mat4 Object::GetModel()
{
	return model;
}

void Object::Render(GLint& m_modelMatrix)
{
	for (auto &i : children) {
		i->Render(m_modelMatrix);
	}

	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(GetModel()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


}

void Object::FlipOrbitRotation(){
	orbitSpeed = -orbitSpeed;
}

void Object::FlipPlanetRotation(){
	spinSpeed = -spinSpeed;
}

void Object::ToggleOrbitRotation(){
	stopOrbit = !stopOrbit;
}

void Object::TogglePlanetRotation(){
	stopSpin = !stopSpin;
}

void Object::AddChild(float speed, float scale, float radius) {

	auto child = new Object(speed, scale, radius);

	children.push_back(child);
}

void Object::Load(std::string modelLoation){
	std::string line;
	
	std::ifstream inFile(modelLoation.c_str());
	if(!inFile.is_open()) 
	{
		std::cout << "file not open" << std::endl;
		return;
	}
	
	Vertices.clear();
	Indices.clear();

	while(true) {
		getline(inFile, line, ' ');
		if(inFile.eof()) break;

		while(std::isspace(line[0])) {
			line.erase(0, 1);
		}

		std::cout << "reading:" << line << std::endl;

		if(line == "#") {
			//Skip line for comment
			getline(inFile, line);

		} else if(line == "v") {
			std::cout << "Vertex found" << std::endl;
			float r = rand() / (float) RAND_MAX;
			float g = rand() / (float) RAND_MAX;
			float b = rand() / (float) RAND_MAX;
			Vertex tempVert = {{0.0, 0.0, 0.0}, {r, g, b}};
			inFile >> tempVert.vertex.x;
			inFile >> tempVert.vertex.y;
			inFile >> tempVert.vertex.z;

			Vertices.push_back(tempVert);

		} else if (line == "vn") {
			getline(inFile, line);
		} else if (line == "mtllib") {
			getline(inFile, line);
		} else if (line == "o") {
			getline(inFile, line);
		} else if (line == "usemtl") {
			getline(inFile, line);
		} else if (line == "s") {
			getline(inFile, line);
		} else if(line == "f") {
			//New face
			//f #/#/# #/#/# #/#/#
			unsigned int faceVertex[3];
			std::string nextArg;
			char* numEnding;
			unsigned vertexIndex;
		
			for (unsigned int &i : faceVertex) {
				//Just take the first number, we don't need to know texture mapping or normals
				inFile >> nextArg;
				i = strtol(nextArg.c_str(), &numEnding, 10);
				Indices.push_back(i);
				
				//strtol(numEnding + 1, &numEnding, 10); //just in case of the second number
				//strtol(numEnding + 1, &numEnding, 10) - 1;
				
			}

		}


	}	
}

