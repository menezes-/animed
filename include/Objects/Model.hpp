#pragma once

#include "../OpenGL/Shader.hpp"
#include "Mesh.hpp"
#include "../Loaders/TextureLoader.hpp"
#include "../Transform.hpp"
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

    Material loadMaterials(aiMaterial *amaterial);

    std::vector<Texture> loadMaterialTextures(aiMaterial *material, TextureType type);

};
