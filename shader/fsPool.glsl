#version 330

in vec3 fragmentColor;
in vec2 tex_coord;

uniform sampler2D texBase;

out vec4 outputColor;

void main(){
    float alpha = 1.0;
    outputColor = (1 - alpha)*vec4( fragmentColor, 1.0 ) + alpha*texture(texBase, tex_coord);
}
