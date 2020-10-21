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
uniform int effectActivator;
uniform mat4 viewRot;
uniform vec3 cunt;

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


// OP union
vec4 min4v(vec4 d1, vec4 d2)
{
    return (d1.w < d2.w) ? d1 : d2;
}
vec3 min4vc(vec4 d1, vec4 d2)
{
    return (d1.w < d2.w) ? d1.xyz : d2.xyz;
}
vec3 max4vc(vec4 d1, vec4 d2)
{
    return (d1.w > d2.w) ? d1.xyz : d2.xyz;
}
vec4 max4v(vec4 d1, vec4 d2)
{
    return (d1.w > d2.w) ? d1 : d2;
}

vec4 smoothMin4v(vec4 a, vec4 b, float k)
{
    float h = clamp(.5 + .5 * (b.w - a.w), 0., 1.);
    return vec4(min4vc(a, b), mix(b.w, a.w, h) - k*h*(1.0-h));
}

vec4 Cube(vec3 p, float b, vec3 color)
{
    vec3 q = abs(p) - vec3(b);
    
    return vec4(color, length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z) ),0.0));
}

vec4 Sphere(vec3 p, vec3 position, float radius, vec3 color)
{
    return vec4(color, length(p-position) - radius);
}

vec4 Box(vec3 p, vec3 b, vec3 color)
{
    return vec4(color, length(max(abs(p)-b,0.0)));
}

float offset = 6.0;

vec4 GetDist(vec3 p)
{
    vec3 spherePos = vec3(.5, 3, 6.);
    // spherePos.xz *= SimpleRotate(sin(time*2));
    // spherePos += vec3(3, 0, 0);
    vec3 cube = vec3(0., 1, 6.);
    // cube.xz *= SimpleRotate(cos(time*3));
    vec3 cube2 = vec3(0., 5., 6.);
    vec4 dP = vec4(vec3(1, 1, 1),p.y);
    if(effectActivator == 1)
    { p = mod(p +offset, 30.)-offset;}
    vec4 dS = Sphere(p, spherePos, 1.2, vec3(.9, .4, .8/*.9, .6, .1*/));
    vec4 dB = Box(p - cube, vec3(1.), vec3(.4, .4, .8));
    vec4 dB2 = Cube(p- cube2, 1, vec3(.1, .9, .8));
    vec4 dPS = smoothMin4v(dB, dS, 1.);
    vec4 dAO = smoothMin4v(dPS, dB2, .85);
    return min4v(dAO, dP);
    
}

float RayMarching(vec3 ro, vec3 rd, inout vec3 col)
{
    float d0 = 0.;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + d0 * rd;
        vec4 dS = GetDist(p);
        d0 += dS.w;
        if(d0 > MAX_DIST || d0 < CONTACT) {
            col = dS.xyz;
            break;
            }
    }
    
    return d0;
}
vec3 GetNormal(vec3 p)
{
    vec4 d = GetDist(p);
    vec2 e = vec2(0.01,0);
    vec3 n = d.w - vec3(
        GetDist(p-e.xyy).w,
        GetDist(p-e.yxy).w,
        GetDist(p-e.yyx).w);
    return normalize(n);
}

vec3 GetLight(vec3 p, vec3 color)
{
    vec3 lightPos1 = vec3(0, 6, 3);
    vec3 secLightPos = vec3(3., 8., 3.);
    vec3 lightPos = min(lightPos1, secLightPos);
    lightPos.xz += vec2(sin(time), cos(time)) *2.;
    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);
    float diffuse = clamp(dot(n, l), 0., 1.) ;
    vec3 diffcol = diffuse * color;
 	return diffcol;
} 
vec3 R(vec2 uv, vec3 p, vec3 l, float z) {
    vec3 f = normalize(l-p),
        r = normalize(cross(p, f)),
        u = cross(f,r),
        c = p+f*z,
        i = c + uv.x*r + uv.y*u,
        d = normalize(i-p);
    return d;
}
void main()
{
    // vec2 mouse = vec2(mouseX, mouseY);
    vec2 resolution = vec2(x, y);
    vec2 uv = (gl_FragCoord.xy-.5*resolution.xy)/resolution.y;
    // mouse = mouse/resolution;
    vec3 oCol;
    // vec2 movement = vec2(forw_move*0.05, side_move*0.05);
    vec3 ro = ro;
    // ro.xz *= vec2(RotateY(10.)*vec4(ro, 1.0));
    
    
    vec3 rd = R(uv, ro, cunt/*vec3(viewRot)*/, 1.);
    

    
    vec3 col = vec3(0);
    
    float rm = RayMarching(ro, rd, oCol);
    vec3 point = (ro + rd * rm)* (effectActivator == 1 ? vec3(1., 0.01, 0.) : vec3(1)); // fun stuff happens with the lighting by multiplying it by this vec3*/
    vec3 diffuse = GetLight(point, oCol);
    
    
    col = diffuse;

    
    FragColor = vec4(col,1.0);
}