
#version 330 core

in vec3 Normal;
in vec3 FragPos;  // позиция фрагмента

out vec4 FragColor;

// Структура источника света
struct Light {
    vec3 position;  // Позиция источника света в мировом пространстве
    vec3 ambient;   // Интенсивность фонового освещения
    vec3 diffuse;   // Интенсивность рассеянного освещения
    vec3 specular;  // Интенсивность бликового освещения
};

// Структура материала
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; 
};

uniform Light light; // Объявление uniform-переменной свет
uniform Material material; // Объявление uniform-переменной материал
uniform vec3 viewPos; // Позиция камеры



void main() {
    // 1. Ambient (Окружающее освещение)
    vec3 ambient = light.ambient * material.ambient;
  	
    // 2. Diffuse (Диффузное освещение)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // 3. Specular (Зеркальное освещение - блик)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    // Итоговый цвет
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

};

