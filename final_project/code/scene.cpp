#include <iostream>
#include <vector>
#include <cmath>
#include "scene.h"
#include "calc.h"

scene::scene() {
    Init();
}

scene::~scene() {
}

void scene::Forward(float x, float z) {
    this->eye[0] += x;
    this->eye[2] += z;
    this->vat[0] += x;
    this->vat[2] += z;
}

void scene::Backward(float x, float z) {
    this->eye[0] -= x;
    this->eye[2] -= z;
    this->vat[0] -= x;
    this->vat[2] -= z;
}

void scene::Left(float x, float z) {
    this->eye[0] -= x;
    this->eye[2] -= z;
    this->vat[0] -= x;
    this->vat[2] -= z;
}

void scene::Right(float x, float z) {
    this->eye[0] += x;
    this->eye[2] += z;
    this->vat[0] += x;
    this->vat[2] += z;
}

void scene::TurnLeft(float  fraction, float x, float z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1]);
    this->eye[0] += fraction * x;
    this->eye[2] += fraction * z;
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1]);
    this->eye[0] = new_dis[0]*r/new_r + this->vat[0];
    this->eye[2] = new_dis[1]*r/new_r + this->vat[2];
}

void scene::TurnRight(float  fraction, float x, float z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1]);
    this->eye[0] -= fraction * x;
    this->eye[2] -= fraction * z;
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1]);
    this->eye[0] = new_dis[0]*r/new_r + this->vat[0];
    this->eye[2] = new_dis[1]*r/new_r + this->vat[2];
}

void scene::TurnUp(float fraction, float cross_x, float cross_y, float cross_z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1] + dis[2]*dis[2]);
    this->eye[0] += fraction * vup[0];
    this->eye[1] += fraction * vup[1];
    this->eye[2] += fraction * vup[2];
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1] + new_dis[2]*new_dis[2]);
    float ratio = r/new_r;

    this->eye[0] *= ratio;
    this->eye[1] *= ratio;
    this->eye[2] *= ratio;

    // Fixed vup
    dis[0] = this->eye[0] - this->vat[0];
    dis[1] = this->eye[1] - this->vat[1];
    dis[2] = this->eye[2] - this->vat[2];

    float new_vup_x = dis[1]*cross_z - dis[2]*cross_y;
    float new_vup_y = dis[2]*cross_x - dis[0]*cross_z;
    float new_vup_z = dis[0]*cross_y - dis[1]*cross_x;
    float new_vup_dis = sqrt(new_vup_x*new_vup_x + new_vup_y*new_vup_y + new_vup_z*new_vup_z);
    this->vup[0] = new_vup_x/new_vup_dis;
    this->vup[1] = new_vup_y/new_vup_dis;
    this->vup[2] = new_vup_z/new_vup_dis;
}

void scene::TurnDown(float fraction, float cross_x, float cross_y, float cross_z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1] + dis[2]*dis[2]);
    this->eye[0] -= fraction * vup[0];
    this->eye[1] -= fraction * vup[1];
    this->eye[2] -= fraction * vup[2];
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1] + new_dis[2]*new_dis[2]);
    float ratio = r/new_r;

    this->eye[0] *= ratio;
    this->eye[1] *= ratio;
    this->eye[2] *= ratio;

    // Fixed vup
    dis[0] = this->eye[0] - this->vat[0];
    dis[1] = this->eye[1] - this->vat[1];
    dis[2] = this->eye[2] - this->vat[2];

    float new_vup_x = dis[1]*cross_z - dis[2]*cross_y;
    float new_vup_y = dis[2]*cross_x - dis[0]*cross_z;
    float new_vup_z = dis[0]*cross_y - dis[1]*cross_x;
    float new_vup_dis = sqrt(new_vup_x*new_vup_x + new_vup_y*new_vup_y + new_vup_z*new_vup_z);
    this->vup[0] = new_vup_x/new_vup_dis;
    this->vup[1] = new_vup_y/new_vup_dis;
    this->vup[2] = new_vup_z/new_vup_dis;
}

void scene::Init() {
    eye[0] = 0; eye[1] = 100; eye[2] = 700;
    for (int i = 0; i < 3; ++i) vat[i] = 0.0f;
    vup[0] = 0; vup[1] = 1; vup[2] = 0;
    fovy = 0;
    dnear = 0.1;
    dfar = 5000;
    for (int i = 0; i < 2; ++i) viewport[i] = 0.0f;
    for (int i = 2; i < 4; ++i) viewport[i] = 800;
}

void BALL::init() {
    r = 1.0, g = 1.0, b = 1.0;
    x = 0.0, y = 0.0, z = 0.75;
    v[0] = v[1] = 0, v[2] = 1;
    radius = 0.0625;
    tmp_v_written=false;
}
void BALL::detectBalls(int i,std::vector<BALL>&data)
{
    for(int it=0;it<data.size();++it)
    {
        if(it==i) continue;
        double dx=data[it].x-x,dy=data[it].y-y,dz=data[it].z-z;
        double rpr=data[it].radius+radius;
        if(dx*dx+dy*dy+dz*dz < rpr*rpr) // touched
        {
            double v_[3]={data[it].v[0]-v[0],data[it].v[1]-v[1],data[it].v[2]-v[2]};
            if( dot(dx,dy,dz,v_[0],v_[1],v_[2])<0 ) // collapse
            {
                double distance2=dx*dx+dy*dy+dz*dz;
                double Di_dis2=dot(dx,dy,dz,tmp_v[0],tmp_v[1],tmp_v[2])/distance2;
                double Dit_dis2=dot(dx,dy,dz,data[it].v[0],data[it].v[1],data[it].v[2])/distance2;
                tmp_v[0]-=(Di_dis2-Dit_dis2)*dx;
                tmp_v[1]-=(Di_dis2-Dit_dis2)*dy;
                tmp_v[2]-=(Di_dis2-Dit_dis2)*dz;
            }
        }
    }
}
void BALL::wirte_tmp_v()
{
    tmp_v_written=true;
    for(int i=0;i<3;++i)
        tmp_v[i]=v[i];
}
void BALL::update_speed()
{
    if(!tmp_v_written) return;
    tmp_v_written=false;
    for(int i=0;i<3;++i)
        v[i]=tmp_v[i];
}
/* BALL -------------------------------- PLANE */
void PLANE::init() {
    loc[0][0]=-10.0;
    loc[0][1]=-10.0;
    loc[0][2]=0.0;
    loc[1][0]=10.0;
    loc[1][1]=-10.0;
    loc[1][2]=0.0;
    loc[2][0]=10.0;
    loc[2][1]=10.0;
    loc[2][2]=0.0;
    loc[3][0]=-10.0;
    loc[3][1]=10.0;
    loc[3][2]=0.0;
    u_norm[0]=0.0;
    u_norm[1]=0.0;
    u_norm[2]=1.0;
    area=400;
    r=0.0;
    g=1.0;
    b=0.0;
    alpha=0.5;
    x=0.0;
    y=0.0;
    z=0.0;
}
void PLANE::collapse(BALL &rhs)
{
    double loc_d=dot(x-rhs.x,   y-rhs.y,   z-rhs.z,    /* ball_center -> plane_center */
                     u_norm[0], u_norm[1], u_norm[2]); // |plane's u_norm|==1
    double pjlen=loc_d<0? -loc_d : loc_d;
    if(pjlen < rhs.radius)
    {
        double vd=dot(rhs.v[0],  rhs.v[1],  rhs.v[2],    /* ball_v */
                      u_norm[0], u_norm[1], u_norm[2]); // |plane's u_norm|==1
        for(int i=0;i<3;++i)
            rhs.v[i]-=2*vd*u_norm[i];
    }
}
/* PLANE -------------------------------- CYLINDER */
void CYLINDER::init()
{
    bx=by=bz=0,btx=bty=0,btz=3,
    radius=1,height=3;
    r=g=1,b=0,alpha=0.5;
}
void CYLINDER::collapse(BALL&rhs)
{
    double dx=rhs.x-bx,dy=rhs.y-by,dz=rhs.z-bz;
    double DOT=dot(dx,dy,dz,btx,bty,btz);
    double pjlen=DOT/height;
    double Dhh=pjlen/height;
    double pjv[3]={btx*Dhh,bty*Dhh,btz*Dhh};
    double dv[3]={dx-pjv[0],dy-pjv[1],dz-pjv[2]};
    double dvdv=dv[0]*dv[0]+dv[1]*dv[1]+dv[2]*dv[2];
    if( (pjlen<height)&&(dvdv<radius*radius) ) // indise
    {
        double vDhh=dot(rhs.v[0],rhs.v[1],rhs.v[2],btx,bty,btz)/height/height;
        double vpjv[3]={btx*vDhh,bty*vDhh,btz*vDhh};
        if( (pjlen+rhs.radius>=height) )
        {
            if(vDhh>0)
                for(int i=0;i<3;++i)
                    rhs.v[i]-=2*vpjv[i];
        }
        double rsr=radius-rhs.radius;
        if( (dvdv>=rsr*rsr) )
        {
            for(int i=0;i<3;++i)
                vpjv[i]=rhs.v[i]-vpjv[i];
            for(int i=0;i<3;++i)
                rhs.v[i]-=2*vpjv[i];
        }
        /*/
        if(dvdv<rsr*rsr)
        {
            for(int i=0;i<3;++i)
                rhs.v[i]-=2*dv[i];
        }// */
    }

    /*/
    if( (pjlen<height)&&(pjlen+rhs.radius>=height) )
    {
        for(int i=0;i<3;++i)
            rhs.v[i]-=2*pjv[i];
    }
    if( (dv[0]*dv[0]+dv[1]*dv[1]+dv[2]*dv[2] < rsr*rsr) )
    {
        for(int i=0;i<3;++i)
            rhs.v[i]-=(2*dv[i]);
    }// */
}
