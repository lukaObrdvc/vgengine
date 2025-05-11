#ifndef CAMERA_H
#define CAMERA_H

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


inline Camera* get_main_camera()
{
    return &get_engine_state()->mainCamera;
}
inline r32 get_z_near()
{
    return get_main_camera()->zNear;
}
inline r32 get_z_far()
{
    return get_main_camera()->zFar;
}
inline r32 get_fov()
{
    return get_main_camera()->fov;
}


#endif
