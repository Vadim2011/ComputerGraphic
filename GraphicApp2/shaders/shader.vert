
#version 330 core
layout (location = 0) in vec3 aPos;     // Позиция вершины
layout (location = 1) in vec3 aNormal;  // Нормаль вершины

// Выходные переменные во фрагментный шейдер
out vec3 Normal;
out vec3 FragPos; // Дополнительно: позиция фрагмента в мире

// Униформы (матрицы трансформации)
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    // Трансформируем позицию вершины: Proj * View * Model * Position
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    
    // Передаем позицию вершины в мировых координатах
    FragPos = vec3(model * vec4(aPos, 1.0));

   Normal = mat3(transpose(inverse(model))) * aNormal;
};



