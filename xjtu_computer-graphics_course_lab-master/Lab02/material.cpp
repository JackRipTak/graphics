#include "material.h"

using namespace std;

float get_random_float(float min, float max) {
static  std::random_device dev;
static  std::mt19937 rng(dev());
static  std::uniform_real_distribution<float> dist(
      min, max); // distribution in range [min, max]
  return dist(rng);
}

Vec3 random_in_unit_sphere() {
	while(1){
		auto p=Vec3(get_random_float(-1,1),get_random_float(-1,1),get_random_float(-1,1));
		if(p.squared_length()<1) return p;
	}		
}

Vec3 random_sample_hemisphere(const Vec3 &normal) {
	auto p=random_in_unit_sphere();
	if(dot(p,normal)>0.0) return p;
	else return -p;
}

Vec3 reflect(Vec3 &v, Vec3 &n) {  
	return v-2*dot(v,n)*n;
}


Vec3 refract(Vec3 &v, Vec3 &n, float ior){
	Vec3 iv=unit_vector(v);
	double cos_theta=dot(iv,-n);
	double sin_theta2=1-cos_theta*cos_theta;
	double st2=ior*ior*sin_theta2;
	double cost=sqrt(1-st2);
	return (iv-n*cos_theta)*ior-n*cost;
}


float schlick_reflectance(float cos, float ior) {
  // Use Schlick's approximation for reflectance
	float r = (1-ior) / (1+ior);
    r =r*r;
    return r + (1-r)*pow((1 - cos),5);
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
                       Vec3 &attenu,int &tag) {
  switch (type) {
  case Diffuse: {
	Vec3 tmp=rec.normal+random_in_unit_sphere();
	scattered=Ray(rec.p,tmp);
	attenu=albedo;
	return 1;
  }

  case Metal: {
	Vec3 dir=r.Direction(); dir.make_unit_vector();
	Vec3 tmp=reflect(dir,rec.normal);
	scattered=Ray(rec.p,tmp+fuzz*random_in_unit_sphere());
	attenu=albedo;
	return dot(scattered.Direction(),rec.normal)>0;
  }

  case Dielectric: {
   attenu=Vec3(1.0,1.0,1.0);
   double ttt=rec.front_face?ior:1.0/ior;
   Vec3 dir=unit_vector(r.Direction());
   double cos_theta = fmin(dot(-dir,rec.normal), 1.0);
   double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

   bool cannot_refract = ttt * sin_theta > 1.0;
   Vec3 direction;
   if (cannot_refract||schlick_reflectance(cos_theta,ttt) > get_random_float(0,1))
         direction = reflect(dir, rec.normal);
   else{
         direction = refract(dir, rec.normal, ttt),tag^=1;
   }
   scattered = Ray(rec.p, direction);
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
