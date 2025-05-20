#ifndef CAMERA_H
#define CAMERA_H


#define MAIN_CAMERA ENGINE_STATE->main_camera
#define Z_NEAR MAIN_CAMERA.z_near
#define Z_FAR MAIN_CAMERA.z_far
#define FOV MAIN_CAMERA.fov

// @doc canvas plane is implicitly at zNear;
// canvas center is (0,0) relative to camera space origin;
// eye is positioned at camera space origin;
// camera is looking down negative Z in camera space;
// canvas aspect ration is implicitly 1, this also means FOVh = FOVv;
struct Camera
{
    Vector3 position;
    Quaternion orientation;
    r32 z_near;
    r32 z_far;
    r32 fov;
};


#endif
