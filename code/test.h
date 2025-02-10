#define CURRENTLY_TESTING none_test()

global_variable b32 filetest = true;
global_variable b32 mathtest = true;
global_variable b32 emacstest = true;


void none_test()
{}

void file_test()
{
    if (filetest)
        {
            u32 readsize;            
            dbg_read_file((u8*)"W:\\cardcrafter\\data\\readtest.txt", (u8*)memory.base+memory.size, &readsize);
            memory.size += readsize;
            dbg_write_file((u8*)"W:\\cardcrafter\\data\\writetest.txt", (u8*)memory.base+memory.size, readsize);
            filetest = false;
        }
}
/*
void math_test()
{
#define PRINT(N)                                \
    char vstr##N[256];                          \
    sprintf(vstr##N, "%f \n", N);               \
    OutputDebugString(vstr##N);
    
#define PRINT2(v)                               \
    char vstr##v[256];                          \
    sprintf(vstr##v, "(%f , %f)\n", v.x, v.y);  \
    OutputDebugString(vstr##v);

#define PRINT3(v)                                           \
    char vstr##v[256];                                      \
    sprintf(vstr##v, "(%f , %f , %f)\n", v.x, v.y, v.z);    \
    OutputDebugString(vstr##v);

#define INIT2(name, X, Y)                       \
    v2 name = {};                               \
    name.x = (r32)X;                            \
    name.y = (r32)Y;

#define INIT3(name, X, Y, Z)                    \
    v3 name = {};                               \
    name.x = (r32)X;                            \
    name.y = (r32)Y;                            \
    name.z = (r32)Z;
    
    if (mathtest)
        {

            INIT2(V1, 12.5, 4.26);
            INIT2(V2, 4.26, 5.55);

            PRINT2(V1);
            PRINT2(V2);

            INIT3(V3, 52.2, -215, -2.523);
            INIT3(V4, -74.2, 0, 636);

            PRINT3(V3);
            PRINT3(V4);

            origin2 bas2 = {};
            origin3 base3 = {};

            r32 len1 = elen(bas2, V1);
            PRINT(len1);
            r32 len2 = edist(V2, V1);
            r32 len3 = edist(V1, V2);
            PRINT(len2);
            PRINT(len3);
            r32 len4 = elen(base3, V3);
            PRINT(len4);
            r32 len5 = edist(V3, V4);
            r32 len6 = edist(V4, V3);
            PRINT(len5);
            PRINT(len6);

            // @Note when lerping from high you step from high toward low,
            // when lerping from low you step from low toward high,
            // same step distance if same argument passed
            r32 lerp1 = lerp(40, 60, (r32)0.4);
            r32 lerp2 = lerp(60, 40, (r32)0.4);
            r32 lerp3 = lerp(40, 60, (r32)0.2);
            r32 lerp4 = lerp(40, 60, (r32)0.8);
            r32 lerp5 = lerp(40, 60, (r32)0);
            r32 lerp6 = lerp(40, 60, (r32)1);
            PRINT(lerp1);
            PRINT(lerp2);
            PRINT(lerp3);
            PRINT(lerp4);
            PRINT(lerp5);
            PRINT(lerp6);

            INIT2(op1, ((V1+V2).x), ((V1+V2).y));
            INIT2(op2, ((V1-V2).x), ((V1-V2).y));
            INIT2(op3, ((V2-V1).x), ((V2-V1).y));
            INIT2(op4, ((V1*V2).x), ((V1*V2).y));

            INIT3(op5, ((V4+V3).x), ((V4+V3).y), ((V4+V3).z));
            INIT3(op6, ((V4-V3).x), ((V4-V3).y), ((V4-V3).z));
            INIT3(op7, ((V3-V3).x), ((V3-V3).y), ((V3-V3).z));
            INIT3(op8, ((V4*V3).x), ((V4*V3).y), ((V4*V3).z));
            
            PRINT2(op1);
            PRINT2(op2);
            PRINT2(op3);
            PRINT2(op4);
            PRINT3(op5);
            PRINT3(op6);
            PRINT3(op7);
            PRINT3(op8);

            v2 e1;
            e1 = V1;
            PRINT2(e1);
            v2 e2 = {};
            e2 += V2 += V1;
            PRINT2(e2);

            v2 e3;
            e3 = icomponent(e1);
            PRINT2(e3);
            v2 e4;
            e4 = jcomponent(e2);
            PRINT2(e4);

            v2 e5;
            e5 = perp(e3);
            v2 e6;
            e6 = perp(e4);
            PRINT2(e5);
            PRINT2(e6);

            // @Fail numerical inaccuracy when using sin/cos to rotate
            // due to PI not being exact
            v2 r1;
            r1 = rotate_d(e3, 90);
            v2 r2;
            r2 = rotate(e4, -PI/2);
            PRINT2(r1);
            PRINT2(r2);

            // @TODO test transpose for 2d ..?
            
            mathtest = false;
            
        }

#undef PRINT
#undef PRINT2
#undef PRINT3
#undef INIT2
#undef INIT3
    
}
*/
void test()
{
    CURRENTLY_TESTING;
}
