#version 330 core
out vec4 FragColor;

#define MAX_STEPS 100
#define MAX_DIST 100.
#define CONTACT 0.01

uniform float time;
uniform float x;
uniform float y;
uniform float mouseX;
uniform float mouseY;
uniform float forw_move;
uniform float side_move;
uniform vec3 ro;

mat4 RotateX(float a)
{
    mat4 rotated = mat4(1, 0,       0,      0,
                        0, cos(a), -sin(a), 0,
                        0, sin(a),  cos(a), 0,
                        0, 0,       0,      1);

    return rotated;
}
mat4 RotateY(float a)
{
    mat4 rotated = mat4(cos(a), 0,  sin(a), 0,
                        0,      1,  0,      0,
                       -sin(a), 0,  cos(a), 0,
                        0,      0,  0,      1);

    return rotated;
}
mat4 RotateZ(float a)
{
    mat4 rotated = mat4(cos(a), -sin(a),  0,      0,
                        sin(a),  cos(a),  0,      0,
                        0,       0,       0,      0,
                        0,       0,       0,      1);

    return rotated;
}

mat2 SimpleRotate(float a)
{
    mat2 rotation = mat2(cos(a), -sin(a),
                         sin(a),  cos(a));

    return rotation;
}

float Cube(vec3 p, float b)
{
    vec3 q = abs(p) - vec3(b);
    
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z) ),0.0);
}

vec4 Sphere(vec3 p, vec3 position, float radius, vec3 color)
{
    return vec4(color, length(p-position) - radius);
}

float Box(vec3 p, vec3 b)
{
    return length(max(abs(p)-b,0.0));
}

vec4 GetDist(vec3 p)
{
    vec3 spherePos = vec3(0., 1, 6.);
    vec3 cube = vec3(0., 1, 6.);
    vec4 dP = vec4(vec3(.4, .4, .1),p.y);
    // p = mod(p +2., 10.)-2.;
    vec4 dS = Sphere(p, spherePos, 1., vec3(.3, .6, .7));
    float dB = Box(p - cube, vec3(.75));
    float dPS = max(dS, dB);
    return min(dPS, dP);
    
}

float RayMarching(vec3 ro, vec3 rd)
{
    float d0 = 0.;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + d0 * rd;
        float dS = GetDist(p);
        d0 += dS;
        if(d0 > MAX_DIST || d0 < CONTACT) break;
    }
    
    return d0;
}
vec3 GetNormal(vec3 p)
{
    float d = GetDist(p);
    vec2 e = vec2(0.01,0);
    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));
    return normalize(n);
}

float GetLight(vec3 p)
{
    vec3 lightPos = vec3(0, 6, 3);
    lightPos.xz += vec2(sin(time), cos(time)) *2.;
    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);
    float diffuse = clamp(dot(n, l), 0., 1.) ;
 	return diffuse;
} 
void main()
{
    // vec2 mouse = vec2(mouseX, mouseY);
    vec2 resolution = vec2(x, y);
    vec2 uv = (gl_FragCoord.xy-.5*resolution.xy)/resolution.y;
    // mouse = mouse/resolution;
    vec2 movement = vec2(forw_move*0.05, side_move*0.05);
    vec3 ro = ro;
    
    
    vec3 rd = normalize(vec3(uv.x, uv.y, 1.));

    
    vec3 col = vec3(0);
    
    float rm = RayMarching(ro, rd);
    vec3 point = (ro + rd * rm)* vec3(1., 0.01, 0.);
    float diffuse = GetLight(point);
    
    //rm /= 7.;
    col = vec3(diffuse) *clamp(sin(time), 0.5, 1.0)*1.5;

    
    FragColor = vec4(col,1.0);
}