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

	GLFWwindow* window = glfwCreateWindow(1400, 800, "UI", NULL, NULL);

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

	while (!glfwWindowShouldClose(window) && !m_CloseWindow)
	{
		glfwPollEvents();

		glfwGetWindowPos(window, &m_WindowX, &m_WindowY);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		DrawSkeletonHierarchy();

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




void UiWindow::DrawBone(const Bone& bone) const
{
	for (const Bone* const bone : bone.GetChildren())
	{
		if (ImGui::TreeNode(bone->GetName().c_str()))
		{
			DrawBone(*bone);
			ImGui::TreePop();
		}
	}
}

void UiWindow::DrawSkeletonHierarchy() const
{
	if (!m_Skeleton)
		return;

	ImGui::Begin("Skeleton");

	DrawBone(m_Skeleton->GetRoot());

	ImGui::End();
}

void UiWindow::SetSkeleton(Skeleton* skeleton)
{
	m_Skeleton = skeleton;
}
