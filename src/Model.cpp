#include "../include/Model.hpp"
#include "../include/utils.hpp"
#include <assimp/Importer.hpp>

Model::Model(const std::string &filename, TextureLoader &textureLoader, Shader &shader) : filename(filename),
                                                                                          shader(shader), textureLoader(textureLoader) {
    auto found = filename.find_last_of('/');
    basePath = filename.substr(0, found);
    if (basePath.back() != '/') {
        basePath = basePath.append(std::string{'/'});
    }

    load();
}

void Model::draw() {
    for (const auto &mesh: meshes) {
        mesh.draw(shader);
    }
}

void Model::load() {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate  | aiProcess_GenSmoothNormals |
                                                       aiProcess_FlipUVs );

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error{"ERRO ao carregar o modelo " + filename + ":\n" + importer.GetErrorString()};
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
    std::vector<Texture> textures;

    //Vertices
    for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.position = glm::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        vertex.normal = glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

        if (mesh->mTextureCoords[0]) {
            // um vertex pode conter até 8 coordenadas  de textura diferentes.
            // a implementação para o uso das outras 7 fica a cargo do leitor.
            vertex.texCoords = glm::vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        } else {
            vertex.texCoords = glm::vec2{0.0f, 0.0f};
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

    for (std::size_t i = 0; i < TextureType::MAX_TEXTURE_TYPE; ++i) {
        TextureType ti = static_cast<TextureType>(i);
        auto tmp = loadMaterial(material, ti);
        std::copy(tmp.begin(), tmp.end(), std::back_inserter(textures));
    }

    meshes.emplace_back(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterial(aiMaterial *material, TextureType type) {
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
        Texture texture;
        texture.id = textureId;
        texture.type = type;
        textures.push_back(texture);
    }

    return textures;
}






