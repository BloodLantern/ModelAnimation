#include "ui_window.h"

#include "GLFW/glfw3.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "animation_programming.h"

#include <iostream>
#include <numbers>
#include <thread>

#ifdef NOENGINE
UiWindow::UiWindow()
	: m_Skeleton(new Skeleton)
#else
UiWindow::UiWindow(Skeleton* skeleton)
	: m_Skeleton(skeleton)
#endif
{
#ifdef NOENGINE
	m_Skeleton->Load("Resources/ThirdPersonWalk.skel");

	for (Bone& bone : m_Skeleton->GetBones())
		bone.ComputeTransform();

	m_Skeleton->SetupFamily();
	Main();
#else
	StartThread();
#endif
}

UiWindow::~UiWindow()
{
#ifndef NOENGINE
	EndThread();
#else
	delete m_Skeleton;
#endif
}

#ifndef NOENGINE
void UiWindow::StartThread()
{
    m_CloseWindow = false;
    m_Thread = std::thread(&UiWindow::Main, this);
}

void UiWindow::EndThread()
{
    if (m_Thread.joinable())
        m_Thread.join();
}
#endif

void UiWindow::Main()
{
	// Setup GLFW
	glfwSetErrorCallback(
		[](int error, const char* description)
		{
			std::cerr << "GLFW error " << error << ": " << description << '\n';
		}
	);

	if (!glfwInit())
		return;

	// GL 3.0 + GLSL 130
	const char* glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "UI", NULL, NULL);

	if (window == NULL)
		return;

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // Enable vsync

	// Initialize GLAD
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));  // NOLINT(clang-diagnostic-cast-function-type-strict)

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports
	
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

#ifdef NOENGINE
	m_Mesh = new Mesh("Resources/malphite.glb");
	m_Mesh->Forward();

	m_Shader = new Shader("Resources", "shader.vs", "shader.fs");
#endif

	// Show window
	glfwShowWindow(window);
#ifndef NOENGINE
	glfwHideWindow(window);
#endif
	
	while (
		!glfwWindowShouldClose(window)
#ifndef NOENGINE
		&& !m_CloseWindow
#endif
		)
	{
		glfwPollEvents();

		glfwGetWindowPos(window, &m_WindowX, &m_WindowY);

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

#ifdef NOENGINE
		m_Shader->SetUniform("modelViewMatrix", Matrix4x4::TRS(vec3(0.f, -5.f, 10.f), vec3(0.f), 0.05f));
		Matrix4x4 projection;
		Matrix4x4::PerspectiveProjectionMatrix(std::numbers::pi_v<float> / 2.f, 16.f / 9.f, 0.01f, 1000.f, projection);
		m_Shader->SetUniform("projectionMatrix", projection);

		m_Shader->Use();
		m_Mesh->Draw();
#endif

		DrawSkeletonHierarchy();
		DrawCurrentBoneInfo();

		DrawAnimations();

		// Rendering
		ImGui::Render();
		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);

		glfwSwapBuffers(window);
	}
	
#ifdef NOENGINE
	delete m_Shader;
	delete m_Mesh;
#endif

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}




void UiWindow::DrawBoneInSkeletonHierarchy(Bone& parentBone)
{
	for (Bone* const bone : parentBone.GetChildren())
	{
		if (!ImGui::TreeNodeEx(bone->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
			continue;

		if (ImGui::IsItemClicked())
			m_SelectedBone = bone;

		DrawBoneInSkeletonHierarchy(*bone);
		ImGui::TreePop();
	}
}

void UiWindow::DrawCurrentBoneInfo() const
{
	if (!m_SelectedBone)
		return;

	ImGui::Begin("Current bone");

	ImGui::BeginDisabled();

	ImGui::Text("%s", m_SelectedBone->GetName().c_str());
	ImGui::InputFloat3("Position", &m_SelectedBone->Position.x);
	ImGui::InputFloat4("Rotation", &m_SelectedBone->Rotation.imaginary.x);

	ImGui::Dummy(ImVec2(10.f, 25.f));
	ImGui::InputFloat4("Local row 1", &const_cast<Matrix4x4&>(m_SelectedBone->GetLocalTransform())[0].x);
	ImGui::InputFloat4("Local row 2", &const_cast<Matrix4x4&>(m_SelectedBone->GetLocalTransform())[1].x);
	ImGui::InputFloat4("Local row 3", &const_cast<Matrix4x4&>(m_SelectedBone->GetLocalTransform())[2].x);
	ImGui::InputFloat4("Local row 4", &const_cast<Matrix4x4&>(m_SelectedBone->GetLocalTransform())[3].x);

	ImGui::Dummy(ImVec2(10.f, 25.f));
	ImGui::InputFloat4("Global row 1", &const_cast<Matrix4x4&>(m_SelectedBone->GetGlobalTransform())[0].x);
	ImGui::InputFloat4("Global row 2", &const_cast<Matrix4x4&>(m_SelectedBone->GetGlobalTransform())[1].x);
	ImGui::InputFloat4("Global row 3", &const_cast<Matrix4x4&>(m_SelectedBone->GetGlobalTransform())[2].x);
	ImGui::InputFloat4("Global row 4", &const_cast<Matrix4x4&>(m_SelectedBone->GetGlobalTransform())[3].x);
	ImGui::EndDisabled();

	ImGui::End();
}

void UiWindow::DrawAnimations()
{
	if (!m_Animations)
		return;

	ImGui::Begin("Animations");

	if (m_MixedAnimationAlpha)
	{
		ImGui::SliderFloat("Mixed alpha", m_MixedAnimationAlpha, 0.f, 1.f);
	}

	for (Animation& a : *m_Animations)
	{
		if (!ImGui::CollapsingHeader(a.GetName().c_str()))
			continue;

		ImGui::PushID(&a);

		const size_t keyCount = a.GetKeyCount();
		ImGui::Text("Key count : %d", keyCount);
		ImGui::SliderInt("Current frame", (int*)&a.CurrentFrame, 0, keyCount - 1);
		ImGui::Checkbox("Paused", &a.Paused);
		ImGui::InputFloat("Delta modulation", &a.Speed, 0.1f, 1.f);

		for (size_t i = 0; i < keyCount; i++)
		{
			std::string txt = std::string("Frame ").append(std::to_string(i));

			if (!ImGui::TreeNode(txt.c_str()))
				continue;

			for (size_t j = 0; j < a.GetBoneCount(); j++)
			{
				txt = std::string("Bone ").append(std::to_string(j));
				if (!ImGui::TreeNode(txt.c_str()))
					continue;

				const KeyFrame& keyFrame = a.GetKeyFrame(i, j);

				ImGui::Text("%s", m_Skeleton->GetBone(j).GetName().c_str());

				ImGui::BeginDisabled();
				ImGui::InputFloat3("Position", &const_cast<Vector3&>(keyFrame.GetPosition()).x);
				ImGui::InputFloat4("Rotation", &const_cast<Quaternion&>(keyFrame.GetRotation()).imaginary.x);

				ImGui::EndDisabled();
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	ImGui::End();
}

void UiWindow::DrawSkeletonHierarchy()
{
	// Temporary, multithreading breaks stuff probably
	return;
	ImGui::Begin("Skeleton");

	if (m_Skeleton)
		DrawBoneInSkeletonHierarchy(m_Skeleton->GetRoot());
	else
		ImGui::Text("[NO SKELETON]");

	ImGui::End();
}

void UiWindow::SetAnimations(std::vector<Animation>* animations)
{
	m_Animations = animations;
}

void UiWindow::SetMixedAnimationAlpha(float* alpha)
{
	m_MixedAnimationAlpha = alpha;
}

#ifndef NOENGINE
void UiWindow::Close()
{
	// Called when the main window is closed, forcefully close the UI window and join the thread
	m_CloseWindow = true;
	m_Skeleton = nullptr;
}
#endif
