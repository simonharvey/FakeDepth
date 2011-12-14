
attribute vec2 position;
attribute vec2 texCoords;
attribute vec2 depthCoords;
attribute vec2 screenCoords;
uniform mediump mat4 projection;

varying mediump vec2 v_texCoords;
varying mediump vec2 v_depthCoords;
varying mediump vec2 v_screenCoords;

void main()
{
	gl_Position = projection * vec4(position.xy, 0, 1);
	v_texCoords = texCoords;
	v_depthCoords = depthCoords;
	v_screenCoords = screenCoords;
}
