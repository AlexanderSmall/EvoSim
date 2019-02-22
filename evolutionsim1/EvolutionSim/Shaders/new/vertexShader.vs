#version 330

uniform mat4 transformationMatrix;

in vec2 vertexPosition;
in vec4 vertexColor;

out vec4 fragmentColor;
out vec2 fragmentPosition;

void main(){
	fragmentColor = vertexColor;
    fragmentPosition = vertexPosition;
	
	gl_Position = transformationMatrix * vec4(vertexPosition, 0.0, 1.0);
}
