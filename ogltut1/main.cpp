#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<fstream>

//initialize window

int main()
{
	if (!glfwInit())
	{
		std::cout << "failed to initialize GLFW" << std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);

	if (window==nullptr)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;

	}
	
	float vertices[] =
	{ //vertex positions	//vertex colors
		-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,	 //bottom left
		 0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,	//bottom right
		 0.0f, 0.5f,0.0f, 0.0f,0.0f,1.0f	//top
	};

		const char* vertexShaderSource = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;

			out vec3 ourColor;

			void main()
			{
				gl_Position = vec4(aPos, 1.0);
				ourColor = aColor;
			}


			)";

		const char* fragmentShaderSource = R"(

			#version 330 core
			in vec3 ourColor;
			out vec4 FragColor;
			void main()
			{
					FragColor = vec4(ourColor, 1.0);

			}

)";
	glfwMakeContextCurrent(window);

	if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)==0)
	{
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}
		unsigned int VAO, VBO;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(1);


		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource,NULL);

		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

		glCompileShader(fragmentShader);


		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);



	while (glfwWindowShouldClose(window) == false)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;


}