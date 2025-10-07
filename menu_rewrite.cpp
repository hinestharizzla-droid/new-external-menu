#include "global.h"
#include <nlohmann/json.hpp>
#include <commdlg.h>
#include "Resource/Font/IconsFontAwesome6.h"

// Forward declarations for configuration functions
void ApplyPresetConfig(int presetIndex);
void SaveCurrentConfig(const std::string& configName);
void LoadCustomConfig(const std::string& configName);
void ResetToDefaults();
void ExportCurrentConfig();
void ImportConfigFromFile();

namespace {
	// Simplified Theme System
	enum class Theme {
		DARK = 0,
		BLUE = 1,
		PURPLE = 2,
		GREEN = 3,
		RED = 4
	};

	static int currentTheme = 0;

	struct ThemeColors {
		ImVec4 Background;
		ImVec4 Secondary;
		ImVec4 Accent;
		ImVec4 Text;
		ImVec4 TextDim;
		ImVec4 Border;
	};

	static const ThemeColors themes[] = {
		// DARK
		{
			ImVec4(0.08f, 0.08f, 0.08f, 1.0f),  // Background
			ImVec4(0.12f, 0.12f, 0.12f, 1.0f),  // Secondary
			ImVec4(0.20f, 0.60f, 1.00f, 1.0f),  // Accent
			ImVec4(1.00f, 1.00f, 1.00f, 1.0f),  // Text
			ImVec4(0.60f, 0.60f, 0.60f, 1.0f),  // TextDim
			ImVec4(0.25f, 0.25f, 0.25f, 1.0f)   // Border
		},
		// BLUE
		{
			ImVec4(0.05f, 0.10f, 0.15f, 1.0f),
			ImVec4(0.08f, 0.15f, 0.22f, 1.0f),
			ImVec4(0.10f, 0.60f, 1.00f, 1.0f),
			ImVec4(1.00f, 1.00f, 1.00f, 1.0f),
			ImVec4(0.60f, 0.70f, 0.80f, 1.0f),
			ImVec4(0.15f, 0.30f, 0.45f, 1.0f)
		},
		// PURPLE
		{
			ImVec4(0.10f, 0.05f, 0.15f, 1.0f),
			ImVec4(0.15f, 0.08f, 0.22f, 1.0f),
			ImVec4(0.60f, 0.20f, 1.00f, 1.0f),
			ImVec4(1.00f, 1.00f, 1.00f, 1.0f),
			ImVec4(0.70f, 0.60f, 0.80f, 1.0f),
			ImVec4(0.30f, 0.15f, 0.45f, 1.0f)
		},
		// GREEN
		{
			ImVec4(0.05f, 0.10f, 0.05f, 1.0f),
			ImVec4(0.08f, 0.15f, 0.08f, 1.0f),
			ImVec4(0.20f, 1.00f, 0.40f, 1.0f),
			ImVec4(1.00f, 1.00f, 1.00f, 1.0f),
			ImVec4(0.60f, 0.80f, 0.60f, 1.0f),
			ImVec4(0.15f, 0.30f, 0.15f, 1.0f)
		},
		// RED
		{
			ImVec4(0.15f, 0.05f, 0.05f, 1.0f),
			ImVec4(0.22f, 0.08f, 0.08f, 1.0f),
			ImVec4(1.00f, 0.20f, 0.20f, 1.0f),
			ImVec4(1.00f, 1.00f, 1.00f, 1.0f),
			ImVec4(0.80f, 0.60f, 0.60f, 1.0f),
			ImVec4(0.45f, 0.15f, 0.15f, 1.0f)
		}
	};

	inline const ThemeColors& GetColors() {
		return themes[currentTheme];
	}
	#define Colors GetColors()

	struct MenuPage {
		const char* name;
		const char* icon;
		int id;
	};

	constexpr MenuPage PAGES[] = {
		{ "Aimbot", ICON_FA_LOCATION_CROSSHAIRS, 0 },
		{ "ESP", ICON_FA_STREET_VIEW, 1 },
		{ "Settings", ICON_FA_GEARS, 2 },
		{ "Config", ICON_FA_FOLDER_OPEN, 3 },
		{ "Debug", ICON_FA_BUG, 4 }
	};

	int selectedPage = 0;
	static ImVec2 menuSize = { 800.0f, 600.0f };
	static float sidebarWidth = 180.0f;

	static bool menuOpen = true;

	void SetClickThrough(bool clickThrough) {
		LONG_PTR style = GetWindowLongPtr(Render::g_windowHandle, GWL_EXSTYLE);
		if (clickThrough) {
			style |= WS_EX_TRANSPARENT;
		} else {
			style &= ~WS_EX_TRANSPARENT;
		}
		SetWindowLongPtr(Render::g_windowHandle, GWL_EXSTYLE, style);
	}

	void HandleInput() {
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			menuOpen = !menuOpen;
			SetClickThrough(!menuOpen);
		}
	}

	void ApplyStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		
		// Sharp edges everywhere
		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
		
		// Padding and spacing
		style.WindowPadding = ImVec2(0.0f, 0.0f);
		style.FramePadding = ImVec2(8.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
		style.ScrollbarSize = 12.0f;
		style.GrabMinSize = 8.0f;
		
		// Border
		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		
		// Colors
		style.Colors[ImGuiCol_WindowBg] = Colors.Background;
		style.Colors[ImGuiCol_ChildBg] = Colors.Secondary;
		style.Colors[ImGuiCol_Border] = Colors.Border;
		style.Colors[ImGuiCol_FrameBg] = Colors.Secondary;
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(Colors.Accent.x * 0.3f, Colors.Accent.y * 0.3f, Colors.Accent.z * 0.3f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(Colors.Accent.x * 0.5f, Colors.Accent.y * 0.5f, Colors.Accent.z * 0.5f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = Colors.Secondary;
		style.Colors[ImGuiCol_TitleBgActive] = Colors.Secondary;
		style.Colors[ImGuiCol_Button] = Colors.Secondary;
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(Colors.Accent.x * 0.4f, Colors.Accent.y * 0.4f, Colors.Accent.z * 0.4f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = Colors.Accent;
		style.Colors[ImGuiCol_Header] = Colors.Accent;
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(Colors.Accent.x * 0.8f, Colors.Accent.y * 0.8f, Colors.Accent.z * 0.8f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = Colors.Accent;
		style.Colors[ImGuiCol_SliderGrab] = Colors.Accent;
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(Colors.Accent.x * 1.2f, Colors.Accent.y * 1.2f, Colors.Accent.z * 1.2f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = Colors.Secondary;
		style.Colors[ImGuiCol_ScrollbarGrab] = Colors.Accent;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(Colors.Accent.x * 0.8f, Colors.Accent.y * 0.8f, Colors.Accent.z * 0.8f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = Colors.Accent;
		style.Colors[ImGuiCol_CheckMark] = Colors.Accent;
		style.Colors[ImGuiCol_Tab] = Colors.Secondary;
		style.Colors[ImGuiCol_TabHovered] = ImVec4(Colors.Accent.x * 0.6f, Colors.Accent.y * 0.6f, Colors.Accent.z * 0.6f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = Colors.Accent;
		style.Colors[ImGuiCol_Text] = Colors.Text;
		style.Colors[ImGuiCol_TextDisabled] = Colors.TextDim;
	}

	// Render Functions
	void RenderAimbot();
	void RenderESP();
	void RenderSettings();
	void RenderConfig();
	void RenderDebug();
}

void Menu::RenderMenu() {
	HandleInput();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		Loop::RunLoop();

		if (menuOpen) {
			ApplyStyle();

			// Main window
			ImGui::SetNextWindowPos(ImVec2(Render::g_screenWidth / 2.0f - menuSize.x / 2.0f, Render::g_screenHeight / 2.0f - menuSize.y / 2.0f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(menuSize, ImGuiCond_FirstUseEver);
			ImGui::Begin("CODEX", &menuOpen, ImGuiWindowFlags_NoCollapse);
			{
				auto drawList = ImGui::GetWindowDrawList();
				auto windowPos = ImGui::GetWindowPos();
				auto windowSize = ImGui::GetWindowSize();

				// Draw accent line at top
				drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + 2), ImGui::GetColorU32(Colors.Accent));

				// Sidebar
				ImGui::SetCursorPos(ImVec2(0, 2));
				ImGui::BeginChild("##sidebar", ImVec2(sidebarWidth, 0), true, ImGuiWindowFlags_NoScrollbar);
				{
					// Title
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
					ImGui::PushStyleColor(ImGuiCol_Text, Colors.Accent);
					ImGui::SetCursorPosX((sidebarWidth - ImGui::CalcTextSize("CODEX").x) * 0.5f);
					ImGui::Text("CODEX");
					ImGui::PopStyleColor();
					
					ImGui::Spacing();
					drawList->AddLine(
						ImVec2(windowPos.x + 10, windowPos.y + ImGui::GetCursorPosY()),
						ImVec2(windowPos.x + sidebarWidth - 10, windowPos.y + ImGui::GetCursorPosY()),
						ImGui::GetColorU32(Colors.Border), 1.0f
					);
					ImGui::Spacing();
					ImGui::Spacing();

					// Page buttons
					for (const auto& page : PAGES) {
						bool isSelected = (selectedPage == page.id);
						
						if (isSelected) {
							ImGui::PushStyleColor(ImGuiCol_Button, Colors.Accent);
							ImGui::PushStyleColor(ImGuiCol_Text, Colors.Background);
						}
						
						std::string label = std::string(page.icon) + "  " + page.name;
						if (ImGui::Button(label.c_str(), ImVec2(sidebarWidth - 16, 30))) {
							selectedPage = page.id;
						}
						
						if (isSelected) {
							ImGui::PopStyleColor(2);
						}
					}
					
					// Footer info
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 60);
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::PushStyleColor(ImGuiCol_Text, Colors.TextDim);
					ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
					ImGui::Text("v2.1.0");
					ImGui::PopStyleColor();
				}
				ImGui::EndChild();

				// Main content area
				ImGui::SameLine();
				ImGui::BeginChild("##content", ImVec2(0, 0), false);
				{
					ImGui::SetCursorPos(ImVec2(15, 15));
					ImGui::BeginChild("##content_inner", ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), false);
					{
						// Render selected page
						switch (selectedPage) {
							case 0: RenderAimbot(); break;
							case 1: RenderESP(); break;
							case 2: RenderSettings(); break;
							case 3: RenderConfig(); break;
							case 4: RenderDebug(); break;
						}
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}
	ImGui::EndFrame();
	Render::PresentImGuiFrame();
}

namespace {
	// Simple tab system
	void BeginTabs(const char* id, int* current, const char** tabs, int tabCount) {
		ImGui::PushID(id);
		for (int i = 0; i < tabCount; i++) {
			if (i > 0) ImGui::SameLine();
			
			if (*current == i) {
				ImGui::PushStyleColor(ImGuiCol_Button, Colors.Accent);
				ImGui::PushStyleColor(ImGuiCol_Text, Colors.Background);
			}
			
			if (ImGui::Button(tabs[i], ImVec2(120, 25))) {
				*current = i;
			}
			
			if (*current == i) {
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::PopID();
		ImGui::Separator();
		ImGui::Spacing();
	}

	void RenderAimbot() {
		ImGui::PushStyleColor(ImGuiCol_Text, Colors.Accent);
		ImGui::Text(ICON_FA_LOCATION_CROSSHAIRS " AIMBOT");
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Spacing();

		static int tab = 0;
		const char* tabs[] = { "Settings", "Advanced", "Visual" };
		BeginTabs("##aimtabs", &tab, tabs, 3);

		ImAdd::Checkbox("Enable Aimbot", &Config::Aim::enable);
		
		if (!Config::Aim::enable) {
			ImGui::PushStyleColor(ImGuiCol_Text, Colors.TextDim);
			ImGui::Text("Enable aimbot to access settings");
			ImGui::PopStyleColor();
			return;
		}

		if (tab == 0) {
			ImGui::Spacing();
			ImAdd::SliderFloat("Smoothing X", &Config::Aim::smoothingX, 1.f, 100.f);
			ImAdd::SliderFloat("Smoothing Y", &Config::Aim::smoothingY, 1.f, 100.f);
			ImGui::Spacing();
			ImAdd::Combo("Target Bone", &Config::Aim::aimbone, "Head\0Neck\0Chest\0Pelvis\0Smart\0");
			ImGui::Spacing();
			ImAdd::ToggleButton("Visible Check", &Config::Aim::visibleCheck);
			ImAdd::Hotkey("Aim Key", &Config::Aim::aimkey);
		}
		else if (tab == 1) {
			ImGui::Spacing();
			ImAdd::SliderFloat("FOV", &Config::Aim::FOV, 1.f, 600.f);
			ImAdd::SliderFloat("Max Distance", &Config::Aim::maxAimbotDistance, 1.f, 300.f);
			ImGui::Spacing();
			ImAdd::ToggleButton("Target Lock", &Config::Aim::targetLock);
			if (Config::Aim::targetLock) {
				ImGui::Indent();
				ImAdd::SliderFloat("Lock Timeout", &Config::Aim::targetLockTimeout, 0.5f, 10.0f);
				ImAdd::SliderFloat("Lock Range", &Config::Aim::targetLockRange, 1.0f, 3.0f);
				ImGui::Unindent();
			}
		}
		else if (tab == 2) {
			ImGui::Spacing();
			ImAdd::ToggleButton("Show FOV Circle", &Config::Aim::showFOV);
			ImAdd::ToggleButton("Target Line", &Config::Aim::targetLine);
			if (Config::Aim::targetLine) {
				ImGui::Indent();
				ImAdd::ColorEdit4("Line Color", (float*)&Config::Aim::targetLineColor);
				ImGui::Unindent();
			}
		}
	}

	void RenderESP() {
		ImGui::PushStyleColor(ImGuiCol_Text, Colors.Accent);
		ImGui::Text(ICON_FA_STREET_VIEW " ESP");
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Spacing();

		static int tab = 0;
		const char* tabs[] = { "Features", "Colors" };
		BeginTabs("##esptabs", &tab, tabs, 2);

		ImAdd::ToggleButton("Enable ESP", &Config::ESP::enable);
		
		if (!Config::ESP::enable) {
			ImGui::PushStyleColor(ImGuiCol_Text, Colors.TextDim);
			ImGui::Text("Enable ESP to access settings");
			ImGui::PopStyleColor();
			return;
		}

		if (tab == 0) {
			ImGui::Spacing();
			ImAdd::ToggleButton("Boxes", &Config::ESP::box);
			if (Config::ESP::box) {
				ImGui::Indent();
				ImAdd::Combo("Box Type", &Config::ESP::boxType, "Normal\0Cornered\0");
				ImAdd::ToggleButton("Filled", &Config::ESP::boxFilled);
				ImGui::Unindent();
			}
			ImGui::Spacing();
			ImAdd::ToggleButton("Names", &Config::ESP::nickname);
			ImAdd::ToggleButton("Health Bar", &Config::ESP::health);
			if (Config::ESP::health) {
				ImGui::Indent();
				ImAdd::Combo("Position", &Config::ESP::healthBarPos, "Left\0Right\0Top\0Bottom\0");
				ImGui::Unindent();
			}
			ImGui::Spacing();
			ImAdd::ToggleButton("Distance", &Config::ESP::distance);
			ImAdd::ToggleButton("Snaplines", &Config::ESP::snaplines);
			ImAdd::ToggleButton("Skeleton", &Config::ESP::skeleton);
			ImAdd::ToggleButton("Indicators", &Config::ESP::indicator);
			ImGui::Spacing();
			ImAdd::SliderInt("Max Distance", &Config::ESP::maxESPDistance, 1, 300);
		}
		else if (tab == 1) {
			ImGui::Spacing();
			ImAdd::ToggleButton("Rainbow Mode", &VisualSettings::bRainbowAll);
			ImGui::Separator();
			ImGui::Spacing();
			
			if (Config::ESP::box) {
				ImGui::Text("Box Colors:");
				ImAdd::ColorEdit4("Visible", (float*)&Config::ESP::boxVisibleOutlineColor);
				ImAdd::ColorEdit4("Hidden", (float*)&Config::ESP::boxInvisibleOutlineColor);
				if (Config::ESP::boxFilled) {
					ImAdd::ColorEdit4("Fill Visible", (float*)&Config::ESP::boxVisibleFilledColor);
					ImAdd::ColorEdit4("Fill Hidden", (float*)&Config::ESP::boxInvisibleFilledColor);
				}
				ImGui::Spacing();
			}
			
			if (Config::ESP::nickname) {
				ImGui::Text("Name Colors:");
				ImAdd::ColorEdit4("Visible##name", (float*)&Config::ESP::nicknameVisibleColor);
				ImAdd::ColorEdit4("Hidden##name", (float*)&Config::ESP::nicknameInvisibleColor);
				ImGui::Spacing();
			}
			
			if (Config::ESP::snaplines) {
				ImGui::Text("Snapline Colors:");
				ImAdd::ColorEdit4("Visible##snap", (float*)&Config::ESP::snaplinesVisibleColor);
				ImAdd::ColorEdit4("Hidden##snap", (float*)&Config::ESP::snaplinesInvisibleColor);
				ImGui::Spacing();
			}
			
			if (Config::ESP::skeleton) {
				ImGui::Text("Skeleton Colors:");
				ImAdd::ColorEdit4("Visible##skel", (float*)&Config::ESP::skeletonVisibleColor);
				ImAdd::ColorEdit4("Hidden##skel", (float*)&Config::ESP::skeletonInvisibleColor);
			}
		}
	}

	void RenderSettings() {
		ImGui::PushStyleColor(ImGuiCol_Text, Colors.Accent);
		ImGui::Text(ICON_FA_GEARS " SETTINGS");
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Theme:");
		const char* themeNames[] = { "Dark", "Blue", "Purple", "Green", "Red" };
		if (ImGui::Combo("##theme", &currentTheme, themeNames, 5)) {
			ApplyStyle();
		}
		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::Text("Crosshair:");
		ImAdd::ToggleButton("Enable Custom Crosshair", &Config::Settings::enableCrosshair);
		if (Config::Settings::enableCrosshair) {
			ImGui::Indent();
			ImAdd::SliderFloat("Size", &Config::Settings::crosshairSize, 5.0f, 30.0f);
			ImAdd::SliderFloat("Thickness", &Config::Settings::crosshairThickness, 1.0f, 5.0f);
			ImAdd::SliderFloat("Gap", &Config::Settings::crosshairGap, 0.0f, 15.0f);
			ImAdd::ToggleButton("Center Dot", &Config::Settings::crosshairDot);
			ImGui::Unindent();
		}
		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::Text("Privacy:");
		ImAdd::ToggleButton("Stream Proof", &Config::Settings::streamProof);
		ImGui::PushStyleColor(ImGuiCol_Text, Colors.TextDim);
		ImGui::TextWrapped(Config::Settings::streamProof ? "Screenshots blocked" : "Screenshots allowed");
		ImGui::PopStyleColor();
		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		
		if (ImGui::Button("Reset All Settings", ImVec2(150, 25))) {
			ResetToDefaults();
		}
	}

	void RenderConfig() {
		ImGui::PushStyleColor(ImGuiCol_Text, Colors.Accent);
		ImGui::Text(ICON_FA_FOLDER_OPEN " CONFIG");
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Spacing();

		static int selectedPreset = 0;
		static char configName[64] = "";
		static std::vector<std::string> customConfigs = { "Default", "MyConfig", "Tournament", "Streaming" };
		static int selectedConfig = 0;

		ImGui::Text("Presets:");
		const char* presets[] = { "Legit", "Semi-Legit", "Rage", "HvH" };
		ImGui::Combo("##preset", &selectedPreset, presets, 4);
		if (ImGui::Button("Load Preset", ImVec2(120, 25))) {
			ApplyPresetConfig(selectedPreset);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Custom Configs:");
		if (ImGui::BeginListBox("##configs", ImVec2(-1, 150))) {
			for (int i = 0; i < customConfigs.size(); i++) {
				bool isSelected = (selectedConfig == i);
				if (ImGui::Selectable(customConfigs[i].c_str(), isSelected)) {
					selectedConfig = i;
				}
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Load", ImVec2(80, 25))) {
			if (selectedConfig < customConfigs.size()) {
				LoadCustomConfig(customConfigs[selectedConfig]);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Save", ImVec2(80, 25))) {
			ImGui::OpenPopup("Save Config");
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete", ImVec2(80, 25))) {
			if (selectedConfig > 0 && selectedConfig < customConfigs.size()) {
				customConfigs.erase(customConfigs.begin() + selectedConfig);
			}
		}

		// Save popup
		if (ImGui::BeginPopupModal("Save Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Config Name:");
			ImGui::InputText("##name", configName, sizeof(configName));
			if (ImGui::Button("Save", ImVec2(100, 0))) {
				if (strlen(configName) > 0) {
					SaveCurrentConfig(configName);
					customConfigs.push_back(configName);
					memset(configName, 0, sizeof(configName));
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(100, 0))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Export", ImVec2(100, 25))) {
			ExportCurrentConfig();
		}
		ImGui::SameLine();
		if (ImGui::Button("Import", ImVec2(100, 25))) {
			ImportConfigFromFile();
		}
	}

	void RenderDebug() {
		ImGui::PushStyleColor(ImGuiCol_Text, Colors.Accent);
		ImGui::Text(ICON_FA_BUG " DEBUG");
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Spacing();

		static int tab = 0;
		const char* tabs[] = { "Players", "System" };
		BeginTabs("##debugtabs", &tab, tabs, 2);

		if (tab == 0) {
			// Player list
			std::vector<PlayerData> players = Cache::playerData;
			ImGui::Text("Players: %d", (int)players.size());
			ImGui::Separator();
			
			if (ImGui::BeginTable("##players", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 300))) {
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 100);
				ImGui::TableSetupColumn("HP", ImGuiTableColumnFlags_WidthFixed, 40);
				ImGui::TableSetupColumn("Dist", ImGuiTableColumnFlags_WidthFixed, 50);
				ImGui::TableSetupColumn("Vis", ImGuiTableColumnFlags_WidthFixed, 40);
				ImGui::TableHeadersRow();

				for (const auto& p : players) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%s", p.playerName.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%d", p.playerHealth);
					ImGui::TableNextColumn();
					ImGui::Text("%.1f", p.distance);
					ImGui::TableNextColumn();
					ImGui::Text(p.isVisible ? "Yes" : "No");
				}
				ImGui::EndTable();
			}
		}
		else if (tab == 1) {
			// System info
			ImGui::Text("Performance:");
			ImGui::BulletText("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::BulletText("Frame Time: %.2fms", 1000.0f / ImGui::GetIO().Framerate);
			
			ImGui::Spacing();
			ImGui::Text("Cache:");
			ImGui::BulletText("Bone Base: 0x%llX", Cache::boneBase);
			ImGui::BulletText("Client Info: 0x%llX", Cache::clientInfo);
			ImGui::BulletText("Local Player: 0x%llX", Cache::localPlayer);
			
			ImGui::Spacing();
			ImGui::Text("Position:");
			ImGui::BulletText("X: %.1f", Cache::localPlayerPosition.X);
			ImGui::BulletText("Y: %.1f", Cache::localPlayerPosition.Y);
			ImGui::BulletText("Z: %.1f", Cache::localPlayerPosition.Z);
			
			ImGui::Spacing();
			if (ImGui::Button("Refresh Cache", ImVec2(-1, 25))) {
				Logger.log("Manual cache refresh requested");
			}
		}
	}
}

// Config management functions (keep same implementation)
void ApplyPresetConfig(int presetIndex) {
	switch (presetIndex) {
		case 0: // Legit
			Config::Aim::enable = true;
			Config::Aim::smoothingX = 15.0f;
			Config::Aim::smoothingY = 15.0f;
			Config::Aim::FOV = 45.0f;
			Config::Aim::visibleCheck = true;
			Config::ESP::enable = true;
			Config::ESP::box = true;
			Config::ESP::maxESPDistance = 150;
			break;
		case 1: // Semi-Legit
			Config::Aim::enable = true;
			Config::Aim::smoothingX = 8.0f;
			Config::Aim::smoothingY = 8.0f;
			Config::Aim::FOV = 75.0f;
			Config::ESP::enable = true;
			Config::ESP::maxESPDistance = 250;
			break;
		case 2: // Rage
			Config::Aim::enable = true;
			Config::Aim::smoothingX = 2.0f;
			Config::Aim::smoothingY = 2.0f;
			Config::Aim::FOV = 180.0f;
			Config::Aim::visibleCheck = false;
			Config::ESP::enable = true;
			Config::ESP::boxFilled = true;
			Config::ESP::maxESPDistance = 300;
			break;
		case 3: // HvH
			Config::Aim::enable = true;
			Config::Aim::smoothingX = 1.0f;
			Config::Aim::smoothingY = 1.0f;
			Config::Aim::FOV = 360.0f;
			Config::ESP::enable = true;
			Config::ESP::maxESPDistance = 300;
			break;
	}
	Logger.log("Applied preset {}", presetIndex);
}

void SaveCurrentConfig(const std::string& configName) {
	CreateDirectoryA("configs", NULL);
	std::string filename = "configs\\" + configName + ".json";
	
	try {
		nlohmann::json config;
		config["aim"]["enable"] = Config::Aim::enable;
		config["aim"]["smoothingX"] = Config::Aim::smoothingX;
		config["aim"]["smoothingY"] = Config::Aim::smoothingY;
		config["aim"]["FOV"] = Config::Aim::FOV;
		config["esp"]["enable"] = Config::ESP::enable;
		config["esp"]["box"] = Config::ESP::box;
		config["esp"]["maxESPDistance"] = Config::ESP::maxESPDistance;
		
		std::ofstream file(filename);
		file << config.dump(4);
		file.close();
		Logger.log("Config saved: {}", filename);
	} catch (const std::exception& e) {
		Logger.log(LogLevel::ERROR, "Save failed: {}", e.what());
	}
}

void LoadCustomConfig(const std::string& configName) {
	std::string filename = "configs\\" + configName + ".json";
	try {
		std::ifstream file(filename);
		if (!file.is_open()) return;
		
		nlohmann::json config;
		file >> config;
		file.close();
		
		if (config.contains("aim")) {
			Config::Aim::enable = config["aim"]["enable"];
			Config::Aim::smoothingX = config["aim"]["smoothingX"];
			Config::Aim::smoothingY = config["aim"]["smoothingY"];
			Config::Aim::FOV = config["aim"]["FOV"];
		}
		if (config.contains("esp")) {
			Config::ESP::enable = config["esp"]["enable"];
			Config::ESP::box = config["esp"]["box"];
			Config::ESP::maxESPDistance = config["esp"]["maxESPDistance"];
		}
		Logger.log("Config loaded: {}", filename);
	} catch (const std::exception& e) {
		Logger.log(LogLevel::ERROR, "Load failed: {}", e.what());
	}
}

void ResetToDefaults() {
	Config::Aim::enable = false;
	Config::Aim::smoothingX = 5.0f;
	Config::Aim::smoothingY = 5.0f;
	Config::Aim::FOV = 60.0f;
	Config::ESP::enable = false;
	Config::ESP::box = false;
	Config::Settings::streamProof = false;
	Logger.log("Settings reset to defaults");
}

void ExportCurrentConfig() {
	char filename[MAX_PATH] = {0};
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "JSON\0*.json\0";
	ofn.lpstrDefExt = "json";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	
	if (GetSaveFileNameA(&ofn)) {
		SaveCurrentConfig(filename);
	}
}

void ImportConfigFromFile() {
	char filename[MAX_PATH] = {0};
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "JSON\0*.json\0";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (GetOpenFileNameA(&ofn)) {
		LoadCustomConfig(filename);
	}
}
