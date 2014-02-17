#version 330 core
in vec3 UV;
in vec3 lightDir_tangentSpace;
in vec3 cameraPos_tangentSpace;
uniform sampler2DArray color_texture;
uniform sampler2DArray normal_texture;
uniform float render_distance;
//uniform sampler2DArray height_texture;
const float parallaxScale = 0.08;
const float parallaxBias = -0.03;
out vec4 color;
in float brightness;
void main(void)
{

	float z = min(gl_FragCoord.z / gl_FragCoord.w + 1.0f,render_distance);
	float height = texture(normal_texture, UV).b * parallaxScale + parallaxBias;
	vec3 newUV = UV + vec3(cameraPos_tangentSpace.xy * height,0.);
	vec3 normal = normalize(texture(normal_texture, newUV).rgb * 2.0 - 1.0);
	vec4 c = texture(color_texture, newUV) * (clamp(dot(normal, -lightDir_tangentSpace), 0.05, 1.)) * brightness;
	color = (c - (c - vec4(0.39, 0.58, 0.93,1.0)) * (z/render_distance));
}