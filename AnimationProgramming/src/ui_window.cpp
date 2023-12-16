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
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	GLFWwindow* window = glfwCreateWindow(1400, 800, "UI", NULL, NULL);

	if (window == NULL)
		return;

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

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// glfwGetWindowPos(window, &Globals::gWindowX, &Globals::gWindowY);
		// glfwGetWindowSize(window, &Globals::gWindowWidth, &Globals::gWindowHeight);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("hello");
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

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
	m_Thread = std::thread(&UiWindow::Main, this);
}

std::thread& UiWindow::GetThread()
{
	return m_Thread;
}
