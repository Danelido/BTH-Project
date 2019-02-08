#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    mat4 modelMatrix;

} vs_data[];


uniform vec3 cameraPos;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 frag_normal;
out vec2 frag_uv;
out vec3 frag_objectPos;

bool checkCulling(int index)
{
    vec4 worldVertexPos = vs_data[index].modelMatrix * vec4(vs_data[index].position, 1.0f);
    vec3 toCamera = normalize(cameraPos - worldVertexPos.xyz);
    
    float d = dot(toCamera, vs_data[index].normal);
    if(d > 0.0f)
    {
       gl_Position = projectionMatrix * viewMatrix * worldVertexPos;
       frag_normal = vs_data[index].normal;
       frag_uv = vs_data[index].uv;
       frag_objectPos = worldVertexPos.xyz;
       EmitVertex();

       return true;
    }

    return false;
}

void main()
{
    if(checkCulling(0) && checkCulling(1) && checkCulling(2))
    {
        EndPrimitive();
    }
}
