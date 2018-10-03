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
	std::cout << "possible chices are ICO_Planet.obj, board.obj or, dragon.obj" << std::endl;
	std::cin >> filename;

	const aiScene *myScene = importer.ReadFile("../Models/"+filename, aiProcess_Triangulate );//| aiProcess_JoinIdenticalVertices);


    // If the import failed, report it
    if( !myScene)
    {
        printf("%s\n", importer.GetErrorString());
        for(unsigned int i = 0; i < Indices.size(); i++)
		{
			Indices[i] = Indices[i] - 1;
		}
    }
    else
    	Load(myScene);




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

void Object::Load(const aiScene *myScene){

	Vertices.clear();
	Indices.clear();
	aiMesh* mesh = myScene->mMeshes[0];
	int vertexNum = 0;
	int index = 0;
	// loop through each face
	for( index = 0; index < mesh->mNumFaces; index++ )
	{
		// create aiFace
		const aiFace& face = mesh->mFaces[index];

		// loop through each face in each vertex
		for( vertexNum = 0; vertexNum < face.mNumVertices; vertexNum++ )
		{
			aiVector3D tempPos = mesh -> mVertices[face.mIndices[vertexNum]];

			float r = rand() / (float) RAND_MAX;
			float g = rand() / (float) RAND_MAX;
			float b = rand() / (float) RAND_MAX;
			Vertex tempVert = {{tempPos.x, tempPos.y, tempPos.z}, {r, g, b}};

			// push vertex to Vertices
			Vertices.push_back(tempVert);
			Indices.push_back(face.mIndices[vertexNum]);
		}

	}





      // create aiMesh




	//Load("../Models/"+filename);





}

