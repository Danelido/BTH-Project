#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_data[];

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPos;

out vec3 frag_normal;
out vec2 frag_uv;
out vec3 frag_objectPos;

void checkCulling()
{
    vec3 v1 = vs_data[1].position - vs_data[0].position;
    vec3 v2 = vs_data[2].position - vs_data[0].position;
    vec3 n = normalize(cross(v2, v1));
    
    vec3 toCamera = normalize(cameraPos - vs_data[0].position);
    
    float d = dot(toCamera, n);

    if(d > 0.0f){
      for(int i = 0; i < 3; i++){
        gl_Position = projectionMatrix * viewMatrix * vec4(vs_data[i].position,1.0f);
        frag_normal = vs_data[i].normal;
        frag_uv = vs_data[i].uv;
        frag_objectPos = vs_data[i].position;
        EmitVertex();
      }
       EndPrimitive();   
    }

}

void main()
{
   checkCulling();
}
