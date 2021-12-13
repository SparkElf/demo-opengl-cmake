#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
using namespace glm;
using namespace std;
class Camera{
    public:
    vec3 targetPos=vec3(),up=vec3(),pos=vec3();
    mat4 view;
    float speed=0.5f;
    Camera(){cout<<"?"<<endl;}
    Camera(vec3 pos,vec3 targetPos,vec3 up):pos(pos),targetPos(targetPos),up(up){//target pos 0.0 0.0 0.0异常？？？原因不明
        cout<<to_string(view)<<endl;
        cout<<"in"<<endl;
        cout<<to_string(pos)<<endl;
        cout<<to_string(targetPos)<<endl;
        cout<<to_string(up)<<endl;
        cout<<to_string(pos+targetPos)<<endl;
        view=lookAt(pos,pos+targetPos,up);
        cout<<to_string(view)<<endl;
    }
    void updateView(){
        //这样设计可以在移动时平移视线 实现头、脚分离
         view=lookAt(pos,pos+targetPos,up);
    }
    mat4& getView(){return view;}
};