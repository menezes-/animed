#version 330 core
struct Material {
    sampler2D texture_ambient1;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    //cores
    vec3 ka;
    vec3 kd;
    vec3 ks;

    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


//representa um caster de luz
struct PointLight {
    vec3 position;
    
    //valores:http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// quantos pontos de luz eu tenho
#define N_POINT_LIGHTS 4

in vec3 fragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[N_POINT_LIGHTS];
uniform SpotLight spotLights;
uniform Material material;

// Function prototypes
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{    
    vec3 result;
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 norm = normalize(Normal);
    
    for(int i = 0; i < N_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], material, norm, fragPosition, viewDir);
    }
    result += CalcSpotLight(spotLights, material, norm, fragPosition, viewDir);
    color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //combina as cores das luzes com as texturas
    vec4 ambient_color = texture(mat.texture_ambient1, TexCoords) * vec4(mat.ka, 1.0f);
    vec4 diffuse_color = texture(mat.texture_diffuse1, TexCoords) * vec4(mat.kd, 1.0f);
    vec4 specular_color = texture(mat.texture_specular1, TexCoords) *  vec4(mat.ks, 1.0f);

    // Combine os resultados
    vec3 ambient = light.ambient * ambient_color.rgb;
    vec3 diffuse = light.diffuse * diff * diffuse_color.rgb;
    vec3 specular = light.specular * spec * specular_color.rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // caclula intensidade nas bordas
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); // basicamente um if(dentro_do_cone)

    //interpola a textura  com as cores do material

    vec4 ambient_color = texture(material.texture_ambient1, TexCoords) * vec4(material.ka, 1.0f);
    vec4 diffuse_color = texture(material.texture_diffuse1, TexCoords) * vec4(material.kd, 1.0f);
    vec4 specular_color = texture(material.texture_specular1, TexCoords) * vec4(material.ks, 1.0f);

    // combina os resultados
    vec3 ambient = light.ambient * ambient_color.rgb;
    vec3 diffuse = light.diffuse * diff * diffuse_color.rgb;
    vec3 specular = light.specular * spec * specular_color.rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}