#include "renderer.h"


void CudaRenderer::initRenderer() {
	int numPixels = imgWidth * imgHeight;
	checkCudaErrors(cudaMalloc((void**)&d_Fb, sizeof(Framebuffer)));
	checkCudaErrors(cudaMemcpy(d_Fb, &h_Fb, sizeof(Framebuffer), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMalloc((void**)&d_camera, sizeof(Camera)));
	checkCudaErrors(cudaMalloc((void**)&d_World, sizeof(HittableList)));
	checkCudaErrors(cudaMalloc((void**)&d_randStates, numPixels * sizeof(curandState)));
}

CudaRenderer::~CudaRenderer() {
	checkCudaErrors(cudaFree(d_List));
	d_List = nullptr;
	checkCudaErrors(cudaFree(d_World));
	d_World = nullptr;
	checkCudaErrors(cudaFree(d_Fb));
	d_Fb = nullptr;
	checkCudaErrors(cudaFree(d_camera));
	d_camera = nullptr;
	checkCudaErrors(cudaFree(d_randStates));
	d_randStates = nullptr;
	checkCudaErrors(cudaFree(d_vertices));
	d_vertices = nullptr;
	checkCudaErrors(cudaFree(d_indices));
	d_indices = nullptr;
	checkCudaErrors(cudaFree(d_bvhNodes));
	d_bvhNodes = nullptr;
	if (glResource) cudaGraphicsUnregisterResource(glResource); // TODO: consider decoupling gl from renderer - move to separate class
}

void CudaRenderer::registerGLTexture(GLuint glTex) {
	cudaGraphicsGLRegisterImage(&glResource, glTex, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsSurfaceLoadStore);
}

void CudaRenderer::setupScene(Camera& camera) {
	std::vector<TriangleInfo> triangles{};
	for (auto mesh : meshDescriptors) {
		for (int i = 0; i < mesh.triangleCount; i++) {
			int idx0 = h_indices[mesh.indexOffset + i * 3 + 0];
			int idx1 = h_indices[mesh.indexOffset + i * 3 + 1];
			int idx2 = h_indices[mesh.indexOffset + i * 3 + 2];
			AABB bbox = buildTriangleAABB(h_vertices[idx0], h_vertices[idx1], h_vertices[idx2]);
			triangles.emplace_back(mesh.indexOffset + i * 3, bbox);
		}
	}
	AABB totalSceneAABB{};
	AABB totalCentroidAABB{};
	for (const auto& tri : triangles) {
		totalSceneAABB.expand(tri.bbox);
		totalCentroidAABB.expand(AABB(tri.centroid, tri.centroid));
	}
	BuildNode* root = builder.build(triangles);
	std::vector<LinearBVHNode> flattenedNodes(builder.getTotalNodes());
	std::vector<int> h_indicesSorted{};
	int offset = 0;
	builder.flattenBVH(root, flattenedNodes, offset);
	h_indicesSorted.reserve(h_indices.size());
	for (const auto& tri : triangles) {
		h_indicesSorted.push_back(h_indices[tri.triangleIndex]);
		h_indicesSorted.push_back(h_indices[tri.triangleIndex + 1]);
		h_indicesSorted.push_back(h_indices[tri.triangleIndex + 2]);
	}
	root->destroy();
	delete root;


	int numPixels = imgWidth * imgHeight;
	int vertexAmount = h_vertices.size();
	int nodeAmount = flattenedNodes.size();
	int indexAmount = h_indices.size();
	int meshAmount = meshDescriptors.size();
	checkCudaErrors(cudaMalloc((void**)&d_List, numObjects * sizeof(Hittable*)));
	checkCudaErrors(cudaMalloc((void**)&d_vertices, vertexAmount * sizeof(glm::vec3)));
	checkCudaErrors(cudaMalloc((void**)&d_indices, indexAmount * sizeof(int)));
	checkCudaErrors(cudaMalloc((void**)&d_bvhNodes, flattenedNodes.size() * sizeof(LinearBVHNode)));

	MeshDescriptor* d_meshDescriptors;
	checkCudaErrors(cudaMalloc((void**)&d_meshDescriptors, meshAmount * sizeof(MeshDescriptor)));
	checkCudaErrors(cudaMalloc((void**)&d_meshes, meshAmount * sizeof(Mesh)));

	checkCudaErrors(cudaMemcpy(d_bvhNodes, flattenedNodes.data(), nodeAmount * sizeof(LinearBVHNode), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_vertices, h_vertices.data(), vertexAmount * sizeof(glm::vec3), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_indices, h_indicesSorted.data(), indexAmount * sizeof(int), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_meshDescriptors, meshDescriptors.data(), meshAmount * sizeof(MeshDescriptor), cudaMemcpyHostToDevice));

	checkCudaErrors(cudaMemcpy(d_camera, &camera, sizeof(Camera), cudaMemcpyHostToDevice));
	initCamera<<<1, 1>>>(d_camera, imgWidth, imgHeight);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	dim3 blocks(imgWidth / xBlock + 1, imgHeight / yBlock + 1);
	dim3 threads(xBlock, yBlock);
	utils::random::randomInit<<<blocks, threads>>>(d_randStates, imgWidth, imgHeight);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	skybox.allocCubemap();
	createWorld<<<1, 1>>>(d_List, d_World, d_vertices, d_indices, d_meshDescriptors, meshAmount, d_meshes, d_bvhNodes, numObjects);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	checkCudaErrors(cudaFree(d_meshDescriptors));
}

int CudaRenderer::render(Camera& camera, bool resetFrameIndex) { // TODO: profile this
	if (resetFrameIndex) frameIndex = 1;
	checkCudaErrors(cudaMemcpy(d_camera, &camera, sizeof(Camera), cudaMemcpyHostToDevice));
	initCamera<<<1, 1>>>(d_camera, imgWidth, imgHeight);
	checkCudaErrors(cudaDeviceSynchronize());
	cudaSurfaceObject_t surfObj = 0;
	if (glResource) {
		cudaArray_t cuArray;
		checkCudaErrors(cudaGraphicsMapResources(1, &glResource));
		checkCudaErrors(cudaGraphicsSubResourceGetMappedArray(&cuArray, glResource, 0, 0));
		cudaResourceDesc resDesc = {};
		resDesc.resType = cudaResourceTypeArray;
		resDesc.res.array.array = cuArray;
		checkCudaErrors(cudaCreateSurfaceObject(&surfObj, &resDesc));
	}

	dim3 blocks(imgWidth / xBlock + 1, imgHeight / yBlock + 1);
	dim3 threads(xBlock, yBlock);

	renderScene<<<blocks, threads>>>(d_Fb, d_camera, d_World, d_randStates, frameIndex, skybox.getTextureObject(), surfObj);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	frameIndex++;
	if (glResource) {
		checkCudaErrors(cudaDestroySurfaceObject(surfObj));
		checkCudaErrors(cudaGraphicsUnmapResources(1, &glResource));
	}

	return 0;
}

void CudaRenderer::destroyScene() {
	skybox.destroyGPUBuffers();
	destroyWorld<<<1, 1>>>(d_List, d_World, d_meshes);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
}

void CudaRenderer::setMeshData(std::vector<glm::vec3>& vertexArray, std::vector<int>& indexArray, std::vector<MeshDescriptor>& descriptors) {
	h_vertices = vertexArray;
	h_indices = indexArray;
	meshDescriptors = descriptors;
}