uniform sampler2D texture;

varying float elevation;
varying vec2 texCoord;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL;

uniform float wireframe;

uniform vec4 ambient_product;
uniform vec4 diffuse_product;
uniform vec4 specular_product;
uniform float shininess;

float constantAttenuation = 0.0;
float linearAttenuation = 0.0;
float quadraticAttenuation = 1.75;

void
main()
{
	vec4 texColor = texture2D(texture, texCoord);
    // Brightest color
	vec4 color = vec4(1.0,0.0,0.0,1.0);

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
		color = intensity * color;

		// Add lighting
		vec3 N = normalize(fN);
		vec3 E = normalize(fE);
		vec3 L = normalize(fL);

		vec3 H = normalize(L + E);

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
	}
    gl_FragColor = color;
}

