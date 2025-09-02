#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

uniform mat4[10] transformations;
uniform int transformation_count;
void main()
{
    mat4 transform = mat4(1.0);
    for(int i=0; i<transformation_count; i++)
    {
        transform = transform * transformations[i];
    }
    
    gl_Position = transform * vec4(aPos, 0.0, 1.0);
    
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
