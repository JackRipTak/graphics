#include "svpng.inc"
#include <iostream>
#include "material.h"
#include "camera.h"
#include "hitable_list.h"
#include "sphere.h"
#include <float.h>
#include <omp.h>
#include <random>
#include "omp.h"

#define W 800
#define H 400

// normal shader
Vec3 color(const Ray &r, hitable *world) {
  hit_record rec;
  if (world->hit(r, 0.0, FLT_MAX, rec)) {
    //有撞点：即为球体，将球体颜色设置为球在该点的单位法向量的色彩表映射值
    return 0.5 *
           Vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
  } else {
    // 无撞点：将颜色设置为背景色
    Vec3 unit_direction = unit_vector(r.Direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) +
           t * Vec3(0.5, 0.7, 1.0); // white, light blue
  }
}

/******************** material shader ************************/
/**
 * @brief 光线追踪函数
 *
 * @param r: 光线
 * @param world： 场景中的物体集合
 * @param depth: 光线弹射次数，到达最大次数之后，返回0
 *
 */
using namespace std;
Vec3 ray_color(const Ray &r, hitable *world,int depth,int tag=1) {
	//cerr<<depth<<endl;
	if(depth<=0) return Vec3(0,0,0);
  //如果光线到达反射次数,停止
	hit_record tmp;
	if(world->hit(r,0.0,FLT_MAX,tmp)){
		Vec3 point=tmp.p+tmp.normal+random_sample_hemisphere(tmp.normal);
		//return Vec3(0,0,0);
		Ray scattered; Vec3 attenuation;
		if (tmp.mat->scatter(r, tmp, scattered, attenuation,tag))
            return attenuation * ray_color(scattered, world, depth-1,tag^1);
        return Vec3(0,0,0);
	}	
	Vec3 unit_direction = unit_vector(r.Direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) +
           t * Vec3(0.5, 0.7, 1.0); // white, light blue


  //如果光线与物体相交,设置反射后的颜色


  //如果光线与任何物体不相交,设置背景颜色

}

unsigned char img[W * H * 3];

int main() {
 // unsigned char *p = img;

  // 声明
  Camera cam;

  int SamplingRate = 100; //采样次数
  int Depth =5;
      //16; //光线弹射次数, 如果是一个空心玻璃球的话,弹射次数至少>4次，思考一下？

  //新建材质
  Material *diffuse_center = new Material(Vec3(0.1,0.2,0.5), Diffuse);
  Material *diffuse_ground = new Material(Vec3(0.8, 0.8, 0.0), Diffuse);
  Material *left =new Material(1.5,Dielectric);
  Material *right= new Material(Vec3(0.8,0.6,0.2),0.0,Metal);
  /*将所有能够被撞击的物体信息保存在列表中*/
  std::vector<hitable *> list;
  list.push_back(new sphere(Vec3(0, -100.5, -1), 100, diffuse_ground));
  list.push_back(new sphere(Vec3(0, 0, -1), 0.5, diffuse_center));
  list.push_back(new sphere(Vec3(-1.0,0.0,-1.0),0.5,left));
  //list.push_back(new sphere(Vec3(-1.0,0.0,-1.0),-0.4,left));
  list.push_back(new sphere(Vec3(1.0,0.0,-1.0),0.5,right));
  hitable *world = new hitable_list(list, list.size());

  omp_set_num_threads(12);
#pragma omp parallel for

  for (int j = H - 1; j >= 0; j--) {
	  cerr<<j<<endl;
    for (int i = 0; i < W; i++) {
	 unsigned char *p=img+W*(H-1-j)*3+i*3;
      Vec3 col(0, 0, 0);
      for (int s = 0; s < SamplingRate; s++) {
        float u = float(i + drand48()) / float(W);
        float v = float(j + drand48()) / float(H);

        Ray r = cam.getRay(u, v);
        //col += color(r, world);
         col += ray_color(r, world, Depth);
      }
      col /= float(SamplingRate);

      p[0] = int(255.99 * sqrt(col[0]));
      p[1] = int(255.99 * sqrt(col[1]));
      p[2] = int(255.99 * sqrt(col[2]));
    }
  }

  svpng(fopen("bg.png", "wb"), W, H, img, 0);
  std::cout << "finished" << std::endl;

  //释放资源
  delete diffuse_center, diffuse_ground;

  return 0;
}
