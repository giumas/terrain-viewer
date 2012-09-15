varying float color_intensity;

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
    // Gradient colors
    vec4 color_high = vec4(0.40, 0.6, 0.3, 1.0);
    vec4 color_low = vec4(0.1, 0.2, 0.07, 1.0);
    vec4 color;

    // If wireframe, just set color to black and skip lighting
    if(wireframe > 0.5) {
        color = vec4(0.0,0.0,0.0,1.0);
    }else {
        // Calculate final color based on elevation
        if(color_intensity > 1.0) {
            color_intensity = 1.0;
        }else if(color_intensity < 0.0) {
            color_intensity = 0.0;
        }
        color = color_intensity * color_high + (1.0 - color_intensity) * color_low;

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

