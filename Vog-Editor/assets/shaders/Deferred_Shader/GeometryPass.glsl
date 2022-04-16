#type vertex
#version 450 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in vec3 a_tangent;

out VertexData
{
    vec3 pos;
    vec3 ws_pos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;

    vec4 lightSpace_pos;
} v2f;

layout(std140, binding = 0) uniform ub_camera
{
	mat4 u_view;
	mat4 u_projection;
	mat4 u_viewProjection;
    mat4 u_lightSpace_matrix;
};
uniform mat4 u_transform;

void main()
{
	mat3 normal_matrix = transpose(inverse(mat3(u_transform)));

	v2f.pos = a_position;
	v2f.ws_pos = (u_transform * vec4(a_position, 1.0)).xyz;
	v2f.normal =  normalize(normal_matrix * a_normal);
	v2f.texCoord = a_texCoord;
	v2f.tangent = normalize(normal_matrix * a_tangent);

    v2f.lightSpace_pos = u_lightSpace_matrix * vec4(v2f.ws_pos, 1.0);

	gl_Position = u_viewProjection * vec4(v2f.ws_pos, 1.0);	
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 o_poisiton;
layout(location = 1) out vec4 o_normal;
layout(location = 2) out vec4 o_albedoSpec;

in VertexData
{
    vec3 pos;
    vec3 ws_pos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;

    vec4 lightSpace_pos;
} v2f;

layout(binding = 0) uniform sampler2D u_albedo_map;
layout(binding = 1) uniform sampler2D u_normal_map;
layout(binding = 2) uniform sampler2D u_specular_map;

void main()
{
	vec4 position = vec4(v2f.ws_pos, 1.0);
	//vec4 normal = vec4(v2f.normal, 1.0);
	//vec4 albedo = texture(u_albedo_map, v2f.texCoord);

	vec4 normal     = mix(vec4(v2f.normal, 1.0), texture(u_normal_map, v2f.texCoord), 0.0);
	vec4 albedo     = mix(vec4(1.0, 1.0, 1.0, 1.0), texture(u_albedo_map, v2f.texCoord), 1.0).rgba;
    albedo.a        = mix(1.0, texture(u_specular_map, v2f.texCoord).r, 1.0);
    // if (col.a < 0.0000001)
    // {
    //     discard;
    // }
    // vec3 normalMap = texture(texture_normal, v2f.uv).rgb;
    // normalMap = normalMap * 2.0 - 1.0;   
    // normalMap = normalize(v2f.TBN * normalMap * 0.5 + 0.5);

    // vec3 norm = mix(normalize(v2f.normal * 0.5 + 0.5) , normalMap, material.hasNoramlMap).rgb;
    // //norm = normalize(v2f.normal* 0.5 + 0.5) ;
    // //float specularHighlight = mix(material.specularHighlight, texture(texture_specualr, v2f.uv).r, material.hasSpecularMap);

    // float shininess = log2(material.shininess) / 10.5;

    // gPosition = v2f.w_pos;
    // //gPosition = v2f.vs_pos;

    // gNormal = norm;
    o_poisiton = position;
    o_normal = normal;
    o_albedoSpec = albedo;
}