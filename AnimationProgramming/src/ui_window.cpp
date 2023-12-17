#include "ui_window.h"

#include "GLFW/glfw3.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <iostream>
#include <thread>

UiWindow::UiWindow()
{
	StartThread();
}

UiWindow::~UiWindow()
{
	// Called when the main window is closed, forcefully close the UI window and join the thread
	m_CloseWindow = true;
	EndThread();
}

void UiWindow::Main()
{
	// Setup GLFW
	glfwSetErrorCallback(
		[](int error, const char* description)
		{
			std::cerr << "GLFW error " << error << ": " << description << std::endl;
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

	GLFWwindow* window = glfwCreateWindow(1, 1, "UI", NULL, NULL);

	if (window == NULL)
		return;

	// Disable closing the Ui window
	glfwSetWindowCloseCallback(window, [](GLFWwindow* w) { glfwSetWindowShouldClose(w, GLFW_FALSE); });

	// Set icon
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // Enable vsync

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

	// Show window
	glfwShowWindow(window);
	glfwHideWindow(window);

	while (!glfwWindowShouldClose(window) && !m_CloseWindow)
	{
		glfwPollEvents();

		glfwGetWindowPos(window, &m_WindowX, &m_WindowY);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		DrawSkeletonHierarchy();
		DrawCurrentBoneInfo();

		DrawAnimations();

		// Rendering
		ImGui::Render();
		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

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




void UiWindow::DrawBoneInSkeletonHierarchy(Bone& bone)
{
	for (Bone* const bone : bone.GetChildren())
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
	ImGui::InputFloat4("Local row 1", &m_SelectedBone->GetLocalTransform()[0].x);
	ImGui::InputFloat4("Local row 2", &m_SelectedBone->GetLocalTransform()[1].x);
	ImGui::InputFloat4("Local row 3", &m_SelectedBone->GetLocalTransform()[2].x);
	ImGui::InputFloat4("Local row 4", &m_SelectedBone->GetLocalTransform()[3].x);

	ImGui::Dummy(ImVec2(10.f, 25.f));
	ImGui::InputFloat4("Global row 1", &m_SelectedBone->GetGlobalTransform()[0].x);
	ImGui::InputFloat4("Global row 2", &m_SelectedBone->GetGlobalTransform()[1].x);
	ImGui::InputFloat4("Global row 3", &m_SelectedBone->GetGlobalTransform()[2].x);
	ImGui::InputFloat4("Global row 4", &m_SelectedBone->GetGlobalTransform()[3].x);
	ImGui::EndDisabled();

	ImGui::End();
}

void UiWindow::DrawAnimations()
{
	if (!m_Animations)
		return;

	ImGui::Begin("Animations");

	for (const Animation& a : *m_Animations)
	{
		if (!ImGui::CollapsingHeader(a.GetName().c_str()))
			continue;

		const size_t keyCount = a.GetKeyCount();
		ImGui::Text("Key count : %d", keyCount);

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

				ImGui::BeginDisabled();
				ImGui::InputFloat3("Position", &const_cast<Vector3&>(keyFrame.GetPosition()).x);
				ImGui::InputFloat4("Rotation", &const_cast<Quaternion&>(keyFrame.GetRotation()).imaginary.x);

				ImGui::EndDisabled();
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void UiWindow::DrawSkeletonHierarchy()
{
	if (!m_Skeleton)
		return;

	ImGui::Begin("Skeleton");

	DrawBoneInSkeletonHierarchy(m_Skeleton->GetRoot());

	ImGui::End();
}

void UiWindow::SetSkeleton(Skeleton* skeleton)
{
	m_Skeleton = skeleton;
}

void UiWindow::SetAnimations(std::vector<Animation>* animations)
{
	m_Animations = animations;
}
