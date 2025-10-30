#version 450 core
in layout(location = 0) vec2 position;
in layout(location = 1) vec2 v_uv;
out vec2 f_uv;

void main() {
	f_uv = v_uv;
	gl_Position = vec4(position, 0.0, 1.0);
}