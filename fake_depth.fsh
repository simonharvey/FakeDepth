varying mediump vec2 v_colorTexCoords;
uniform sampler2D colorTex;

void main()
{
	gl_FragColor = texture2D(colorTex, v_colorTexCoords);//vec4(1.0, 0, 0, 1.0);
}