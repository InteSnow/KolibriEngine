#include "Model.h"
#include <string>

#include GL_HEADER

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "core/logger.h"
Model Model::create(const char* path, ShadingType shading) {
  Model model = {};
  model.shading = shading;

  std::vector<Vertex> vertices;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  tinyobj::LoadObj(&attrib, &shapes, &materials, NULL, NULL, path, "resources/models");

  for (uint32 i = 0; i < shapes.size(); i++) {
    vertices.clear();
    for (const auto& index : shapes[i].mesh.indices) {
      Vertex v = {};

      v.pos = {
        attrib.vertices[3*index.vertex_index+0],
        attrib.vertices[3*index.vertex_index+1],
        -attrib.vertices[3*index.vertex_index+2],
      };

      v.texCoords = {
        attrib.texcoords[2*index.texcoord_index+0],
        1.0f - attrib.texcoords[2*index.texcoord_index+1]
      };

      v.normal = {
        attrib.normals[3*index.normal_index+0],
        attrib.normals[3*index.normal_index+1],
        -attrib.normals[3*index.normal_index+2],
      };

      if (shapes[i].mesh.material_ids.size() >= attrib.vertices.size()/3) {
        v.color = {
          materials[shapes[i].mesh.material_ids[index.vertex_index/3]].diffuse[0],
          materials[shapes[i].mesh.material_ids[index.vertex_index/3]].diffuse[1],
          materials[shapes[i].mesh.material_ids[index.vertex_index/3]].diffuse[2],
        };
      } else {
        v.color = vec3(1.0f);
      }
      vertices.push_back(v);
    }
    std::string diff = std::string("resources/textures/") + materials[shapes[i].mesh.material_ids[0]].diffuse_texname;
    model.meshes.push_back(Mesh::create(std::move(vertices), shading, diff));
  }
  return model;
}

void Model::destroy(Model& model) {
  for (Mesh& mesh : model.meshes) {
    Mesh::destroy(mesh);
  }
  model.meshes.clear();
}

void Model::onDraw() {
  for (Mesh& mesh : this->meshes) {
    mesh.onDraw();
  }
}

void Model::onUnregister() {
  Model::destroy(*this);
}

BoundingBox Model::getBoundingBox() const {
  BoundingBox b = {};
  BoundingBox mb = {};
  for (uint32 i = 0; i < this->meshes.size(); i++) {
    mb = this->meshes[i].getBoundingBox();
    if (!i) {
      b = mb;
      continue;
    }
    b.min.x = b.min.x > mb.min.x ? mb.min.x : b.min.x;
    b.min.y = b.min.y > mb.min.y ? mb.min.y : b.min.y;
    b.min.z = b.min.z > mb.min.z ? mb.min.z : b.min.z;
    b.max.x = b.max.x < mb.max.x ? mb.max.x : b.max.x;
    b.max.y = b.max.y < mb.max.y ? mb.max.y : b.max.y;
    b.max.z = b.max.z < mb.max.z ? mb.max.z : b.max.z;
  }
  return b;
}
