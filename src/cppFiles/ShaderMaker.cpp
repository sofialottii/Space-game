
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include "ShaderMaker.h"


#pragma warning(disable:4996)
char* ShaderMaker::readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

GLuint ShaderMaker::createProgram(char* vertexfilename, char* fragmentfilename)
{

	int success;
	char infoLog[512];


	// Creiamo gli eseguibili degli shader
	//Leggiamo il codice del Vertex Shader
	GLchar* VertexShader = readShaderSource(vertexfilename);
	//Visualizzo sulla console il CODICE VERTEX SHADER
	//cout << VertexShader;

	//Generiamo un identificativo per il vertex shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	//Associamo all'identificativo il codice del vertex shader
	glShaderSource(vertexShaderId, 1, (const char**)&VertexShader, NULL);
	//Compiliamo il Vertex SHader
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	//Leggiamo il codice del Fragment Shader
	const GLchar* FragmentShader = readShaderSource(fragmentfilename);

	//Generiamo un identificativo per il FRAGMENT shader
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&FragmentShader, NULL);
	//Compiliamo il FRAGMENT SHader
	glCompileShader(fragmentShaderId);


	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Creiamo un identificativo di un eseguibile e gli colleghiamo i due shader compilati
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	return programId;
}
