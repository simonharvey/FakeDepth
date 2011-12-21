#ifdef GL_ES
precision highp float;
#endif

varying mediump vec2 v_texCoords;

uniform sampler2D tex;

void main()
{
	vec4 c = texture2D(tex, v_texCoords);
	gl_FragColor = c;
}