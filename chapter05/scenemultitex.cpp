#include "scenemultitex.h"
#include "texture.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneMultiTex::SceneMultiTex() : cube() { }

void SceneMultiTex::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(1.0f,1.25f,1.25f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

	prog.setUniform("Light.L", vec3(1.0f));
	prog.setUniform("Light.La", vec3(0.15f) );

    brick = Texture::loadTexture("../media/texture/brick1.jpg");
	moss = Texture::loadTexture("../media/texture/moss.png");

	// Load brick texture file into channel 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brick);
	// Load moss texture file into channel 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, moss);

#ifdef __APPLE__
    prog.setUniform("BaseTex", 0);
    prog.setUniform("MossTex", 1);
#endif
}

void SceneMultiTex::update( float t ) { }

void SceneMultiTex::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", glm::vec4(0.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Material.Ks", 0.05f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    setMatrices();
    cube.render();
}

void SceneMultiTex::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneMultiTex::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneMultiTex::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/multitex_41.vs");
    prog.compileShader("shader/multitex_41.fs");
#else
    prog.compileShader("shader/multitex.vert.glsl");
    prog.compileShader("shader/multitex.frag.glsl");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
