#version 330 core
struct Material {
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

    // Combine os resultados
    vec3 ambient = light.ambient * mat.ka;
    vec3 diffuse = light.diffuse * diff * mat.kd;
    vec3 specular = light.specular * spec * mat.ks;
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

    // combina os resultados
    vec3 ambient = light.ambient * material.ka;
    vec3 diffuse = light.diffuse * diff * material.kd;
    vec3 specular = light.specular * spec * material.ks;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}