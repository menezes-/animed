#include "../../include/Objects/Model.hpp"
#include <assimp/Importer.hpp>
#include <iostream>
#include <fmt/format.h>

Model::Model(const std::string &filename, TextureLoader &textureLoader) : filename(filename),
                                                                          textureLoader(textureLoader) {
    auto found = filename.find_last_of('/');
    basePath = filename.substr(0, found);
    if (basePath.back() != '/') {
        basePath = basePath.append(std::string{'/'});
    }

    load();
}


void Model::draw(Shader &shader) const {
    for (const auto &mesh: meshes) {
        mesh.draw(shader);
    }
}

void Model::load() {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename,
                                             aiProcess_Triangulate | aiProcess_OptimizeGraph |
                                             aiProcess_OptimizeMeshes | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                                             aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error{
                fmt::format("ERROR ao carregar o modelo {0} \n: {1}", filename, importer.GetErrorString())};
    }
    
    std::vector<Material> materials;

    for (int i = 0; i < scene->mNumMaterials; ++i) {
        Material material = loadMaterials(scene->mMaterials[i]);
        materials.push_back(material);
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (std::size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        makeMesh(mesh, scene);
    }

    // processa os descendentes recursivamente
    for (std::size_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

void Model::makeMesh(aiMesh *mesh, const aiScene *scene) {

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            // um vertex pode conter até 8 coordenadas  de textura diferentes.
            // a implementação para o uso das outras 7 fica a cargo do leitor.
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;

            // Tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            // Bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;

        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    //vertex indices.
    for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        //extrai todos os indices da face e insere no vetor de indices
        std::copy(face.mIndices, face.mIndices + face.mNumIndices, std::back_inserter(indices));
    }
    //materiais

    // ler comentário em Mash.hpp para convenções
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    Material meshMaterial = loadMaterials(material);

    meshes.emplace_back(vertices, indices, meshMaterial);
}


Material Model::loadMaterials(aiMaterial *amaterial) {
    //valores default
    glm::vec3 ka{0.05f, 0.2f, 0.05f};
    glm::vec3 kd{0.3f, 0.5f, 0.3f};
    glm::vec3 ks{1.0f, 1.0f, 1.0f};
    GLfloat shininess = 32.0f;

    aiColor4D color{0.0f, 0.0f, 0.0f, 0.0f};
    if (AI_SUCCESS == aiGetMaterialColor(amaterial, AI_MATKEY_COLOR_AMBIENT, &color)) {
        ka = glm::vec3{color.r, color.g, color.b};
    }


    if (AI_SUCCESS == aiGetMaterialColor(amaterial, AI_MATKEY_COLOR_DIFFUSE, &color)) {
        kd = glm::vec3{color.r, color.g, color.b};
    }

    if (AI_SUCCESS == aiGetMaterialColor(amaterial, AI_MATKEY_COLOR_SPECULAR, &color)) {
        ks = glm::vec3{color.r, color.g, color.b};
    }

    aiString name;
    amaterial->Get(AI_MATKEY_NAME, name);

    amaterial->Get(AI_MATKEY_SHININESS, shininess);

    if (shininess <= 0.1) {
        shininess = 32.0f;
    }

    Material material;
    material.ambient = ka;
    material.diffuse = kd;
    material.specular = ks;
    material.shininess = shininess;

    for (std::size_t i = 0; i < TextureType::MAX_TEXTURE_TYPE; ++i) {
        TextureType ti = static_cast<TextureType>(i);
        auto tmp = loadMaterialTextures(amaterial, ti);
        std::copy(tmp.begin(), tmp.end(), std::back_inserter(material.textures));
    }

    return material;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, TextureType type) {
    aiTextureType aiType;
    switch (type) {

        case DIFFUSE:
            aiType = aiTextureType_DIFFUSE;
            break;
        case SPECULAR:
            aiType = aiTextureType_SPECULAR;
            break;
        case NORMAL:
            aiType = aiTextureType_NORMALS;
            break;
        case HEIGHT:
            aiType = aiTextureType_HEIGHT;
            break;
        case AMBIENT:
            aiType = aiTextureType_AMBIENT;
            break;
        default:
            throw std::runtime_error{"Tipo de material não registrado"};
    }

    std::vector<Texture> textures;

    for (std::size_t i = 0; i < material->GetTextureCount(aiType); ++i) {
        aiString path;
        unsigned int ui = static_cast<unsigned int>(i);
        material->GetTexture(aiType, ui, &path);
        GLuint textureId = textureLoader.loadFromPath(basePath + std::string{path.C_Str()});
        // se eu tentar carregar uma textura usada para normal mapping e essa textura não existir
        // não carrega ela.
        if ((type == HEIGHT || type == NORMAL) && textureId == textureLoader.getDefaultTexture()) {
            continue;
        }
        Texture texture;
        texture.id = textureId;
        texture.type = type;
        textures.push_back(texture);
    }

    return textures;
}
