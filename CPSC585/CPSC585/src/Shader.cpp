#include "Shader.h"

char* Shader::textFileRead(const char* fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int Shader::textFileWrite(char* fn, char* s) 
{
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {

			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}


Shader::Shader()
{
	v = 0;
	f = 0;
	p = 0;
}

Shader::Shader(char* vert, char* frag)
{
	const char* vSource = textFileRead(vert);
	const char* fSource = textFileRead(frag);

	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vSource, NULL);
	glCompileShader(v); 

	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &fSource, NULL);
	glCompileShader(f); 

	p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);

}

Shader::~Shader()
{

}

// returns the address of a uniform give it's name
GLint Shader::getUniform(const char* name)
{
	GLint loc = glGetUniformLocation(p, name);

	return loc;
}

// bind the shader to opengl
void Shader::turnShadersOn()
{	
	glUseProgram(p);
}

void Shader::turnShadersOff()
{
	glUseProgram(0);
}


void Shader::debug()
{
	// Debugging information for shader
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(p, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
	    infoLog = (char *)malloc(infologLength);
	    glGetProgramInfoLog(p, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
	    free(infoLog);
	}
}
