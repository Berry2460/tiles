#include "window.h"
#include "draw.h"
#include "GLFW/glfw3.h"

static char* title;
static float start;
static GLFWwindow* window;

static void mouse(GLFWwindow* window, double x, double y);
static void scroll(GLFWwindow* window, double xoffset, double yoffset);
static void mouseControl(GLFWwindow* window, int key, int action, int mods);
static void buttons(GLFWwindow* window, int key, int scancode, int action, int mods);

static void mouse(GLFWwindow* window, double x, double y){ //update mouse
	mouseX=x;
	mouseY=y;
}

static void scroll(GLFWwindow* window, double xoffset, double yoffset){ //zoom
	if (yoffset > 0){
		scale+=SCALE_AMOUNT*scale;
		if (scale > MAX_SCALE){scale=MAX_SCALE;}
	}
	else if (yoffset < 0){
		scale-=SCALE_AMOUNT*scale;
		if (scale < MIN_SCALE){scale=MIN_SCALE;}
	}
}

static void mouseControl(GLFWwindow* window, int key, int action, int mods){
	switch(key){
		case GLFW_MOUSE_BUTTON_LEFT:
			if (action == GLFW_PRESS){keys[LMB]=true;}
			else if (action == GLFW_RELEASE){keys[LMB]=false;}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (action == GLFW_PRESS){keys[RMB]=true;}
			else if (action == GLFW_RELEASE){keys[RMB]=false;}
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
			if (action == GLFW_PRESS){keys[KEY_W]=true;}
			else if (action == GLFW_RELEASE){keys[KEY_W]=false;};
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS){keys[KEY_S]=true;}
			else if (action == GLFW_RELEASE){keys[KEY_S]=false;};
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS){keys[KEY_A]=true;}
			else if (action == GLFW_RELEASE){keys[KEY_A]=false;};
			break;
		case GLFW_KEY_D:
			if (action == GLFW_PRESS){keys[KEY_D]=true;}
			else if (action == GLFW_RELEASE){keys[KEY_D]=false;};
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
	if (fullscreen){
		window=glfwCreateWindow(screenWidth, screenHeight, title, glfwGetPrimaryMonitor(), NULL);
	}
	else{
		window=glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
	}
	if (!window){glfwTerminate();return -1;}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, buttons);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window, mouseControl);
	glfwSwapInterval(vsync);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//textures and transparency
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//allocate memory
	textures=malloc(MAX_TEXTURES*sizeof(GLuint));
	glGenTextures(MAX_TEXTURES, textures);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	return 0;
}

bool windowLoop(){
	if (!glfwWindowShouldClose(window)){
		//fps
		frames++;
		fps=1/(glfwGetTime()-start);
		start=glfwGetTime();
		if (frames > 300){
			char out[128];
			sprintf(out, "%s FPS: %d", title, fps); //convert to chars
			glfwSetWindowTitle(window, out);
			frames=0;
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return !glfwWindowShouldClose(window);
}
