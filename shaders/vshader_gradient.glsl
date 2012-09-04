attribute vec4 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoord;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec4 light_position;

varying float elevation;
varying vec2 texCoord;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL;

void
main()
{
	elevation = vPosition.y / 100.0;
	texCoord = vTexCoord;
	
	vec3 pos = (model_view * vPosition).xyz;
	
	fN = (model_view * vec4(vNormal,0.0)).xyz;
	fE = -pos;
	fL = light_position.xyz - pos;

	//if(light_position.w != 0.0) {
		//fL = light_position.xyz - vPosition.xyz;
	//}
	
    gl_Position = (projection * model_view * vPosition) / vPosition.w;
}
