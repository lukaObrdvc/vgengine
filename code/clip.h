#ifndef CLIP_H
#define CLIP_H

typedef v4 Vertex4;

bool InsideRight(v4 v) {
    return v.x <= v.w;
}
bool InsideLeft(v4 v) {
    return v.x >= -v.w;
}
bool InsideTop(v4 v) {
    return v.y <= v.w;
}
bool InsideBottom(v4 v) {
    return v.y >= -v.w;
}
bool InsideNear(v4 v) {
    return v.z >= -v.w;
}
bool InsideFar(v4 v) {
    return v.z <= v.w;
}

inline v4 lerp(v4 a, v4 b, float t)
{
    v4 result;
    result.x = a.x + t * (b.x - a.x);
    result.y = a.y + t * (b.y - a.y);
    result.z = a.z + t * (b.z - a.z);
    result.w = a.w + t * (b.w - a.w);
    return result;
}

v4 IntersectRight(v4 a, v4 b) {
    float t = (a.w - a.x) / ((a.w - a.x) - (b.w - b.x));
    return lerp(a, b, t);
}
v4 IntersectLeft(v4 a, v4 b) {
    float t = (a.w + a.x) / ((a.w + a.x) - (b.w + b.x));
    return lerp(a, b, t);
}
v4 IntersectTop(v4 a, v4 b) {
    float t = (a.w - a.y) / ((a.w - a.y) - (b.w - b.y));
    return lerp(a, b, t);
}
v4 IntersectBottom(v4 a, v4 b) {
    float t = (a.w + a.y) / ((a.w + a.y) - (b.w + b.y));
    return lerp(a, b, t);
}
v4 IntersectNear(v4 a, v4 b) {
    float t = (a.z) / (a.z - b.z);
    return lerp(a, b, t);
}
v4 IntersectFar(v4 a, v4 b) {
    float t = (a.w - a.z) / ((a.w - a.z) - (b.w - b.z));
    return lerp(a, b, t);
}


int ClipAgainstPlane(Vertex4* in_vertices, int in_count, Vertex4* out_vertices,
                     bool (*Inside)(Vertex4), Vertex4 (*Intersect)(Vertex4, Vertex4))
{
    int out_count = 0;
    Vertex4 prev = in_vertices[in_count-1];

    for (int i = 0; i < in_count; i++) {
        Vertex4 curr = in_vertices[i];

        bool prev_inside = Inside(prev);
        bool curr_inside = Inside(curr);

        if (curr_inside) {
            if (!prev_inside) {
                out_vertices[out_count++] = Intersect(prev, curr);
            }
            out_vertices[out_count++] = curr;
        }
        else if (prev_inside) {
            out_vertices[out_count++] = Intersect(prev, curr);
        }

        prev = curr;
    }

    return out_count;
}

int ClipTriangle(v4 A, v4 B, v4 C, TriangleHom* out_triangles)
{
    Vertex4 input[8];
    Vertex4 temp[8];
    Vertex4 temp2[8];

    input[0] = A;
    input[1] = B;
    input[2] = C;
    int count = 3;

    // Clip against each plane in order
    count = ClipAgainstPlane(input, count, temp, InsideRight, IntersectRight);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp, count, temp2, InsideLeft, IntersectLeft);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp2, count, temp, InsideTop, IntersectTop);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp, count, temp2, InsideBottom, IntersectBottom);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp2, count, temp, InsideNear, IntersectNear);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp, count, temp2, InsideFar, IntersectFar);
    if (count == 0) return 0;

    // Now temp2 holds final polygon
    // Triangulate fan (0,1,2), (0,2,3), (0,3,4), etc
    for (int i = 1; i < count-1; i++) {
        out_triangles[i-1].A = temp2[0];
        out_triangles[i-1].B = temp2[i];
        out_triangles[i-1].C = temp2[i+1];
    }

    return count-2; // Number of triangles output
}


#endif
