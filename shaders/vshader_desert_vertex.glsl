attribute vec4 vPosition;
attribute vec3 vNormal;

uniform mat4 model_view;
uniform mat4 projection;
uniform float wireframe;

uniform vec4 ambient_product;
uniform vec4 diffuse_product;
uniform vec4 specular_product;
uniform vec4 light_position;
uniform float shininess;

float constantAttenuation = 0.0;
float linearAttenuation = 0.0;
float quadraticAttenuation = 1.75;

varying vec4 fragColor;

void
main()
{
    vec4 color;

	// First determine color based on elevation
	const vec4 waterColor = vec4(0.0,0.1,0.6,1.0);
    const vec4 grassColor = vec4(0.1,0.7,0.1,1.0);
    const vec4 rockColor = vec4(0.6,0.3,0.1,1.0);
    const vec4 snowColor = vec4(0.9,0.9,0.9,1.0);
	
	float elevation = vPosition.y;
	elevation = elevation / 100.0;
    if(elevation < 0.01) {
        color = waterColor;
    }else if(elevation < 0.03) {
        color = grassColor;
    }else if(elevation < 0.075){
        color = rockColor;
    }else {
        color = snowColor;
    }

	// If wireframe, just set color to black and skip lighting
	if(wireframe > 0.5) {
		color = vec4(0.0,0.0,0.0,1.0);
	}else {
		// Add stripes in 'rock' by making the terrain more/less red
		// in certain segments
		float red = 0.8;
		vec4 rockColor = vec4(0.8,0.45,0.2,1.0);
		if(mod(elevation, 0.03) < 0.01) {
			rockColor.r += 0.1;
		}
		if(mod(elevation, 0.05) < 0.001) {
			rockColor.r -= 0.1;
		}
		if(mod(elevation, 0.023) < 0.003) {
			rockColor.r += 0.05;
			rockColor.b -= 0.02;
		}

		// Calculate final color based on elevation
		float intensity = (elevation * 10.0 / 1.25) + 0.25;
		if(intensity > 1.0) {
			intensity = 1.0;
		}
		color = rockColor * vec4(intensity,intensity,intensity,1.0);
		
		// Add lighting
		vec3 pos = (model_view * vPosition).xyz;

		vec4 lp = light_position;
		vec3 L = normalize(lp.xyz - pos);
		vec3 E = normalize(-pos);
		vec3 H = normalize(L + E);

		vec3 N = normalize(model_view * vec4(vNormal,0.0)).xyz;

		float distance = length(L);
		float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance 
					+ quadraticAttenuation * distance * distance);

		vec4 ambient = ambient_product;

		float Kd = max(dot(L,N),0.0);
		vec4 diffuse = attenuation * Kd * diffuse_product;

		float Ks = pow(max(dot(N,H),0.0),shininess);
		vec4 specular = attenuation * Ks * specular_product;

		if(dot(L,N) < 0.0) {
			specular = vec4(0.0,0.0,0.0,1.0);
		}
		vec4 lighting = ambient + diffuse + specular;

		color = lighting * color;
		color.a = 1.0;
	}

	// Update final position and color
    fragColor = color;
    gl_Position = projection * model_view * vPosition / vPosition.w;
    //gl_Position = projection * model_view * vPosition;
}
