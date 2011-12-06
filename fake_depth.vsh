
attribute vec2 position;

uniform mediump mat4 projection;

void main()
{
	gl_Position = projection * vec4(position.xy, 0, 1);
}
