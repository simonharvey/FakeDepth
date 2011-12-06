
attribute vec2 position;
attribute vec2 texCoords;
uniform mediump mat4 projection;

varying mediump vec2 v_texCoords;

void main()
{
	gl_Position = projection * vec4(position.xy, 0, 1);
	v_texCoords = texCoords;
}
