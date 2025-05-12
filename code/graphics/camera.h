#ifndef CAMERA_H
#define CAMERA_H


#define MAIN_CAMERA ENGINESTATE->mainCamera
#define Z_NEAR MAIN_CAMERA.zNear
#define Z_FAR MAIN_CAMERA.zFar
#define FOV MAIN_CAMERA.fov

// @doc canvas plane is implicitly at zNear;
// canvas center is (0,0) relative to camera space origin;
// eye is positioned at camera space origin;
// camera is looking down negative Z in camera space;
// canvas aspect ration is implicitly 1, this also means FOVh = FOVv;
struct Camera
{
    Vec3f position;
    Quaternion orientation;
    r32 zNear;
    r32 zFar;
    r32 fov;
};


#endif
