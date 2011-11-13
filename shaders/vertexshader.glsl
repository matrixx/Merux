attribute highp vec3 vertex;
uniform float time;
uniform bool rippleOn;
uniform vec2 ripplePos;
uniform vec2 res;
varying vec3 vertexPos;

void main(void)
{
    vec3 pos;
    if (rippleOn) {
        float maxLen = 2.82;
        float len = length((vertex.xy - ripplePos) * vec2(1.0, res.y / res.x));
		float borderLen = time / 100.0 * (maxLen / 3.0);
		bool inArea = len < borderLen;
        if (inArea) {
            float timeAmpFactor = (1.01 - time / 100.0) / 33.0;
            float distanceAmpFactor = 1.0 - clamp(len / (maxLen / 2.0), 0.0, 1.0);
            float coord = len * 30.0;
            float lengthPeriodFactor = 2.5 - pow(len / (maxLen / 2.0), 2.0);
            float shiftFactor = (time / 100.0) * 2.0 * 3.14;
            float z = clamp(timeAmpFactor * distanceAmpFactor * sin(coord * lengthPeriodFactor - shiftFactor), -0.03, 0.03);
            pos = vec3(vertex.x, vertex.y, z);
        } else {
            pos = vertex;
        }
    } else {
        pos = vertex;
    }
    vertexPos = pos;
    gl_Position = vec4(pos, 1.0);
}
