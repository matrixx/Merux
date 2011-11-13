#ifdef GL_ES
precision highp float;
#endif

uniform vec2 res;
uniform sampler2D tex;

varying vec3 vertexPos;

void main(void) {
    vec2 texcoord = gl_FragCoord.xy / res.xy;
    vec3 color = texture2D(tex, texcoord).xyz + vec3(vertexPos.z, vertexPos.z, vertexPos.z);

    gl_FragColor = vec4(color, 1.0);
}
