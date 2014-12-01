#include <iostream>
#include <cmath>
#include "scene.h"

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
}
