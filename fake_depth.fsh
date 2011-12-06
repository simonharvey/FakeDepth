varying mediump vec2 v_texCoords;

uniform sampler2D depthTex;
//uniform sampler2D depthTex;

void main()
{
	gl_FragColor = texture2D(depthTex, v_texCoords);//vec4(1.0, 0, 0, 1.0);
	// opengles 2.0 does not support gl_FragDepth... dammit
	//gl_FragDepth = texture2D(depthTex, v_colorTexCoords).r;
}