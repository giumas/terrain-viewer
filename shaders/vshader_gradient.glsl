attribute vec4 vPosition;
attribute vec3 vNormal;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec4 light_position;
uniform float max_elevation;

varying float color_intensity;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL;

void
main()
{
    color_intensity = vPosition.y / max_elevation;

    vec3 pos = (model_view * vPosition).xyz;

    fN = (model_view * vec4(vNormal,0.0)).xyz;
    fE = -pos;
    fL = light_position.xyz - pos;

    gl_Position = (projection * model_view * vPosition) / vPosition.w;
}
