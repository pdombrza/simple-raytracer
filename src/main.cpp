#include <iostream>
#include <memory>
#include <chrono>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "camera/camera.h"
#include "camera/camera_controller.h"
#include "renderer/renderer.h"
#include "hittable/hittable.h"
#include "hittablelist/hittablelist.h"
#include "material/material.h"
#include "kernel/kernel.h"
#include "shader/shader.h"
#include "window/window.h"
#include "window/windinput.h"
#include "aabb/aabb.h"
#include "bvh/bvh.h"

void processInput(GLFWwindow* window);
void addBoxToScene(std::vector<glm::vec3>& vertices, std::vector<int>& indices, std::vector<MeshDescriptor>& descriptors);
void addPyramidToScene(std::vector<glm::vec3>& vertices, std::vector<int>& indices, std::vector<MeshDescriptor>& descriptors);

int main() {
	{
		int runtimeVersion = 0;
		cudaRuntimeGetVersion(&runtimeVersion);
		std::cout << "CUDA Runtime Version: " << runtimeVersion / 1000 << "." << (runtimeVersion % 1000) / 10 << "\n";

		if (!glfwInit()) { // TODO: move OpenGL related code to Window class
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return -1;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		int width = 1200;
		int height = 800;
		GLFWwindow* window = glfwCreateWindow(width, height, "RT", NULL, NULL);
		if (!window) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(0); 

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			glfwDestroyWindow(window);
			glfwTerminate();
			return -1;
		}

		Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");

		float vertices[] = {
			-1.f, -1.f,   0.f, 0.f,
			 1.f, -1.f,   1.f, 0.f,
			 1.f,  1.f,   1.f, 1.f,

			-1.f, -1.f,   0.f, 0.f,
			 1.f,  1.f,   1.f, 1.f,
			-1.f,  1.f,   0.f, 1.f
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int VAO, VBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		GLuint PBO;
		cudaGraphicsResource* cudaPBOResource;

		glGenBuffers(1, &PBO);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * sizeof(uchar4), nullptr, GL_DYNAMIC_DRAW);
		cudaGraphicsGLRegisterBuffer(&cudaPBOResource, PBO, cudaGraphicsMapFlagsWriteDiscard);

		GLuint glTex;
		glGenTextures(1, &glTex);
		glBindTexture(GL_TEXTURE_2D, glTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int xBlock = 16;
		int yBlock = 16;
		std::cerr << "Rendering a " << width << "x" << height << " image " << std::endl;
		std::cerr << "in " << xBlock << "x" << yBlock << " blocks" << std::endl;
		int numPixels = width * height;
		
		CameraOrientation orientation;
		orientation.lookFrom = glm::vec3(0.0f, 0.0f, 1.0f);
		orientation.lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
		orientation.vUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Camera h_camera(orientation, 90.0f, (float)width / (float)height);
		//h_camera.setVFov(20.0f);
		//h_camera.setDefocusAngle(0.6f);
		//h_camera.setFocusDist(10.0f);
		CameraController controller(orientation);
		glfwSetWindowUserPointer(window, &controller);
		glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
			static float lastX = 600, lastY = 400;
			static bool firstMouse = true;

			if (firstMouse) {
				lastX = xpos; lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			lastX = xpos; lastY = ypos;

			auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
			ctrl->processMouse(xoffset, yoffset);
		});

		// prepare vertex array, index array and mesh descriptors
		HittableList scene{};
		std::vector<glm::vec3> h_vertices{};
		std::vector<int> h_indices{};
		std::vector<MeshDescriptor> h_meshDescriptors{};

		// Load the models
		std::string inputfile = MODELS_PATH "marble_bust_01_4k.obj";
		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = MODELS_PATH;

		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(inputfile, reader_config)) {
			if (!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
			exit(1);
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();
		h_vertices.reserve(attrib.vertices.size() / 3);
		h_indices.reserve(attrib.vertices.size() / 3); // 
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			MeshDescriptor desc{};
			desc.vertexOffset = h_vertices.size();
			desc.indexOffset = h_indices.size();
			size_t indexOffset = 0;
			std::unordered_map<glm::vec3, int> uniqueVertices{};
			int vertexCounter = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				for (size_t v = 0; v < 3; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

					glm::vec3 vertex = {
						attrib.vertices[3 * idx.vertex_index + 0],
						attrib.vertices[3 * idx.vertex_index + 1],
						attrib.vertices[3 * idx.vertex_index + 2]
					};
					if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
						uniqueVertices[vertex] = vertexCounter++; // Assign it the next available slot
						h_vertices.emplace_back(vertex);
					}
					h_indices.push_back(uniqueVertices[vertex]);
				}
				indexOffset += 3;
			}
			int triangleCount = (int) (h_indices.size() - desc.indexOffset) / 3;
			desc.triangleCount = triangleCount;
			h_meshDescriptors.push_back(desc);
		}

		addBoxToScene(h_vertices, h_indices, h_meshDescriptors);
		addPyramidToScene(h_vertices, h_indices, h_meshDescriptors);
		CudaRenderer renderer(&scene, width, height);
		renderer.setNumObjects(6);
		renderer.setNumMeshes(3);	
		renderer.setMeshData(h_vertices, h_indices, h_meshDescriptors);
		renderer.registerGLTexture(glTex);
		renderer.setupScene(h_camera);
		shader.use();

		glViewport(0, 0, width, height);
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); });
		float lastFrame = 0.0f;

		while (!glfwWindowShouldClose(window)) {
			float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			controller.handleInputs(window, deltaTime);
			bool moved = controller.isChanged();
			if (moved) {
				orientation = controller.getOrientation();
				h_camera.setCameraOrientation(orientation);
				controller.clearChanged();
			}

			renderer.render(h_camera, moved);

			glClear(GL_COLOR_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, glTex);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		renderer.destroyScene();
	}

	cudaDeviceReset();
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void addBoxToScene(std::vector<glm::vec3>& vertices, std::vector<int>& indices, std::vector<MeshDescriptor>& descriptors) {
	MeshDescriptor desc;
	desc.vertexOffset = vertices.size();
	desc.indexOffset = indices.size();
	desc.triangleCount = 12;

	vertices.push_back({ -3.5, -0.5, -0.5 });
	vertices.push_back({ -2.5, -0.5, -0.5 });
	vertices.push_back({ -2.5,  0.5, -0.5 });
	vertices.push_back({ -3.5,  0.5, -0.5 });
	vertices.push_back({ -3.5, -0.5,  0.5 });
	vertices.push_back({ -2.5, -0.5,  0.5 });
	vertices.push_back({ -2.5,  0.5,  0.5 });
	vertices.push_back({ -3.5,  0.5,  0.5 });

	int boxIndices[] = {
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};
	for (int i : boxIndices) indices.push_back(i);

	descriptors.push_back(desc);
}


void addPyramidToScene(std::vector<glm::vec3>& vertices, std::vector<int>& indices, std::vector<MeshDescriptor>& descriptors) {
	MeshDescriptor desc;
	desc.vertexOffset = vertices.size();
	desc.indexOffset = indices.size();
	desc.triangleCount = 6;
	vertices.push_back({ 3.0f,  1.0f, -2.5f });
	vertices.push_back({ 2.5f,  0.0f, -2.0f }); 
	vertices.push_back({ 3.5f,  0.0f, -2.0f });
	vertices.push_back({ 2.5f,  0.0f, -3.0f });
	vertices.push_back({ 3.5f,  0.0f, -3.0f });
	int pyrIndices[] = {
		1, 2, 0,
		2, 4, 0,
		4, 3, 0,
		3, 1, 0,
		1, 3, 4,
		1, 4, 2
	};

	for (int i : pyrIndices) {
		indices.push_back(i);
	}

	descriptors.push_back(desc);
}