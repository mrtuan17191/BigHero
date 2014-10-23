#ifdef GL_ES
precision mediump float;
#endif
 
// 1
varying vec2 v_texCoord;
uniform sampler2D u_texture;

 
void main()
{
    vec4 normalColor = texture2D(u_texture, v_texCoord);
    gl_FragColor = vec4(1.0,1.0,1.0,normalColor.a);
}