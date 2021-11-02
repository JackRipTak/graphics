#include "material.h"

float get_random_float(float min, float max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<float> dist(
      min, max); // distribution in range [min, max]
  return dist(rng);
}

Vec3 random_in_unit_sphere() {

}

Vec3 random_sample_hemisphere(const Vec3 &normal) {

}

Vec3 reflect(Vec3 &v, Vec3 &n) {  }

Vec3 refract(Vec3 &v, Vec3 &n, float ior) {

}

float schlick_reflectance(float cos, float ior) {
  // Use Schlick's approximation for reflectance

}

Material::Material(Vec3 albedo_, MaterialType type_) {
  albedo = albedo_;
  type = type_;
}

Material::Material(Vec3 albedo_, float fuzz_, MaterialType type_) {
  albedo = albedo_;
  type = type_;
  fuzz = std::min<float>(fuzz_, 1);
}

Material::Material(float ior_, MaterialType type_) {
  ior = ior_;
  type = type_;
}

Material::~Material() {}

bool Material::scatter(const Ray &r, hit_record &rec, Ray &scattered,
                       Vec3 &attenu) {
  switch (type) {
  case Diffuse: {

  }

  case Metal: {

  }

  case Dielectric: {

    return true;
    break;
  }

  default: {
    std::cout << "Unsurportted Materail type!" << std::endl;
    return false;
    break;
  }
  }
}