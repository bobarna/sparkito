#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { 
        objects.clear();
    }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
    }
    
    bool hit(const Ray& ray,
             Interval ray_t,
             HitRecord& rec) const override {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for(const auto& o : objects) {
            if(o->hit(ray, Interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                // TODO check if this is actually the closest object so far
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        
        return hit_anything;
    }

};


#endif //HITTABLE_LIST_H

