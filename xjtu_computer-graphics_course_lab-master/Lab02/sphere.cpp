#include "sphere.h"

inline bool sphere::hit(const Ray & r, float tmin, float tmax, hit_record & rec) const
{
    Vec3 oc = r.Origin() - center;
    float a = dot(r.Direction(), r.Direction());
    float b = dot(oc, r.Direction());
    float c = dot(oc, oc) - Radius * Radius;
    float discriminant  = b * b - a * c;
	Vec3 outward_normal = (rec.p - center) / Radius;
	rec.set_face_normal(r, outward_normal);
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b*b - a*c)) / (a) ;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = unit_vector((rec.p - center)/ Radius);
            rec.mat=mat;
            return true;
        }
        temp = (-b + sqrt(b*b - a*c)) / ( a);
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = unit_vector((rec.p - center)/ Radius);
            rec.mat=mat;
            return true;
        }
    }
    return false;
}
