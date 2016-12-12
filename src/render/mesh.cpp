#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::map<const std::string, VertexBuffer *> Mesh::vbufferMap;

Mesh::Mesh(const std::string &filename) {
	auto it = vbufferMap.find(filename);
	if(it != vbufferMap.end()) {
		buffer = it->second;
		buffer->addReference();
	} else {

	    Assimp::Importer importer;
	    const aiScene *scene = importer.ReadFile(("./res/models/" + filename).c_str(), aiProcess_Triangulate |
	                                                        aiProcess_JoinIdenticalVertices); //| 
	                                                       // aiProcess_FlipUVs);

	    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	        return;


	    aiMesh *mesh = scene->mMeshes[0];

	    std::vector<glm::vec3> pos;
	    std::vector<glm::vec2> uvs;
	    std::vector<glm::vec3> nor;
	    std::vector<Vertex> vertices;
	    std::vector<unsigned int> indices;

	    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
	        const aiVector3D p = mesh->mVertices[i];
	        const aiVector3D t = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0,0,0);
	        const aiVector3D n = mesh->mNormals[i];

	        vertices.push_back(Vertex(glm::vec3(p.x, p.y, p.z), glm::vec2(t.x, t.y), glm::vec3(n.x, n.y, n.z)));
	    }

	    for(unsigned int i = 0; i< mesh->mNumFaces; i++) {
	        const aiFace& face = mesh->mFaces[i];
	        indices.push_back(face.mIndices[0]);
	        indices.push_back(face.mIndices[1]);
	        indices.push_back(face.mIndices[2]);
	    }
	    buffer = new VertexBuffer(vertices, indices);

	    vbufferMap.insert(std::pair<std::string, VertexBuffer *>(filename, buffer));
	}
}

Mesh::Mesh(const std::string &name, VertexBuffer *buffer) {
	auto it = vbufferMap.find(name);
	if(it != vbufferMap.end()) {
		this->buffer = it->second;
		this->buffer->addReference();
	} else {
		this->buffer = buffer;
		vbufferMap.insert(std::pair<std::string, VertexBuffer *>(name, buffer));
	}
}

Mesh::~Mesh() {
    if(buffer && buffer->removeReference()) {
        delete buffer;
    }
}