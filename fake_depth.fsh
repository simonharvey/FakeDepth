#ifdef GL_ES
precision highp float;
#endif

varying mediump vec2 v_texCoords;
varying mediump vec2 v_depthCoords;
varying mediump vec2 v_screenCoords;

uniform sampler2D depthTex;
uniform sampler2D colorTex;
uniform sampler2D screenDepthTex;

void main()
{
	lowp vec4 depth_v = texture2D(depthTex, v_texCoords);
	//if (depth_v.r > .5)
	//	discard;
	gl_FragColor = texture2D(screenDepthTex, v_texCoords);
}