#include "window.h"

static void mouse(GLFWwindow* window, double x, double y){ //update mouse
	mouseX=x;
	mouseY=y;
}

static void scroll(GLFWwindow* window, double xoffset, double yoffset){ //zoom
	if (yoffset > 0){
		scale+=0.06f*scale;
		if (scale > 2.0f){scale=2.0f;}
	}
	else if (yoffset < 0){
		scale-=0.06f*scale;
		if (scale < 0.5f){scale=0.5f;}
	}
}

static void mouseControl(GLFWwindow* window, int key, int action, int mods){
	switch(key){
		case GLFW_MOUSE_BUTTON_LEFT:
			if (action == GLFW_PRESS){keys[LMB]=true;}
			else if (action == GLFW_RELEASE){keys[LMB]=false;}
			break;
	}
}

static void buttons(GLFWwindow* window, int key, int scancode, int action, int mods){
	switch (key){
		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS){keys[SPACE]=true;}
			else if (action == GLFW_RELEASE){keys[SPACE]=false;};
			break;
		case GLFW_KEY_W:
			if (action == GLFW_PRESS){keys[W]=true;}
			else if (action == GLFW_RELEASE){keys[W]=false;};
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS){keys[S]=true;}
			else if (action == GLFW_RELEASE){keys[S]=false;};
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS){keys[A]=true;}
			else if (action == GLFW_RELEASE){keys[A]=false;};
			break;
		case GLFW_KEY_D:
			if (action == GLFW_PRESS){keys[D]=true;}
			else if (action == GLFW_RELEASE){keys[D]=false;};
			break;
		case GLFW_KEY_LEFT_SHIFT:
			if (action == GLFW_PRESS){keys[SHIFT]=true;}
			else if (action == GLFW_RELEASE){keys[SHIFT]=false;};
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			if (action == GLFW_PRESS){keys[SHIFT]=true;}
			else if (action == GLFW_RELEASE){keys[SHIFT]=false;};
			break;
	}
}

int startWindow(char* winTitle){
	title=winTitle;
	start=glfwGetTime();
	if (!glfwInit()){return -1;}
	window=glfwCreateWindow(WIN_X, WIN_Y, title, NULL, NULL);
	if (!window){glfwTerminate();return -1;}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, buttons);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window, mouseControl);
	glfwSwapInterval(V_SYNC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//allocate memory
	glEnable(GL_TEXTURE_2D);
	textures=malloc(MAX_TEXTURES*sizeof(int));
	glGenTextures(MAX_TEXTURES, textures);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	return 0;
}

bool windowLoop(){
	if (!glfwWindowShouldClose(window)){
		//fps
		frames++;
		fps=frames/(glfwGetTime()-start+0.00001f);
		char out[128];
		sprintf(out, "%s FPS: %d", title, fps); //convert to chars
		if (glfwGetTime()-start > 1){
			start=glfwGetTime();
			frames=0;
			glfwSetWindowTitle(window, out);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return !glfwWindowShouldClose(window);
}
