#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec2 TexCoord;
out vec3 Color;
uniform mat4[10] model;
uniform int model_count;
uniform mat4 view;
void main()
{
    mat4 model_transform = mat4(1.0);
    for(int i=0; i<model_count; i++)
    {
        model_transform = model_transform * model[i];
    }
    
    gl_Position = model_transform * vec4(aPos, 1.0);
    Color = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
