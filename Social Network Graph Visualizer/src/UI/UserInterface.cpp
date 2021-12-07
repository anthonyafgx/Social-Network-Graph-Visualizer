#include "UserInterface.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_sdlrenderer.h"
#include <SDL.h>
#include <stdio.h>
#include "../GraphicsEngine.h"
#include "../Actors/Graph.h"
#include <string>

//#define ADITIONAL_WINDOW		//< Tells if another window will be made for the UI.

UserInterface::UserInterface(GraphicsEngine* graphics) :
	mGraphics(graphics),
	mWindow(nullptr),
	mRenderer(nullptr)
{
	Init();
}

UserInterface::~UserInterface()
{
	Shutdown();
}

int UserInterface::Init()
{
#ifdef ADITIONAL_WINDOW
	// Setup Window
	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	mWindow = SDL_CreateWindow(
		"Social Network Graph Controller",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		340,
		720,
		windowFlags
	);

	if (!mWindow)
	{
		SDL_Log("Unable to create UI Window: %s", SDL_GetError());
		return false;
	}

	// Setup renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
	);

	if (!mRenderer)
	{
		SDL_Log("Unable to create UI Renderer: %s", SDL_GetError());
		return false;
	}
#else	// Single Window Setup
	mWindow = mGraphics->GetWindow();
	mRenderer = mGraphics->GetRenderer();
#endif	// ADITIONAL_WINDOW

	// Setup IMGUI Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Enable Keyboard
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup IMGUI Window/Renderer
	ImGui_ImplSDL2_InitForSDLRenderer(mWindow);
	ImGui_ImplSDLRenderer_Init(mRenderer);

	return 0;
}

void UserInterface::Update()
{
	// Setup New Frame
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(mWindow);
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	// WINDOW EDITING CODE
	ImGui::Begin("Control de Grafo");
	
	if (ImGui::TreeNode("Insertar Nodo"))
	{
		ImGui::PushItemWidth(32);
		static int id = 0; ImGui::InputInt("ID", &id, 0);
		ImGui::PopItemWidth();
		// clamp id
		id = (id > 255) ? 255 : id;
		id = (id < 0) ? 0 : id;

		ImGui::PushItemWidth(128);
		static char name[16]; ImGui::InputText("Nombre", name, 16);
		ImGui::PopItemWidth();

		if (ImGui::Button("Insertar"))
		{
			mGraphics->GetGraph()->InsertNode(id, name);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Remover Nodo"))
	{
		ImGui::PushItemWidth(32);
		static int id = 0; ImGui::InputInt("ID", &id, 0);
		ImGui::PopItemWidth();

		if (ImGui::Button("Remover"))
		{
			mGraphics->GetGraph()->RemoveNode(id);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Verificar Nodo"))
	{
		ImGui::PushItemWidth(32);
		static int id = 0; ImGui::InputInt("ID", &id, 0);
		ImGui::PopItemWidth();

		if (ImGui::Button("Verificar"))
		{
			mGraphics->GetGraph()->VerifyNode(id);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Relaciones"))
	{
		ImGui::PushItemWidth(32);
		static int idA = 0; ImGui::InputInt("ID A", &idA, 0);
		ImGui::SameLine();
		static int idB = 0; ImGui::InputInt("ID B", &idB, 0);
		ImGui::PopItemWidth();

		if (ImGui::Button("Crear"))
		{
			mGraphics->GetGraph()->AddRelation(idA, idB);
		}
		ImGui::SameLine();
		if (ImGui::Button("Remover"))
		{
			mGraphics->GetGraph()->RemoveRelation(idA, idB);
		}
		ImGui::SameLine();
		if (ImGui::Button("Verificar"))
		{
			mGraphics->GetGraph()->HighlightRelation(idA, idB);
		}

		ImGui::Separator();

		ImGui::Text("Sugerir Relaciones");
		ImGui::PushItemWidth(32);
		static int idS = 0; ImGui::InputInt("ID", &idS, 0);
		ImGui::PopItemWidth();
		if (ImGui::Button("Sugerir"))
		{
			mGraphics->GetGraph()->HighlightSuggestions(idS);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Encontrar Camino"))
	{
		ImGui::PushItemWidth(32);
		static int idA = 0; ImGui::InputInt("ID A", &idA, 0);
		ImGui::SameLine();
		static int idB = 0; ImGui::InputInt("ID B", &idB, 0);
		ImGui::PopItemWidth();

		if (ImGui::Button("Mostrar"))
		{
			mGraphics->GetGraph()->HighlightPath(idA, idB);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Botones Peligrosos"))
	{
		if (ImGui::Button("Remover todos los nodos"))
		{
			mGraphics->GetGraph()->RemoveAllNodes();
		}

		ImGui::TreePop();
	}

	ImGui::End();	// Control de Grafo


	ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::SetNextWindowSize(ImVec2(900, 32));
	ImGui::SetNextWindowBgAlpha(0.75f);
	ImGui::Begin("Information Overlay", nullptr, overlayFlags);
	
	ImGui::Text("Ultimo mensaje:	");
	ImGui::SameLine();
	ImGui::Text(mGraphics->GetGraph()->GetFeedback());

	ImGui::End(); // Information Overlay

	// Rendering Code
	//ImGui::Render();
	//SDL_SetRenderDrawColor(mRenderer, 240, 240, 245, 255);
	//SDL_RenderClear(mRenderer);
	//ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	//SDL_RenderPresent(mRenderer);
}

void UserInterface::Draw()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::Shutdown()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
}