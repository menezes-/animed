#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureLoader.hpp"
#include "Transform.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/glm.hpp>

class Model {
public:
    Model(const std::string &filename, TextureLoader &textureLoader);

    void draw(Shader &shader) const;

private:
    const std::string filename;
    std::string basePath;
    std::vector<Mesh> meshes;
    TextureLoader &textureLoader;

    void load();

    void processNode(aiNode *node, const aiScene *scene);

    void makeMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterial(aiMaterial *material, TextureType type);

};
