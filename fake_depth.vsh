
attribute vec2 position;
attribute vec2 colorTexCoords;
uniform mediump mat4 projection;

varying mediump vec2 v_colorTexCoords;

void main()
{
	gl_Position = projection * vec4(position.xy, 0, 1);
	v_colorTexCoords = colorTexCoords;
}
