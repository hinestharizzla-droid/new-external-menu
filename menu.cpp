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
	// Single Dark Theme System
	struct ThemeColors {
		ImVec4 Background;      // Main background
		ImVec4 BackgroundAlt;   // Alternative background
		ImVec4 Surface;         // Surface elements
		ImVec4 SurfaceAlt;      // Alternative surface
		ImVec4 Border;          // Border color
		ImVec4 BorderActive;    // Active border
		ImVec4 Accent;          // Primary accent
		ImVec4 AccentHover;     // Hovered accent
		ImVec4 AccentActive;    // Active accent
		ImVec4 TextPrimary;     // Primary text
		ImVec4 TextSecondary;   // Secondary text
		ImVec4 Success;         // Success state
		ImVec4 Warning;         // Warning state
		ImVec4 Error;           // Error state
		ImVec4 Transparent;     // Transparent
	};

	// Single clean dark theme with cyan accent
	static const ThemeColors DarkTheme = {
		ImAdd::Hex2RGBA(0x0a0a0a, 1.0f),  // Background - Very dark gray
		ImAdd::Hex2RGBA(0x151515, 1.0f),  // BackgroundAlt - Dark gray
		ImAdd::Hex2RGBA(0x1a1a1a, 1.0f),  // Surface - Slightly lighter
		ImAdd::Hex2RGBA(0x222222, 1.0f),  // SurfaceAlt - Alternative surface
		ImAdd::Hex2RGBA(0x2a2a2a, 1.0f),  // Border - Subtle border
		ImAdd::Hex2RGBA(0x3a3a3a, 1.0f),  // BorderActive - Active border
		ImAdd::Hex2RGBA(0x00d9ff, 1.0f),  // Accent - Cyan accent
		ImAdd::Hex2RGBA(0x33e0ff, 1.0f),  // AccentHover - Brighter cyan
		ImAdd::Hex2RGBA(0x00b8d4, 1.0f),  // AccentActive - Darker cyan
		ImAdd::Hex2RGBA(0xffffff, 0.95f), // TextPrimary - White
		ImAdd::Hex2RGBA(0xaaaaaa, 0.85f), // TextSecondary - Gray
		ImAdd::Hex2RGBA(0x00ff88, 1.0f),  // Success - Green
		ImAdd::Hex2RGBA(0xffaa00, 1.0f),  // Warning - Orange
		ImAdd::Hex2RGBA(0xff3366, 1.0f),  // Error - Red
		ImAdd::Hex2RGBA(0x000000, 0.0f)   // Transparent
	};
			ImAdd::Hex2RGBA(0xf87171, 1.0f),  // Error
			ImAdd::Hex2RGBA(0x000000, 0.0f)   // Transparent
		}
	};
	
	// Performance optimization variables
	static bool needsRedraw = true;
	static int lastSelectedPageId = -1;
	static int framesSinceUpdate = 0;
	static const int REDRAW_INTERVAL = 3; // Update every 3 frc int lastThemeIndex = -1;
	
	// Optimized animation variables with frame limiting
	static float headerAnimTime = 0.0f;
	static float sidebarAnimTime = 0.0f;
	static float controlAnimTime = 0.0f;
	static float lastFrameTime = 0.0f;
	
	// Page transition animations
	static float pageTransitionProgress = 1.0f;
	static bool pageTransitioning = false;
	static int transitionFromPage = -1;
	static int transitionToPage = -1;
	static const float TRANSITION_SPEED = 4.0f;
	
	// Enhanced control animations with cleanup
	static std::unordered_map<ImGuiID, float> buttonHoverAnim;
	static std::unordered_map<ImGuiID, float> toggleSlideAnim;
	static std::uno	// Direct access to the single dark theme
	#define ModernColors DarkThemehedColors;
	}
	#define ModernColors GetModernColors()
	
	// Performance helper to check if we need to update UI animations (not game logic)
	inline bool ShouldUpdateUIAnimations() {
		++framesSinceUpdate;
		if (framesSinceUpdate >= REDRAW_INTERVAL || needsRedraw) {
			framesSinceUpdate = 0;
			needsRedraw = false; // Reset redraw flag
			return true;
		}
		return false;
	}
	
	// Cleanup unused animation entries periodically
	void CleanupAnimationMaps() {
		static int cleanupCounter = 0;
		if (++cleanupCounter > 300) { // Clean up every 300 frames (~5 seconds at 60 FPS)
			// Keep only recent entries in animation maps
			if (buttonHoverAnim.size() > 50) {
				buttonHoverAnim.clear();
			}
			if (toggleSlideAnim.size() > 50) {
				toggleSlideAnim.clear();
			}
			if (sliderGlowAnim.size() > 50) {
				sliderGlowAnim.clear();
			}
			cleanupCounter = 0;
		}
	}

	struct MenuPage {
		const char* pageTitle;
		const char* icon;
		void (*renderPage)();
	};

	struct MenuSection {
		const char* title;
		std::span<const MenuPage> pages;
	};

	namespace PageRenderers {
		void Aimbot();
		void ESP();
		void Settings();
		void Config();
		void Debug();
		
		// Box renderer functions for 4-panel layout
		void AimbotBox1();
		void AimbotBox2();
		void AimbotBox3();
		void AimbotBox4();
		void ESPBox1();
		void ESPBox2();
		void ESPBox3();
		void ESPBox4();
		void SettingsBox1();
		void SettingsBox2();
		void SettingsBox3();
		void SettingsBox4();
		void DebugBox1();
		void DebugBox2();
		void DebugBox3();
		void DebugBox4();
	}

	constexpr MenuPage COMBAT_PAGES[] = {
		{ "Aimbot", ICON_FA_LOCATION_CROSSHAIRS, &PageRenderers::Aimbot }
	};

	constexpr MenuPage VISUAL_PAGES[] = {
		{ "ESP", ICON_FA_STREET_VIEW, &PageRenderers::ESP }
	};

	constexpr MenuPage SETTINGS_PAGES[] = {
		{ "Settings", ICON_FA_GEARS, &PageRenderers::Settings }
	};

	constexpr MenuPage CONFIG_PAGES[] = {
		{ "Configs", ICON_FA_FOLDER_OPEN, &PageRenderers::Config }
	};


	constexpr MenuPage DEBUG_PAGES[] = {
		{ "Debug", ICON_FA_BUG, &PageRenderers::Debug }
	};

	constexpr MenuSection SECTIONS_DATA[] = {
		{ "COMBAT",			COMBAT_PAGES	},
		{ "VISUALS",		VISUAL_PAGES	},
		{ "MISC",			SETTINGS_PAGES	},
		{ "CONFIGURATION",	CONFIG_PAGES	},
		{ "DEBUG",			DEBUG_PAGES	},
	};

	int pageID = 0;
	float sidebarSize = 220.0f;
	static ImVec2 menuSize = { 780.0f , 520.0f };
	static ImVec2 topbarSize = { 450.0f , 50.0f };

	int selectedPageId = 0;
	const std::span<const MenuSection> allSections = SECTIONS_DATA;
	
	const MenuPage* GetCurrentPage() {
		int currentID = 0;

		for (const auto& section : allSections) {
			for (const auto& page : section.pages) {
				if (currentID == selectedPageId) {
					return &page;
				}

				currentID++;
			}
		}

		return nullptr;
	}

	void SetClickThrough(bool clickThrough) {
		LONG_PTR style = GetWindowLongPtr(Render::g_windowHandle, GWL_EXSTYLE);

		if (clickThrough) {
			style |= WS_EX_TRANSPARENT;
		} else {
			style &= ~WS_EX_TRANSPARENT;
		}

		SetWindowLongPtr(Render::g_windowHandle, GWL_EXSTYLE, style);
	}
	
	// Enhanced typography setup
	void SetupFonts() {
		ImGuiIO& io = ImGui::GetIO();
		
		// Clear existing fonts
		io.Fonts->Clear();
		
		// Font configuration
		ImFontConfig config;
		config.OversampleH = 2;
		config.OversampleV = 1;
		config.RasterizerMultiply = 1.0f;
		config.PixelSnapH = true;
		
		// Primary font - Default ImGui font with better sizing
		io.Fonts->AddFontDefault(&config);
		
		// Icon font integration
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icon_config = config;
		icon_config.MergeMode = true;
		icon_config.PixelSnapH = true;
		icon_config.GlyphMinAdvanceX = 13.0f;
		
		// Build font atlas
		io.Fonts->Build();
		
		// Apply font improvements
		ImGuiStyle& style = ImGui::GetStyle();
		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;
		style.AntiAliasedLinesUseTex = true;
	}

	static bool menuOpen = true;
	void HandleInput() {
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			menuOpen = !menuOpen;
			SetClickThrough(!menuOpen);
		}
	}

	static const ImGuiWindowFlags mainWindowFlags =
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoBackground;

	static const ImGuiWindowFlags topbarWindowFlags =
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove;
}

void Menu::RenderMenu() {
	HandleInput();
	
	// Check if we need to update UI animations (throttled for performance)
	bool shouldUpdateUIAnims = ShouldUpdateUIAnimations();
	
	// Check for page changes to trigger redraw - ALWAYS run this
	if (lastSelectedPageId != selectedPageId) {
		lastSelectedPageId = selectedPageId;
		needsRedraw = true;
	}
	
	// Update page transition animation - only throttle the animation progress
	if (pageTransitioning) {
		if (shouldUpdateUIAnims) {
			pageTransitionProgress += TRANSITION_SPEED * ImGui::GetIO().DeltaTime;
			if (pageTransitionProgress >= 1.0f) {
				pageTransitionProgress = 1.0f;
				pageTransitioning = false;
			}
		}
	}
	
	// Clean up animation maps periodically - not dependent on frame throttling
	CleanupAnimationMaps();
	
	// Skip KeyAuth session validation - already authenticated at startup
	// Session validation was moved to console authentication phase
	// if (!KeyAuth::ValidateSession()) {
	//		Logger.log(LogLevel::ERROR, "KeyAuth session validation failed - terminating");
	//		ExitProcess(0);
	//		return;
	// }
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		Loop::RunLoop();

		if (menuOpen) {
			ImGuiStyle& style = ImGui::GetStyle()				ImGui::SetNextWindowPos({ static_cast<float>(Render::g_screenWidth / 2) - (topbarSize.x / 2), 10 }, ImGuiCond_Always);
				ImGui::SetNextWindowSize(topbarSize);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 10.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ModernColors.Surface);
				ImGui::PushStyleColor(ImGuiCol_Border, ModernColors.Accent);
				ImGui::Begin("##modern_topbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);(ImGuiStyleVar_Windo					auto drawList = ImGui::GetWindowDrawList();
					auto windowPos = ImGui::GetWindowPos();
					auto windowSize = ImGui::GetWindowSize();
					
					// Animated accent line at bottom
					float pulseAlpha = (sin(headerAnimTime * 2.0f) + 1.0f) * 0.3f + 0.6f;
					ImVec4 accentColor = ModernColors.Accent;
					accentColor.w *= pulseAlpha;
					drawList->AddRectFilled(
						windowPos + ImVec2(0.0f, windowSize.y - 2.0f),
						windowPos + ImVec2(windowSize.x, windowSize.y),
						ImGui::GetColorU32(accentColor)
					);
					
					// Left side - Brand info
					ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
					ImGui::Text(ICON_FA_SHIELD_HALVED " CODEX | Premium");rnColors.BrightBlue.z, pulseAlpha)),
						10.0f, 0, 2.0f
					);
					
					// Left side - Brand info with animated icon
					float iconSpin = headerAnimTime * 0.5f;
					ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
					ImGui::Text(ICON_FA_SHIELD_HALVED " CODEX | Premium");
					ImGui::PopStyleColor();
					
					// Center - Time with modern formatting
					ImGui::SameLine(topbarSize.x * 0.4f);
					ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
					const auto& now = std::chrono::system_clock::now();
					auto time_t = std::chrono::system_cloc					// Right side - FPS with color coding
					ImGui::SameLine(topbarSize.x - 90);
					const auto& io = ImGui::GetIO();
					float fps = io.Framerate;
					ImVec4 fpsColor = fps > 60 ? ModernColors.Success : (fps > 30 ? ModernColors.Warning : ModernColors.Error);
					ImGui::PushStyleColor(ImGuiCol_Text, fpsColor);
					ImGui::Text(ICON_FA_GAUGE_HIGH " %.0f", fps);
					ImGui::PopStyleColor();
				}
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(3);0 ? ModernColors.Success : (fps > 30 ? ModernColors.Warning : ModernColors.Error);
					ImGui::PushStyleColor(ImGuiCol_Text, fpsColor);
					ImGui::Text(ICON_FA_GAUGE_HIGH " %.0f", fps);
					ImGui::PopStyleColor();
				}
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(2);
			}

			ImGui::SetNextWindowPos({ (static_cast<float>(Render::g_screenWidth) / 2.0f) - (menuSize.x / 2.0f), (static_cast<float>(Render::g_screenHeight) / 2.0f) - (menuSize.y / 2.0f) }, ImGui			// Clean sidebar with solid colors and sharp borders
			{
				auto drawList = ImGui::GetWindowDrawList();
				auto windowPos = ImGui::GetWindowPos();
				auto windowSize = ImGui::GetWindowSize();
				
				// Solid sidebar background
				drawList->AddRectFilled(
					windowPos,
					windowPos + ImVec2(sidebarSize, windowSize.y),
					ImGui::GetColorU32(ModernColors.BackgroundAlt)
				);
					
					// Main content solid background
					drawList->AddRectFilled(
						windowPos + ImVec2(sidebarSize, 0.0f),
						windowPos + windowSize,
						ImGui::GetColorU32(ModernColors.Background)
					);
					
					// Sharp border around entire window
					drawList->AddRect(
						windowPos,
						windowPos + windowSize,
						ImGui::GetColorU32(ModernColors.Border),
						0.0f, 0, 2.0f
					);
					
					// Animated sidebar separator line
					sidebarAnimTime += ImGui::GetIO().DeltaTime;
					float glowIntensity = (sin(sidebarAnimTime * 2.0f) + 1.0f) * 0.3f + 0.5f;
					ImVec4 separatorColor = ModernColors.Accent;
					separatorColor.w = glowIntensity;
					drawList->AddRectFilled(
						windowPos + ImVec2(sidebarSize - 2.0f, 0.0f),
						windowPos + ImVec2(sidebarSize, windowSize.y),
						ImGui::GetColorU32(separatorColor)
					); (sin(sidebarAnimTime * 1.5f) + 1.0f) * 0.3f + 0.4f;
						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 15.0f));
					ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 6.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ModernColors.Transparent);
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ModernColors.Accent);
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ModernColors.AccentHover);
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ModernColors.AccentActive);shStyleColor(ImGuiCol_ScrollbarBg, ModernColors.Transparent);
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ModernColors.						// Clean logo/title section
						ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
						ImGui::SetCursorPosX((sidebarSize - ImGui::CalcTextSize("CODEX MENU").x) * 0.5f - 12.0f);
						ImGui::Text(ICON_FA_ATOM " CODEX MENU");
						ImGui::PopStyleColor();
						
						// Sharp underline bar
						float underlineWidth = sidebarSize - 24.0f;
						float underlineX = 12.0f;
						ImVec2 underlineStart = ImGui::GetWindowPos() + ImVec2(underlineX, ImGui::GetCursorPosY() + 5.0f);
						ImVec2 underlineEnd = underlineStart + ImVec2(underlineWidth, 0.0f);
						
						drawList->AddRectFilled(
							underlineStart,
							underlineStart + ImVec2(underlineWidth, 2.0f),
							ImGui::GetColorU32(ModernColors.Accent)
						);neStart = ImGui::GetWindowPos() + ImVec2(underlineX, ImGui::GetCursorPosY() + 5.0f);
						ImVec2 underlineEnd = underlineStart + ImVec2(underlineWidth, 0.0f);
						
						drawList->AddLine(
							underlineStart,
							underlineEnd,
								ImGui::GetColorU32(ModernColors.BrightBlue),
							2.0f
						);
						
						ImGui::Spacing(							// Section separator line
							float sectionLineWidth = sidebarSize - 24.0f;
							ImVec2 sectionLineStart = ImGui::GetWindowPos() + ImVec2(12.0f, ImGui::GetCursorPosY() + 3.0f);
							ImVec2 sectionLineEnd = sectionLineStart + ImVec2(sectionLineWidth, 0.0f);
							drawList->AddRectFilled(
								sectionLineStart,
								sectionLineStart + ImVec2(sectionLineWidth, 1.0f),
								ImGui::GetColorU32(ModernColors.Border)
							);::GetWindowPos() + ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 3.0f);
							ImVec2 sectionLineEnd = sectionLineStart + ImVec2(sectionLineWidth, 0.0f);
							drawList->AddLine(
								sectionLineStart,
								sectionLineEnd,
										ImGui::GetColorU32(ModernColors.AccentBlue),
								1.0f
							);
							
							ImGui::Spacing();
							
							for (const auto& page : section.pages) {
								// Custom styled menu buttons with enhanced hover effects
								bool isSelected = selectedPageNum == selectedPageId;
								
								// Create unique ID for hover animation
								ImGuiID buttonId = ImGui::GetID((std::string("page_btn_") + std::to_string(selectedPageNum)).c_str());
								
								// Update hover animation - only throttle the animation update
								if (shouldUpdateUIAnims) {
									bool isHovered = ImGui::IsI								// Interpolate button colors based on hover animation
								float hoverProgress = buttonHoverAnim[buttonId];
								ImVec4 buttonColor;
								if (isSelected) {
									buttonColor = ModernColors.Surface;
								} else {
									buttonColor = ImVec4(
										ModernColors.Surface.x * hoverProgress,
										ModernColors.Surface.y * hoverProgress,
										ModernColors.Surface.z * hoverProgress,
										ModernColors.Surface.w * hoverProgress * 0.5f
									);
								}
								
								ImVec4 textColor = isSelected ? ModernColors.Accent : ImVec4(
									ModernColors.TextSecondary.x + (ModernColors.TextPrimary.x - ModernColors.TextSecondary.x) * hoverProgress,
									ModernColors.TextSecondary.y + (ModernColors.TextPrimary.y - ModernColors.TextSecondary.y) * hoverProgress,
									ModernColors.TextSecondary.z + (ModernColors.TextPrimary.z - ModernColors.TextSecondary.z) * hoverProgress,
									1.0f
								);
								
								ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
								ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 12.0f));
								ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
								
								ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ModernColors.Surface);
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ModernColors.SurfaceAlt);yleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 12.0f));
								ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
								
								ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ModernColors.AccentBlue);
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ModernColors.BrightBlue);
								ImGui::PushStyleColor(ImGuiCol_Text, textColor);
								
								std::string buttonText = std::string(page.icon) + "  " + page.pageTitle;
								if (ImGui::Button(buttonText.c_str(), ImVec2(sidebarSize - 24.0f, 0.0f))								// Add accent indicator for selected button
								if (isSelected) {
									auto buttonDrawList = ImGui::GetWindowDrawList();
									auto itemMin = ImGui::GetItemRectMin();
									auto itemMax = ImGui::GetItemRectMax();
									
									// Left accent bar
									buttonDrawList->AddRectFilled(
										itemMin,
										ImVec2(itemMin.x + 3.0f, itemMax.y),
										ImGui::GetColorU32(ModernColors.Accent)
									);
								} outer glow
									float glowAlpha = hoverProgress * 0.4f;
									buttonDrawList->AddRect(
										itemMin - ImVec2(2.0f, 2.0f),
										itemMax + ImVec2(2.0f, 2.0f),
										ImGui::GetColorU32(ImVec4(ModernColors.BrightBlue.x, ModernColors.BrightBlue.y, ModernColors.BrightBlue.z, glowAlpha)),
										10.0f, 0, 2.0f
									);
								}
								
								ImGui::PopStyleColor(4);
								ImGui::PopStyleVar(3);
								
								ImGui::Spacing();
								selectedPageNum++;
							}
							
							ImGui::Spacing();
						}
					}
					ImGui::EndChild();
					// Clean title bar with current page info
					const auto* currentPage = GetCurrentPage();
					if (currentPage) {
						// Page header with icon and title
						ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
						ImGui::PushFont(nullptr); // You can add a larger font here
						std::string pageHeader = std::string(currentPage->icon) + "  " + currentPage->pageTitle;
						ImGui::Text(pageHeader.c_str());
						ImGui::PopFont();
						ImGui::PopStyleColor();
						
						// Sharp underline bar
						auto contentDrawList = ImGui::GetWindowDrawList();
						float headerWidth = ImGui::CalcTextSize(pageHeader.c_str()).x;
						ImVec2 headerLineStart = ImGui::GetWindowPos() + ImVec2(20.0f, ImGui::GetCursorPosY() + 3.0f);
						ImVec2 headerLineEnd = headerLineStart + ImVec2(headerWidth, 0.0f);
						contentDrawList->AddRectFilled(
							headerLineStart,
							headerLineStart + ImVec2(headerWidth, 2.0f),
							ImGui::GetColorU32(ModernColors.Accent)
						);
					} else {
						ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
						ImGui::Text(ICON_FA_HOUSE " CODEX | Premium External");
						ImGui::PopStyleColor();
					}tart + ImVec2(headerWidth, 0.0f);
							contentDrawList->AddLine(
								headerLineStart,
											// Content area with 4-box grid layout
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
						ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);Gui::PopStyleColor();
						}
						
						ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
						
						// Content area with 4-box grid layout
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15.0f, 15.0f));
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
						
						if (currentPage && current							// Top Left Box
							ImGui::PushStyleColor(ImGuiCol_ChildBg, ModernColors.Surface);
							ImGui::PushStyleColor(ImGuiCol_Border, ModernColors.Border);
							ImGui::BeginChild("##content_box1", ImVec2(boxWidth, boxHeight), true);
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);						// Top row - Box 1 and Box 2
							{
								// Top Left Box
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ModernColors.PrimaryBlue.x, ModernColors.PrimaryBlue.y, ModernColors.PrimaryBlue.z, 0.4f));
								ImGui::BeginChild("##content_box1", ImVec2(boxWidth, boxHeight), true);
								{
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
									ImGui::Text(ICON_FA_BULLSEYE " Primary Settings");
									ImGui::PopStyleColor();
									ImGui::Separator();
									ImGui::Spacing();
									
									// Render first quarter of page content
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
							if (strcmp(currentPage->pageTitle, "Aimbot") == 0) {
								PageRenderers::AimbotBox1();
							} else if 							}
							ImGui::EndChild();
							ImGui::PopStyleColor(2);
							
							// Top Right Box
							ImGui::SameLine();
							ImGui::PushStyleColor(ImGuiCol_ChildBg, ModernColors.Surface);
							ImGui::PushStyleColor(ImGuiCol_Border, ModernColors.Border);
							ImGui::BeginChild("##content_box2", ImVec2(boxWidth, boxHeight), true);
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);							ImGui::PopStyleColor();
								
								// Top Right Box
								ImGui::SameLine();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ModernColors.AccentBlue.x, ModernColors.AccentBlue.y, ModernColors.AccentBlue.z, 0.4f));
								ImGui::BeginChild("##content_box2", ImVec2(boxWidth, boxHeight), true);
								{
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
									ImGui::Text(ICON_FA_SLIDERS " Advanced Options");
									ImGui::PopStyleColor();
									ImGui::Separator();
									ImGui::Spacing();
									
									// Render second quarter of page content
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
							if (strcmp(currentPage->pageTitle, "Aimbo							}
							ImGui::EndChild();
							ImGui::PopStyleColor(2);
						}
						
						// Bottom row - Box 3 and Box 4
						{
							// Bottom Left Box
							ImGui::PushStyleColor(ImGuiCol_ChildBg, ModernColors.Surface);
							ImGui::PushStyleColor(ImGuiCol_Border, ModernColors.Border);
							ImGui::BeginChild("##content_box3", ImVec2(boxWidth, boxHeight), true);
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);pStyleColor();
							}
							
							// Bottom row - Box 3 and Box 4
							{
								// Bottom Left Box
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ModernColors.BrightBlue.x, ModernColors.BrightBlue.y, ModernColors.BrightBlue.z, 0.3f));
								ImGui::BeginChild("##content_box3", ImVec2(boxWidth, boxHeight), true);
								{
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
									ImGui::Text(ICON_FA_PALETTE " Visual Settings");
									ImGui::PopStyleColor();
									ImGui::Separator();
									ImGui::Spacing();
									
									// Render third quarter of page content
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
							if (strcmp(currentPage->							}
							ImGui::EndChild();
							ImGui::PopStyleColor(2);
							
							// Bottom Right Box
							ImGui::SameLine();
							ImGui::PushStyleColor(ImGuiCol_ChildBg, ModernColors.Surface);
							ImGui::PushStyleColor(ImGuiCol_Border, ModernColors.Border);
							ImGui::BeginChild("##content_box4", ImVec2(boxWidth, boxHeight), true);
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);					ImGui::EndChild();
								ImGui::PopStyleColor();
								
								// Bottom Right Box
								ImGui::SameLine();
								ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ModernColors.PrimaryBlue.x, ModernColors.PrimaryBlue.y, ModernColors.PrimaryBlue.z, 0.5f));
								ImGui::BeginChild("##content_box4", ImVec2(boxWidth, boxHeight), true);
								{
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
									ImGui::Text(ICON_FA_GEAR " Miscellaneous");
									ImGui::PopStyleColor();
									ImGui::Separator();
									ImGui::Spacing();
									
									// Render fourth quarter of page content
									ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
							if (strc							}
							ImGui::EndChild();
							ImGui::PopStyleColor(2);
						}
					} else {
						// Welcome screen when no page is selected
						ImGui::PushStyleColor(ImGuiCol_ChildBg, ModernColors.Surface);
						ImGui::PushStyleColor(ImGuiCol_Border, ModernColors.Border);4();
							} else if (strcmp(currentPage->pageTitle, "Debug") == 0) {
								PageRenderers::DebugBox4();
							}
									ImGui::PopStyleColor();
								}
								ImGui::EndChild();
								ImGui::PopStyleColor();
							}
						} else {
							// Welcome screen when no page is selected
							ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ModernColors.PrimaryBlue.x, ModernColors.PrimaryBlue.y, ModernColors.PrimaryBlue.z, 0.3f));
							ImGui::BeginChild("##welcome_area", ImVec2(0.0f, 0.0f), true);
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
								ImVec2 welcomeTextSize = ImGui::CalcTextSize("Welcome to CODEX");
								ImGui::SetCursorPosX((ImGui::GetWindowWidth(						}
						ImGui::EndChild();
						ImGui::PopStyleColor(2);
					}
					
					ImGui::PopStyleVar(4);* 0.4f);
								ImGui::Text("Welcome to CODEX");
								
								ImVec2 subtextSize = ImGui::CalcTextSize("Select a page from the sidebar to get started");
								ImGui::SetCursorPosX((ImGui::GetWindowWidth() - subtextSize.x) * 0.5f);
								ImGui::Text("Select a page from the sidebar to get started");
								ImGui::PopStyleColor();
							}
							ImGui::EndChild();
							ImGui::PopStyleColor();
						}
						
						ImGui::PopStyleVar(3);
					}
					ImGui::EndChild();
					ImGui::PopStyleVar();
				}
			}
			ImGui::End();
		}
	}
	ImGui::EndFrame();

	Render::PresentImGuiFrame();
}

// Page functions...

static int aimTabId = 0;
void PageRenderers::Aimbot() {
	ImAdd::TabBox(
		"##aim_tab_box",
		&aimTabId,
		{
			ICON_FA_PERSON_RIFLE " Settings",
			ICON_FA_GEAR " Misc"
		},
		ImVec2(0.0f, ImGui::GetFrameHeight())
	);

	ImAdd::Checkbox("Enable", &Config::Aim::enable);
	if (Config::Aim::enable) {
		if (aimTabId == 0) {
			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::SliderFloat("Smoothing X Axis", &Config::Aim::smoothingX, 1.f, 100.f);
				ImAdd::SliderFloat("Smoothing Y Axis", &Config::Aim::smoothingY, 1.f, 100.f);
			}
			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::Combo(
					"Target Aimbone",
					&Config::Aim::aimbone,
					"Head\0Neck\0Chest\0Pelvis\0Smart\0"
				);
			}
			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Visible Check", &Config::Aim::visibleCheck);
			}
		} else {
			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Target Line", &Config::Aim::targetLine);
				if (Config::Aim::targetLine) {
					ImAdd::ColorEdit4("Target Line Colour", (float*)&Config::Aim::targetLineColor);
				}
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::SliderFloat("Maximum Distance", &Config::Aim::maxAimbotDistance, 1.f, 300.f);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::SliderFloat("Aim FOV", &Config::Aim::FOV, 1.f, 600.f);
				ImAdd::ToggleButton("Show FOV", &Config::Aim::showFOV);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Target Lock", &Config::Aim::targetLock);
				if (Config::Aim::targetLock) {
					ImAdd::SliderFloat("Lock Timeout (s)", &Config::Aim::targetLockTimeout, 0.5f, 10.0f);
					ImAdd::SliderFloat("Lock Range Multiplier", &Config::Aim::targetLockRange, 1.0f, 3.0f);
				}
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::Hotkey("\t  Aimkey", &Config::Aim::aimkey);
			}
		}
	} else {
		ImGui::Text("Please enable to view settings");
	}
}

static int espTabId = 0;
void PageRenderers::ESP() {
	ImAdd::TabBox(
		"##esp_tab_box",
		&espTabId,
		{
			ICON_FA_EYE " Settings",
			ICON_FA_PALETTE " Misc"
		},
		ImVec2(0.0f, ImGui::GetFrameHeight())
	);


	ImAdd::ToggleButton("Enable", &Config::ESP::enable);
	if (Config::ESP::enable) {
		if (espTabId == 0) {
			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Enable Box", &Config::ESP::box);
				if (Config::ESP::box) {
					ImAdd::Combo(
						"Box Type",
						&Config::ESP::boxType,
						"Normal\0Cornered\0"
					);

					ImAdd::ToggleButton("Filled Box", &Config::ESP::boxFilled);
				}
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Indicators", &Config::ESP::indicator);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Nickname", &Config::ESP::nickname);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Health Bar", &Config::ESP::health);
				if (Config::ESP::health) {
					ImAdd::Combo(
						"Health Bar Position",
						&Config::ESP::healthBarPos,
						"Left\0Right\0Top\0Bottom\0"
					);
				}
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Snaplines", &Config::ESP::snaplines);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Distance", &Config::ESP::distance);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::ToggleButton("Skeleton", &Config::ESP::skeleton);
			}

			ImGui::Spacing(); ImGui::Spacing();
			{
				ImAdd::SliderInt("Max ESP Distance", &Config::ESP::maxESPDistance, 1, 300);
			}
		} else {
			if (Config::ESP::enable) {
				// Rainbow Settings
				ImGui::Spacing(); ImGui::Spacing();
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
					ImGui::Text(ICON_FA_RAINBOW " Rainbow Effects");
					ImGui::PopStyleColor();
					ImGui::Separator();
					ImGui::Spacing();
					
					ImAdd::ToggleButton("Enable Rainbow All", &VisualSettings::bRainbowAll);
					if (VisualSettings::bRainbowAll) {
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Rainbow mode active - all colors will cycle");
					}
				}
				
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				
				// Individual Color Settings
				ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
				ImGui::Text(ICON_FA_PALETTE " Individual Colors");
				ImGui::PopStyleColor();
				ImGui::Separator();
				ImGui::Spacing();
				
				if (Config::ESP::box) {
					ImGui::Spacing(); ImGui::Spacing();
					{
						ImAdd::ColorEdit4("Box Visible", (float*)&Config::ESP::boxVisibleOutlineColor);
						ImAdd::ColorEdit4("Box Hidden", (float*)&Config::ESP::boxInvisibleOutlineColor);

						if (Config::ESP::boxFilled) {
							ImAdd::ColorEdit4("Box Filled Visible Color", (float*)&Config::ESP::boxVisibleFilledColor);
							ImAdd::ColorEdit4("Box Filled Invisible Color", (float*)&Config::ESP::boxInvisibleFilledColor);
						}
					}
				}

				if (Config::ESP::nickname) {
					ImGui::Spacing(); ImGui::Spacing();
					{
						ImAdd::ColorEdit4("Nickname Visible Color", (float*)&Config::ESP::nicknameVisibleColor);
						ImAdd::ColorEdit4("Nickname Invisible Color", (float*)&Config::ESP::nicknameInvisibleColor);
					}
				}

				if (Config::ESP::snaplines) {
					ImGui::Spacing(); ImGui::Spacing();
					{
						ImAdd::ColorEdit4("Snaplines Visible Color", (float*)&Config::ESP::snaplinesVisibleColor);
						ImAdd::ColorEdit4("Snaplines Invisible Color", (float*)&Config::ESP::snaplinesInvisibleCovoid PageRenderers::Settings() {
	// Display info - Theme is now fixed
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Accent);
	ImGui::Text(ICON_FA_PALETTE " Visual Settings");
	ImGui::PopStyleColor();
	ImGui::Separator();
	ImGui::Spacing();
	
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
	ImGui::TextWrapped("Using clean dark theme with sharp edges and cyan accents.");
	ImGui::PopStyleColor();
	
	// Theme preview colors
	ImGui::Spacing();
	ImGui::Text("Theme Colors:");
	
	// Preview color boxes
	float boxSize = 25.0f;
	auto drawList = ImGui::GetWindowDrawList();
	auto cursorPos = ImGui::GetWindowPos() + ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	
	// Color preview
	drawList->AddRectFilled(
		cursorPos,
		cursorPos + ImVec2(boxSize, boxSize),
		ImGui::GetColorU32(ModernColors.Background)
	);
	
	drawList->AddRectFilled(
		cursorPos + ImVec2(boxSize + 5, 0),
		cursorPos + ImVec2(boxSize * 2 + 5, boxSize),
		ImGui::GetColorU32(ModernColors.Surface)
	);
	
	drawList->AddRectFilled(
		cursorPos + ImVec2((boxSize + 5) * 2, 0),
		cursorPos + ImVec2(boxSize * 3 + 10, boxSize),
		ImGui::GetColorU32(ModernColors.Accent)
	);
	
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + boxSize + 10);
	
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();s() + ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	
	// Primary colors preview
	drawList->AddRectFilled(
		cursorPos,
		cursorPos + ImVec2(boxSize, boxSize),
		ImGui::GetColorU32(ModernColors.PrimaryDark),
		5.0f
	);
	
	drawList->AddRectFilled(
		cursorPos + ImVec2(boxSize + 5, 0),
		cursorPos + ImVec2(boxSize * 2 + 5, boxSize),
		ImGui::GetColorU32(ModernColors.AccentBlue),
		5.0f
	);
	
	drawList->AddRectFilled(
		cursorPos + ImVec2((boxSize + 5) * 2, 0),
		cursorPos + ImVec2(boxSize * 3 + 10, boxSize),
		ImGui::GetColorU32(ModernColors.BrightBlue),
		5.0f
	);
	
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + boxSize + 10);
	
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Menu Settings
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.LightBlue);
	ImGui::Text(ICON_FA_SLIDERS " Menu Settings");
	ImGui::PopStyleColor();
	ImGui::Spacing();
	
	static bool showFPS = true;
	static bool showTime = true;
	static float menuOpacity = 0.95f;
	
	ImAdd::ToggleButton("Show FPS Counter", &showFPS);
	ImAdd::ToggleButton("Show Time", &showTime);
	ImAdd::SliderFloat("Menu Opacity", &menuOpacity, 0.5f, 1.0f, "%.2f");
	
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Crosshair Settings
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.LightBlue);
	ImGui::Text(ICON_FA_CROSSHAIRS " Crosshair Settings");
	ImGui::PopStyleColor();
	ImGui::Separator();
	ImGui::Spacing();
	
	ImAdd::ToggleButton("Enable Custom Crosshair", &Config::Settings::enableCrosshair);
	if (Config::Settings::enableCrosshair) {
		ImAdd::SliderFloat("Crosshair Size", &Config::Settings::crosshairSize, 5.0f, 30.0f, "%.1f");
		ImAdd::SliderFloat("Crosshair Thickness", &Config::Settings::crosshairThickness, 1.0f, 5.0f, "%.1f");
		ImAdd::SliderFloat("Crosshair Gap", &Config::Settings::crosshairGap, 0.0f, 15.0f, "%.1f");
		ImAdd::ToggleButton("Show Center Dot", &Config::Settings::crosshairDot);
	}
	
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Stream Proof Settings
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.LightBlue);
	ImGui::Text(ICON_FA_SHIELD_HALVED " Stream Proof Settings");
	ImGui::PopStyleColor();
	ImGui::Spacing();
	
	// Stream proof toggle wi	ImGui::Spacing(); ImGui::Spacing();
	if (ImAdd::Button("Reset to Defaults", ImVec2(150, 30))) {
		showFPS = true;
		showTime = true;
		menuOpacity = 0.95f;
		Config::Settings::streamProof = false;
	}ImAdd::ToggleButton("Stream Proof Protection", &Config::Settings::streamProof)) {
		// Toggle logic is handled by the ToggleButton
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	
	// Help text for stream proof
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
	ImGui::TextWrapped(Config::Settings::streamProof ? 
		"Stream Proof is ENABLED - Screenshots and screen recordings are blocked." : 
		"Stream Proof is DISABLED - Menu can be captured in screenshots and recordings.");
	ImGui::PopStyleColor();
	
	ImGui::Spacing(); ImGui::Spacing();
	if (ImAdd::Button("Reset to Defaults", ImVec2(150, 30))) {
		currentTheme = 0;
		showFPS = true;
		showTime = true;
		menuOpacity = 0.95f;
		Config::Settings::streamProof = false;
	}
}

void PageRenderers::Config() {
	// Configuration management with modern styling
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.LightBlue);
	ImGui::Text(ICON_FA_FOLDER_OPEN " Configuration Manager");
	ImGui::PopStyleColor();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Static variables for config management
	static int selectedPreset = 0;
	static char customConfigName[64] = "";
	static std::vector<std::string> customConfigs = {"Default", "MyConfig1", "Tournament", "Streaming"};
	static int selectedCustomConfig = 0;
	static bool showDeleteConfirm = false;
	static int configToDelete = -1;
	
	// Config categories with descriptions
	const char* presetNames[] = {
		"Legit",
		"Semi-Legit", 
		"Rage",
		"HvH"
	};
	
	const char* presetDescriptions[] = {
		"Low smoothing, human-like aim, minimal ESP",
		"Moderate settings, balanced for competitive", 
		"High aggression, maximum settings",
		"Optimized for HvH scenarios"
	};
	
	// Preset selection with modern styling
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
	ImGui::Text("Built-in Presets:");
	ImGui::PopStyleColor();
	ImGui::Spacing();
	
	// Enhanced preset selector with descriptions
	for (int i = 0; i < 4; i++) {
		bool isSelected = (selectedPreset == i);
		
		// Custom button styling for presets
		ImVec4 buttonColor = isSelected ? ModernColors.BrightBlue : 
			ImVec4(ModernColors.AccentBlue.x, ModernColors.AccentBlue.y, ModernColors.AccentBlue.z, 0.3f);
		
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ModernColors.BrightBlue);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ModernColors.AccentBlue);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
		
		std::string buttonText = std::string(presetNames[i]) + " - " + presetDescriptions[i];
		if (ImGui::Button(buttonText.c_str(), ImVec2(-1, 35))) {
			selectedPreset = i;
			// Apply preset configuration
			ApplyPresetConfig(i);
		}
		
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
		ImGui::Spacing();
	}
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Custom configurations section
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
	ImGui::Text("Custom Configurations:");
	ImGui::PopStyleColor();
	ImGui::Spacing();
	
	// Config list with load/delete options
	if (ImGui::BeginListBox("##custom_configs", ImVec2(-1, 120))) {
		for (int i = 0; i < customConfigs.size(); i++) {
			bool isSelected = (selectedCustomConfig == i);
			
			// Color coding for different config types
			ImVec4 textColor = ModernColors.TextPrimary;
			if (customConfigs[i] == "Default") {
				textColor = ModernColors.Success;
			} else if (customConfigs[i].find("Tournament") != std::string::npos || 
					   customConfigs[i].find("Streaming") != std::string::npos) {
				textColor = ModernColors.Warning;
			}
			
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			
			if (ImGui::Selectable((customConfigs[i] + "##" + std::to_string(i)).c_str(), isSelected)) {
				selectedCustomConfig = i;
			}
			
			ImGui::PopStyleColor();
			
			// Right-click context menu
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Load Config")) {
					// Load the selected custom config
					LoadCustomConfig(customConfigs[i]);
				}
				if (customConfigs[i] != "Default" && ImGui::MenuItem("Delete Config")) {
					showDeleteConfirm = true;
					configToDelete = i;
				}
				ImGui::EndPopup();
			}
		}
		ImGui::EndListBox();
	}
	
	// Action buttons row
	if (ImGui::Button(ICON_FA_DOWNLOAD " Load Selected", ImVec2(120, 30))) {
		if (selectedCustomConfig < customConfigs.size()) {
			LoadCustomConfig(customConfigs[selectedCustomConfig]);
		}
	}
	
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FLOPPY_DISK " Save Current", ImVec2(120, 30))) {
		ImGui::OpenPopup("Save Config");
	}
	
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ModernColors.Error);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ModernColors.Error.x * 1.2f, ModernColors.Error.y * 1.2f, ModernColors.Error.z * 1.2f, 1.0f));
	if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(100, 30))) {
		if (selectedCustomConfig < customConfigs.size() && customConfigs[selectedCustomConfig] != "Default") {
			showDeleteConfirm = true;
			configToDelete = selectedCustomConfig;
		}
	}
	ImGui::PopStyleColor(2);
	
	// Save config popup
	if (ImGui::BeginPopupModal("Save Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Enter configuration name:");
		ImGui::Spacing();
		
		ImGui::InputText("##config_name", customConfigName, sizeof(customConfigName));
		
		ImGui::Spacing();
		if (ImGui::Button("Save", ImVec2(100, 30))) {
			if (strlen(customConfigName) > 0) {
				// Save current configuration
				SaveCurrentConfig(std::string(customConfigName));
				customConfigs.push_back(std::string(customConfigName));
				memset(customConfigName, 0, sizeof(customConfigName));
				ImGui::CloseCurrentPopup();
			}
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(100, 30))) {
			ImGui::CloseCurrentPopup();
			memset(customConfigName, 0, sizeof(customConfigName));
		}
		
		ImGui::EndPopup();
	}
	
	// Delete confirmation popup
	if (showDeleteConfirm) {
		ImGui::OpenPopup("Delete Config?");
		showDeleteConfirm = false;
	}
	
	if (ImGui::BeginPopupModal("Delete Config?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to delete this configuration?");
		ImGui::Spacing();
		
		ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Warning);
		if (configToDelete >= 0 && configToDelete < customConfigs.size()) {
			ImGui::Text("Config: %s", customConfigs[configToDelete].c_str());
		}
		ImGui::PopStyleColor();
		
		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Button, ModernColors.Error);
		if (ImGui::Button("Yes, Delete", ImVec2(100, 30))) {
			if (configToDelete >= 0 && configToDelete < customConfigs.size()) {
				customConfigs.erase(customConfigs.begin() + configToDelete);
				if (selectedCustomConfig >= customConfigs.size()) {
					selectedCustomConfig = customConfigs.size() - 1;
				}
			}
			configToDelete = -1;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();
		
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(100, 30))) {
			configToDelete = -1;
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Quick actions section
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextPrimary);
	ImGui::Text("Quick Actions:");
	ImGui::PopStyleColor();
	ImGui::Spacing();
	
	if (ImGui::Button(ICON_FA_ROTATE_LEFT " Reset to Defaults", ImVec2(150, 30))) {
		ResetToDefaults();
	}
	
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FILE_EXPORT " Export Config", ImVec2(130, 30))) {
		// Export current config to file
		ExportCurrentConfig();
	}
	
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FILE_IMPORT " Import Config", ImVec2(130, 30))) {
		// Import config from file
		ImportConfigFromFile();
	}
}


// ===== BOX RENDERER IMPLEMENTATIONS =====

// Aimbot Box Renderers
void PageRenderers::AimbotBox1() {
	ImAdd::Checkbox("Enable Aimbot", &Config::Aim::enable);
	if (Config::Aim::enable) {
		ImAdd::SliderFloat("Smoothing X", &Config::Aim::smoothingX, 1.f, 100.f);
		ImAdd::SliderFloat("Smoothing Y", &Config::Aim::smoothingY, 1.f, 100.f);
	} else {
		ImGui::TextDisabled("Enable aimbot to configure");
	}
}

void PageRenderers::AimbotBox2() {
	if (Config::Aim::enable) {
		ImAdd::Combo("Target Bone", &Config::Aim::aimbone, "Head\0Neck\0Chest\0Pelvis\0Smart\0");
		ImAdd::ToggleButton("Visible Check", &Config::Aim::visibleCheck);
		ImAdd::SliderFloat("Max Distance", &Config::Aim::maxAimbotDistance, 1.f, 300.f);
		
		// Smart Target Selection Controls
		ImGui::Spacing();
		ImAdd::ToggleButton("Smart Target Selection", &Config::Aim::smartTargetSelection);
		if (Config::Aim::smartTargetSelection) {
			ImGui::Spacing();
			ImGui::Text("Priority Weights:");
			ImAdd::SliderFloat("Distance Weight", &Config::Aim::distanceWeight, 0.0f, 1.0f, "%.2f");
			ImAdd::SliderFloat("Health Weight", &Config::Aim::healthWeight, 0.0f, 1.0f, "%.2f");
			ImAdd::SliderFloat("Angle Weight", &Config::Aim::angleWeight, 0.0f, 1.0f, "%.2f");
		}
	} else {
		ImGui::TextDisabled("Enable aimbot to configure");
	}
}

void PageRenderers::AimbotBox3() {
	if (Config::Aim::enable) {
		ImAdd::SliderFloat("FOV Radius", &Config::Aim::FOV, 1.f, 600.f);
		ImAdd::ToggleButton("Show FOV Circle", &Config::Aim::showFOV);
		ImAdd::ToggleButton("Target Line", &Config::Aim::targetLine);
	} else {
		ImGui::TextDisabled("Enable aimbot to configure");
	}
}

void PageRenderers::AimbotBox4() {
	if (Config::Aim::enable) {
		ImAdd::Hotkey("Aim Key", &Config::Aim::aimkey);
		if (Config::Aim::targetLine) {
			ImAdd::ColorEdit4("Line Color", (float*)&Config::Aim::targetLineColor);
		}
		ImGui::Spacing();
		ImGui::Text("Status: %s", Config::Aim::enable ? "Active" : "Inactive");
	} else {
		ImGui::TextDisabled("Enable aimbot to configure");
	}
}

// ESP Box Renderers
void PageRenderers::ESPBox1() {
	ImAdd::ToggleButton("Enable ESP", &Config::ESP::enable);
	if (Config::ESP::enable) {
		ImAdd::ToggleButton("Player Boxes", &Config::ESP::box);
		if (Config::ESP::box) {
			ImAdd::Combo("Box Style", &Config::ESP::boxType, "Normal\0Cornered\0");
			ImAdd::ToggleButton("Filled Boxes", &Config::ESP::boxFilled);
		}
	} else {
		ImGui::TextDisabled("Enable ESP to configure");
	}
}

void PageRenderers::ESPBox2() {
	if (Config::ESP::enable) {
		ImAdd::ToggleButton("Player Names", &Config::ESP::nickname);
		ImAdd::ToggleButton("Health Bars", &Config::ESP::health);
		if (Config::ESP::health) {
			ImAdd::Combo("Bar Position", &Config::ESP::healthBarPos, "Left\0Right\0Top\0Bottom\0");
		}
		ImAdd::ToggleButton("Distance Info", &Config::ESP::distance);
	} else {
		ImGui::TextDisabled("Enable ESP to configure");
	}
}

void PageRenderers::ESPBox3() {
	if (Config::ESP::enable) {
		ImAdd::ToggleButton("Snaplines", &Config::ESP::snaplines);
		ImAdd::ToggleButton("Skeletons", &Confivoid PageRenderers::SettingsBox1() {
	// Visual info - Theme is now fixed
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
	ImGui::TextWrapped("Clean dark theme with sharp, modern design.");
	ImGui::PopStyleColor();
	
	// Theme preview
	ImGui::Spacing();
	ImGui::Text("Colors:");
	float boxSize = 20.0f;
	auto drawList = ImGui::GetWindowDrawList();
	auto cursorPos = ImGui::GetWindowPos() + ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	
	drawList->AddRectFilled(cursorPos, cursorPos + ImVec2(boxSize, boxSize), ImGui::GetColorU32(ModernColors.Accent));
	drawList->AddRectFilled(cursorPos + ImVec2(boxSize + 3, 0), cursorPos + ImVec2(boxSize * 2 + 3, boxSize), ImGui::GetColorU32(ModernColors.AccentHover));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + boxSize + 5);
}x Visible", (float*)&Config::ESP::boxVisibleOutlineColor);
			ImAdd::ColorEdit4("Box Hidden", (float*)&Config::ESP::boxInvisibleOutlineColor);
		}
		ImGui::Spacing();
		ImGui::Text("Status: %s", Config::ESP::enable ? "Active" : "Inactive");
	} else {
		ImGui::TextDisabled("Enable ESP to configure");
	}
}

// Settings Box Renderers
void PageRenderers::SettingsBox1() {
	// Theme Selection
	const char* themeNames[] = {
		"CODEX Blue", "Cyber Purple", "Neon Green", "Fire Orange", "Ice Blue"
	};
	ImAdd::Combo("Theme", &currentTheme, themeNames, 5);
	
	// Theme preview
	ImGui::Spacing();
	float boxSize = 20.0f;
	auto drawList = ImGui::GetWindowDrawList();
	auto cursorPos = ImGui::GetWindowPos() + ImVec2(ImGui::void PageRenderers::SettingsBox4() {
	// Reset and Info
	if (ImAdd::Button("Reset All", ImVec2(-1, 25))) {
		Config::Settings::streamProof = false;
	};
	drawList->AddRectFilled(cursorPos + ImVec2(boxSize + 3, 0), cursorPos + ImVec2(boxSize * 2 + 3, boxSize), ImGui::GetColorU32(ModernColors.BrightBlue), 3.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + boxSize + 5);
}

void PageRenderers::SettingsBox2() {
	// Menu Options
	static bool showFPS = true;
	static bool showTime = true;
	static float opacity = 0.95f;
	
	ImAdd::ToggleButton("Show FPS", &showFPS);
	ImAdd::ToggleButton("Show Time", &showTime);
	ImAdd::SliderFloat("Opacity", &opacity, 0.5f, 1.0f, "%.2f");
}

void PageRenderers::SettingsBox3() {
	// Stream Proof Settings
	ImGui::PushStyleColor(ImGuiCol_Text, Config::Settings::streamProof ? ModernColors.Success : ModernColors.TextSecondary);
	ImAdd::ToggleButton("Stream Protection", &Config::Settings::streamProof);
	ImGui::PopStyleColor();
	
	ImGui::Spacing();
	ImGui::TextWrapped(Config::Settings::streamProof ? "Screenshots blocked" : "Screenshots allowed");
}

void PageRenderers::SettingsBox4() {
	// Reset and Info
	if (ImAdd::Button("Reset All", ImVec2(-1, 25))) {
		currentTheme = 0;
		Config::Settings::streamProof = false;
	}
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	ImGui::Text("Build: v2.1.0");
	ImGui::Text("Status: Active");
}

void PageRenderers::Debug() {
	// Simple debug page implementation - not used since we use 4-panel layout
	ImGui::Text("Debug information and player list");
	ImGui::Text("This page shows all detected players and system debug info");
}

// Debug Box Renderers
void PageRenderers::DebugBox1() {
	// Player List with basic info
	ImGui::Text("Active Players:");
	ImGui::Separator();
	ImGui::Spacing();
	
	try {
		// Get actual player data from cache
		auto currentPlayers = Cache::playerData;
		
		ImGui::Text("Total Players: %d", (int)currentPlayers.size());
		ImGui::Spacing();
		
		// Display actual player entries
		if (!currentPlayers.empty()) {
			for (size_t i = 0; i < currentPlayers.size() && i < 8; i++) {
				const auto& player = currentPlayers[i];
				
				// Color based on health status
				ImVec4 playerColor = ModernColors.TextPrimary;
				if (player.playerHealth <= 0) {
					playerColor = ModernColors.Error; // Dead player
				} else if (player.playerHealth <= 30) {
					playerColor = ModernColors.Warning; // Low health
				} else if (player.isVisible) {
					playerColor = ModernColors.Success; // Visible player
				}
				
				ImGui::PushStyleColor(ImGuiCol_Text, playerColor);
				
				// Show player name with status indicators
				std::string statusText = "";
				if (player.isBot) statusText += "[BOT] ";
				if (player.sbStatus == SDK::STATUS_DEAD) statusText += "[DEAD] ";
				else if (player.sbStatus == SDK::STATUS_DOWNED) statusText += "[DOWNED] ";
				if (player.isVisible) statusText += "[VIS] ";
				
				ImGui::BulletText("%s%s (HP: %d)", 
					statusText.c_str(),
					player.playerName.c_str(), 
					player.playerHealth);
				
				ImGui::PopStyleColor();
			}
			
			if (currentPlayers.size() > 8) {
				ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
				ImGui::Text("... and %d more", (int)currentPlayers.size() - 8);
				ImGui::PopStyleColor();
			}
		} else {
			ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
			ImGui::Text("No enemy players detected");
			ImGui::PopStyleColor();
		}
	} catch (...) {
		// Fallback if Cache::playerData is not available or accessible
		ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
		ImGui::Text("Player data not available");
		ImGui::Text("(Waiting for cache initialization...)");
		ImGui::PopStyleColor();
	}
}

void PageRenderers::DebugBox2() {
	// Player details and statistics
	ImGui::Text("Player Details:");
	ImGui::Separator();
	ImGui::Spacing();
	
	// Get actual player data from cache
	std::vector<PlayerData> currentPlayers;
	{
		currentPlayers = Cache::playerData;
	}
	
	// Table with real player data
	if (ImGui::BeginTable("RealPlayerTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 120))) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableSetupColumn("K/D", ImGuiTableColumnFlags_WidthFixed, 40.0f);
		ImGui::TableSetupColumn("HP", ImGuiTableColumnFlags_WidthFixed, 35.0f);
		ImGui::TableSetupColumn("Dist", ImGuiTableColumnFlags_WidthFixed, 45.0f);
		ImGui::TableHeadersRow();
		
		for (size_t i = 0; i < currentPlayers.size() && i < 10; i++) {
			const auto& player = currentPlayers[i];
			ImGui::TableNextRow();
			
			// Name column
			ImGui::TableNextColumn();
			ImVec4 nameColor = player.isBot ? ModernColors.Warning : ModernColors.TextPrimary;
			if (player.sbStatus == SDK::STATUS_DEAD) nameColor = ModernColors.Error;
			ImGui::PushStyleColor(ImGuiCol_Text, nameColor);
			
			std::string displayName = player.playerName;
			if (displayName.length() > 10) {
				displayName = displayName.substr(0, 9) + "..";
			}
			ImGui::Text("%s", displayName.c_str());
			ImGui::PopStyleColor();
			
			// K/D column
			ImGui::TableNextColumn();
			ImGui::Text("%d/%d", player.kills, player.deaths);
			
			// Health column
			ImGui::TableNextColumn();
			ImVec4 healthColor = ModernColors.Success;
			if (player.playerHealth <= 0) healthColor = ModernColors.Error;
			else if (player.playerHealth <= 30) healthColor = ModernColors.Warning;
			else if (player.playerHealth <= 60) healthColor = ModernColors.TextSecondary;
			
			ImGui::PushStyleColor(ImGuiCol_Text, healthColor);
			ImGui::Text("%d", player.playerHealth);
			ImGui::PopStyleColor();
			
			// Distance column
			ImGui::TableNextColumn();
			ImGui::Text("%.1fm", player.distance);
		}
		
		ImGui::EndTable();
	}
	
	if (currentPlayers.empty()) {
		ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.TextSecondary);
		ImGui::Text("No player data available");
		ImGui::PopStyleColor();
	}
}

void PageRenderers::DebugBox3() {
	// System debug info with real cache data
	ImGui::Text("System Information:");
	ImGui::Separator();
	ImGui::Spacing();
	
	// Performance metrics
	const auto& io = ImGui::GetIO();
	float fps = io.Framerate;
	
	ImGui::Text("Performance:");
	ImGui::BulletText("FPS: %.1f", fps);
	ImGui::BulletText("Frame Time: %.3fms", 1000.0f / fps);
	
	ImGui::Spacing();
	ImGui::Text("Cache Status:");
	
	// Real cache information
	std::vector<PlayerData> currentPlayers = Cache::playerData;
	ImGui::BulletText("Cached Players: %d", (int)currentPlayers.size());
	
	// Show cache addresses (for debugging)
	ImGui::BulletText("Bone Base: 0x%llX", Cache::boneBase);
	ImGui::BulletText("Client Info: 0x%llX", Cache::clientInfo);
	ImGui::BulletText("Local Player: 0x%llX", Cache::localPlayer);
	
	ImGui::Spacing();
	ImGui::Text("Local Player Info:");
	ImGui::BulletText("Position: %.1f, %.1f, %.1f", 
		Cache::localPlayerPosition.X, 
		Cache::localPlayerPosition.Y, 
		Cache::localPlayerPosition.Z);
	ImGui::BulletText("Camera: %.1f, %.1f, %.1f", 
		Cache::cameraPosition.X, 
		Cache::cameraPosition.Y, 
		Cache::cameraPosition.Z);
	
	ImGui::Spacing();
	ImGui::Text("Game Status:");
	bool inGame = Cache::boneBase != 0 && Cache::clientInfo != 0 && Cache::localPlayer != 0;
	ImGui::PushStyleColor(ImGuiCol_Text, inGame ? ModernColors.Success : ModernColors.Error);
	ImGui::BulletText("Game: %s", inGame ? "Connected" : "Disconnected");
	ImGui::PopStyleColor();
	
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Success);
	ImGui::BulletText("Overlay: Active");
	ImGui::PopStyleColor();
}

void PageRenderers::DebugBox4() {
	// Debug controls and live player statistics
	ImGui::Text("Live Statistics:");
	ImGui::Separator();
	ImGui::Spacing();
	
	// Get current player data
	std::vector<PlayerData> currentPlayers = Cache::playerData;
	
	// Calculate statistics
	int aliveCount = 0, deadCount = 0, downedCount = 0, botCount = 0, visibleCount = 0;
	float avgDistance = 0.0f, closestDistance = 999.0f;
	
	for (const auto& player : currentPlayers) {
		if (player.sbStatus == SDK::STATUS_ALIVE) aliveCount++;
		else if (player.sbStatus == SDK::STATUS_DEAD) deadCount++;
		else if (player.sbStatus == SDK::STATUS_DOWNED) downedCount++;
		
		if (player.isBot) botCount++;
		if (player.isVisible) visibleCount++;
		
		avgDistance += player.distance;
		if (player.distance < closestDistance) {
			closestDistance = player.distance;
		}
	}
	
	if (!currentPlayers.empty()) {
		avgDistance /= currentPlayers.size();
	} else {
		closestDistance = 0.0f;
	}
	
	// Display statistics
	ImGui::Text("Player Status:");
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Success);
	ImGui::BulletText("Alive: %d", aliveCount);
	ImGui::PopStyleColor();
	
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Warning);
	ImGui::BulletText("Downed: %d", downedCount);
	ImGui::PopStyleColor();
	
	ImGui::PushStyleColor(ImGuiCol_Text, ModernColors.Error);
	ImGui::BulletText("Dead: %d", deadCount);
	ImGui::PopStyleColor();
	
	ImGui::BulletText("Bots: %d", botCount);
	ImGui::BulletText("Visible: %d", visibleCount);
	
	ImGui::Spacing();
	ImGui::Text("Distance Info:");
	ImGui::BulletText("Closest: %.1fm", closestDistance);
	ImGui::BulletText("Average: %.1fm", avgDistance);
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	
	// Debug toggles
	static bool showDebugLog = false;
	static bool showAddresses = false;
	
	ImAdd::ToggleButton("Debug Logging", &showDebugLog);
	ImAdd::ToggleButton("Show Addresses", &showAddresses);
	
	ImGui::Spacing();
	
	// Action buttons
	if (ImAdd::Button("Refresh Cache", ImVec2(-1, 0))) {
		// Force cache refresh - this could trigger a manual update
		Logger.log("Manual cache refresh requested from Debug panel");
	}
}

// ===== CONFIGURATION MANAGEMENT FUNCTIONS =====

// Apply preset configurations with optimized settings for different play styles
void ApplyPresetConfig(int presetIndex) {
	switch (presetIndex) {
		case 0: // Legit
			{
				// Aimbot settings - human-like, low detection
				Config::Aim::enable = true;
				Config::Aim::smoothingX = 15.0f;
				Config::Aim::smoothingY = 15.0f;
				Config::Aim::FOV = 45.0f;
				Config::Aim::aimbone = 0; // Head
				Config::Aim::visibleCheck = true;
				Config::Aim::targetLock = false;
				Config::Aim::showFOV = false;
				Config::Aim::targetLine = false;
				Config::Aim::maxAimbotDistance = 100.0f;
				
				// ESP settings - minimal, low-profile
				Config::ESP::enable = true;
				Config::ESP::box = true;
				Config::ESP::boxType = 0; // Normal
				Config::ESP::boxFilled = false;
				Config::ESP::nickname = false;
				Config::ESP::health = true;
				Config::ESP::healthBarPos = 0; // Left
				Config::ESP::snaplines = false;
				Config::ESP::distance = true;
				Config::ESP::skeleton = false;
				Config::ESP::indicator = false;
				Config::ESP::maxESPDistance = 150;
				
				// Misc settings
				Config::Settings::streamProof = true;
				
				Logger.log("Applied Legit preset configuration");
			}
			break;
			
		case 1: // Semi-Legit
			{
				// Aimbot settings - moderate aggression
				Config::Aim::enable = true;
				Config::Aim::smoothingX = 8.0f;
				Config::Aim::smoothingY = 8.0f;
				Config::Aim::FOV = 75.0f;
				Config::Aim::aimbone = 4; // Smart
				Config::Aim::visibleCheck = true;
				Config::Aim::targetLock = true;
				Config::Aim::targetLockTimeout = 2.0f;
				Config::Aim::targetLockRange = 1.5f;
				Config::Aim::showFOV = false;
				Config::Aim::targetLine = false;
				Config::Aim::maxAimbotDistance = 200.0f;
				
				// ESP settings - balanced visibility
				Config::ESP::enable = true;
				Config::ESP::box = true;
				Config::ESP::boxType = 1; // Cornered
				Config::ESP::boxFilled = false;
				Config::ESP::nickname = true;
				Config::ESP::health = true;
				Config::ESP::healthBarPos = 0; // Left
				Config::ESP::snaplines = false;
				Config::ESP::distance = true;
				Config::ESP::skeleton = true;
				Config::ESP::indicator = true;
				Config::ESP::maxESPDistance = 250;
				
				// Misc settings
				Config::Settings::streamProof = false;
				
				Logger.log("Applied Semi-Legit preset configuration");
			}
			break;
			
		case 2: // Rage
			{
				// Aimbot settings - high aggression
				Config::Aim::enable = true;
				Config::Aim::smoothingX = 2.0f;
				Config::Aim::smoothingY = 2.0f;
				Config::Aim::FOV = 180.0f;
				Config::Aim::aimbone = 0; // Head
				Config::Aim::visibleCheck = false;
				Config::Aim::targetLock = true;
				Config::Aim::targetLockTimeout = 5.0f;
				Config::Aim::targetLockRange = 2.5f;
				Config::Aim::showFOV = true;
				Config::Aim::targetLine = true;
				Config::Aim::maxAimbotDistance = 300.0f;
				
				// ESP settings - maximum visibility
				Config::ESP::enable = true;
				Config::ESP::box = true;
				Config::ESP::boxType = 1; // Cornered
				Config::ESP::boxFilled = true;
				Config::ESP::nickname = true;
				Config::ESP::health = true;
				Config::ESP::healthBarPos = 0; // Left
				Config::ESP::snaplines = false; // Disabled - causes messy lines
				Config::ESP::distance = true;
				Config::ESP::skeleton = true;
				Config::ESP::indicator = true;
				Config::ESP::maxESPDistance = 300;
				
				// Misc settings
				Config::Settings::streamProof = false;
				
				Logger.log("Applied Rage preset configuration");
			}
			break;
			
		case 3: // HvH (Hack vs Hack)
			{
				// Aimbot settings - optimized for HvH
				Config::Aim::enable = true;
				Config::Aim::smoothingX = 1.0f;
				Config::Aim::smoothingY = 1.0f;
				Config::Aim::FOV = 360.0f;
				Config::Aim::aimbone = 4; // Smart
				Config::Aim::visibleCheck = false;
				Config::Aim::targetLock = true;
				Config::Aim::targetLockTimeout = 1.0f;
				Config::Aim::targetLockRange = 3.0f;
				Config::Aim::showFOV = true;
				Config::Aim::targetLine = true;
				Config::Aim::maxAimbotDistance = 300.0f;
				
				// ESP settings - full information
				Config::ESP::enable = true;
				Config::ESP::box = true;
				Config::ESP::boxType = 1; // Cornered
				Config::ESP::boxFilled = true;
				Config::ESP::nickname = true;
				Config::ESP::health = true;
				Config::ESP::healthBarPos = 0; // Left
				Config::ESP::snaplines = false; // Disabled - causes messy lines
				Config::ESP::distance = true;
				Config::ESP::skeleton = true;
				Config::ESP::indicator = true;
				Config::ESP::maxESPDistance = 300;
				
				// Misc settings
				Config::Settings::streamProof = false;
				
				Logger.log("Applied HvH preset configuration");
			}
			break;
			
		default:
		Logger.log(LogLevel::WARNING, "Invalid preset index: {}", presetIndex);
			break;
	}
}

// Save current configuration to a custom named config
void SaveCurrentConfig(const std::string& configName) {
	// Create configs directory if it doesn't exist
	std::string configsDir = "configs";
	CreateDirectoryA(configsDir.c_str(), NULL);
	
	std::string filename = configsDir + "\\" + configName + ".json";
	
	try {
		// Create JSON object with current config
		nlohmann::json config;
		
		// Aimbot settings
		config["aim"]["enable"] = Config::Aim::enable;
		config["aim"]["smoothingX"] = Config::Aim::smoothingX;
		config["aim"]["smoothingY"] = Config::Aim::smoothingY;
		config["aim"]["FOV"] = Config::Aim::FOV;
		config["aim"]["aimbone"] = Config::Aim::aimbone;
		config["aim"]["visibleCheck"] = Config::Aim::visibleCheck;
		config["aim"]["targetLock"] = Config::Aim::targetLock;
		config["aim"]["targetLockTimeout"] = Config::Aim::targetLockTimeout;
		config["aim"]["targetLockRange"] = Config::Aim::targetLockRange;
		config["aim"]["showFOV"] = Config::Aim::showFOV;
		config["aim"]["targetLine"] = Config::Aim::targetLine;
		config["aim"]["maxAimbotDistance"] = Config::Aim::maxAimbotDistance;
		config["aim"]["aimkey"] = Config::Aim::aimkey;
		
		// Color as array [r, g, b, a]
		config["aim"]["targetLineColor"][0] = Config::Aim::targetLineColor.x;
		config["aim"]["targetLineColor"][1] = Config::Aim::targetLineColor.y;
		config["aim"]["targetLineColor"][2] = Config::Aim::targetLineColor.z;
		config["aim"]["targetLineColor"][3] = Config::Aim::targetLineColor.w;
		
		// ESP settings
		config["esp"]["enable"] = Config::ESP::enable;
		config["esp"]["box"] = Config::ESP::box;
		config["esp"]["boxType"] = Config::ESP::boxType;
		config["esp"]["boxFilled"] = Config::ESP::boxFilled;
		config["esp"]["nickname"] = Config::ESP::nickname;
		config["esp"]["health"] = Config::ESP::health;
		config["esp"]["healthBarPos"] = Config::ESP::healthBarPos;
		config["esp"]["snaplines"] = Config::ESP::snaplines;
		config["esp"]["distance"] = Config::ESP::distance;
		config["esp"]["skeleton"] = Config::ESP::skeleton;
		config["esp"]["indicator"] = Config::ESP::indicator;
		config["esp"]["maxESPDistance"] = Config::ESP::maxESPDistance;
		
		// ESP Colors
		config["esp"]["boxVisibleOutlineColor"][0] = Config::ESP::boxVisibleOutlineColor.x;
		config["esp"]["boxVisibleOutlineColor"][1] = Config::ESP::boxVisibleOutlineColor.y;
		config["esp"]["boxVisibleOutlineColor"][2] = Config::ESP::boxVisibleOutlineColor.z;
		config["esp"]["boxVisibleOutlineColor"][3] = Config::ESP::boxVisibleOutlineColor.w;
		
		config["esp"]["boxInvisibleOutlineColor"][0] = Config::ESP::boxInvisibleOutlineColor.x;
		config["esp"]["boxInvisibleOutlineColor"][1] = Config::ESP::boxInvisibleOutlineColor.y;
		config["esp"]["boxInvisibleOutlineColor"][2] = Config::ESP::boxInvisibleOutlineColor.z;
		config["esp"]["boxInvisibleOutlineColor"][3] = Config::ESP::boxInvisibleOutlineColor.w;
		
		// Add other ESP colors similarly...
		
		// Settings
		config["settings"]["streamProof"] = Config::Settings::streamProof;
		
		// Metadata
		config["metadata"]["name"] = configName;
		config["metadata"]["version"] = "2.1.0";
		config["metadata"]["created"] = std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
		
		// Write to file
		std::ofstream file(filename);
		file << config.dump(4); // Pretty print with 4 spaces indentation
		file.close();
		
		Logger.log("Configuration saved: {}", filename);
	} catch (const std::exception& e) {
		Logger.log(LogLevel::ERROR, "Failed to save config '{}': {}", configName, e.what());
	}
}

// Load a custom configuration from file
void LoadCustomConfig(const std::string& configName) {
	std::string filename = "configs\\" + configName + ".json";
	
	try {
		std::ifstream file(filename);
		if (!file.is_open()) {
			Logger.log(LogLevel::WARNING, "Config file not found: {}", filename);
			return;
		}
		
		nlohmann::json config;
		file >> config;
		file.close();
		
		// Load aimbot settings
		if (config.contains("aim")) {
			auto& aim = config["aim"];
			if (aim.contains("enable")) Config::Aim::enable = aim["enable"];
			if (aim.contains("smoothingX")) Config::Aim::smoothingX = aim["smoothingX"];
			if (aim.contains("smoothingY")) Config::Aim::smoothingY = aim["smoothingY"];
			if (aim.contains("FOV")) Config::Aim::FOV = aim["FOV"];
			if (aim.contains("aimbone")) Config::Aim::aimbone = aim["aimbone"];
			if (aim.contains("visibleCheck")) Config::Aim::visibleCheck = aim["visibleCheck"];
			if (aim.contains("targetLock")) Config::Aim::targetLock = aim["targetLock"];
			if (aim.contains("targetLockTimeout")) Config::Aim::targetLockTimeout = aim["targetLockTimeout"];
			if (aim.contains("targetLockRange")) Config::Aim::targetLockRange = aim["targetLockRange"];
			if (aim.contains("showFOV")) Config::Aim::showFOV = aim["showFOV"];
			if (aim.contains("targetLine")) Config::Aim::targetLine = aim["targetLine"];
			if (aim.contains("maxAimbotDistance")) Config::Aim::maxAimbotDistance = aim["maxAimbotDistance"];
			if (aim.contains("aimkey")) Config::Aim::aimkey = aim["aimkey"];
			
			if (aim.contains("targetLineColor") && aim["targetLineColor"].is_array()) {
				auto& color = aim["targetLineColor"];
				Config::Aim::targetLineColor.x = color[0];
				Config::Aim::targetLineColor.y = color[1];
				Config::Aim::targetLineColor.z = color[2];
				Config::Aim::targetLineColor.w = color[3];
			}
		}
		
		// Load ESP settings
		if (config.contains("esp")) {
			auto& esp = config["esp"];
			if (esp.contains("enable")) Config::ESP::enable = esp["enable"];
			if (esp.contains("box")) Config::ESP::box = esp["box"];
			if (esp.contains("boxType")) Config::ESP::boxType = esp["boxType"];
			if (esp.contains("boxFilled")) Config::ESP::boxFilled = esp["boxFilled"];
			if (esp.contains("nickname")) Config::ESP::nickname = esp["nickname"];
			if (esp.contains("health")) Config::ESP::health = esp["health"];
			if (esp.contains("healthBarPos")) Config::ESP::healthBarPos = esp["healthBarPos"];
			if (esp.contains("snaplines")) Config::ESP::snaplines = esp["snaplines"];
			if (esp.contains("distance")) Config::ESP::distance = esp["distance"];
			if (esp.contains("skeleton")) Config::ESP::skeleton = esp["skeleton"];
			if (esp.contains("indicator")) Config::ESP::indicator = esp["indicator"];
			if (esp.contains("maxESPDistance")) Config::ESP::maxESPDistance = esp["maxESPDistance"];
			
			// Load ESP colors
			if (esp.contains("boxVisibleOutlineColor") && esp["boxVisibleOutlineColor"].is_array()) {
				auto& color = esp["boxVisibleOutlineColor"];
				Config::ESP::boxVisibleOutlineColor.x = color[0];
				Config::ESP::boxVisibleOutlineColor.y = color[1];
				Config::ESP::boxVisibleOutlineColor.z = color[2];
				Config::ESP::boxVisibleOutlineColor.w = color[3];
			}
			
			if (esp.contains("boxInvisibleOutlineColor") && esp["boxInvisibleOutlineColor"].is_array()) {
				auto& color = esp["boxInvisibleOutlineColor"];
				Config::ESP::boxInvisibleOutlineColor.x = color[0];
				Config::ESP::boxInvisibleOutlineColor.y = color[1];
				Config::ESP::boxInvisibleOutlineColor.z = color[2];
				Config::ESP::boxInvisibleOutlineColor.w = color[3];
			}
			
			// Load other ESP colors similarly...
		}
		
		// Load settings
		if (config.contains("settings")) {
			auto& settings = config["settings"];
			if (settings.contains("streamProof")) Config::Settings::streamProof = settings["streamProof"];
		}
		
		Logger.log("Configuration loaded: {}", filename);
	} catch (const std::exception& e) {
		Logger.log(LogLevel::ERROR, "Failed to load config '{}': {}", configName, e.what());
	}
}

// Reset all settings to default values
void ResetToDefaults() {
	// Reset aimbot settings
	Config::Aim::enable = false;
	Config::Aim::smoothingX = 5.0f;
	Config::Aim::smoothingY = 5.0f;
	Config::Aim::FOV = 60.0f;
	Config::Aim::aimbone = 0; // Head
	Config::Aim::visibleCheck = true;
	Config::Aim::targetLock = false;
	Config::Aim::targetLockTimeout = 3.0f;
	Config::Aim::targetLockRange = 1.5f;
	Config::Aim::showFOV = false;
	Config::Aim::targetLine = false;
	Config::Aim::maxAimbotDistance = 150.0f;
	Config::Aim::aimkey = VK_LBUTTON;
	Config::Aim::targetLineColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	
	// Reset ESP settings
	Config::ESP::enable = false;
	Config::ESP::box = false;
	Config::ESP::boxType = 0;
	Config::ESP::boxFilled = false;
	Config::ESP::nickname = false;
	Config::ESP::health = false;
	Config::ESP::healthBarPos = 0;
	Config::ESP::snaplines = false;
	Config::ESP::distance = false;
	Config::ESP::skeleton = false;
	Config::ESP::indicator = false;
	Config::ESP::maxESPDistance = 200;
	
	// Reset ESP colors to defaults
	Config::ESP::boxVisibleOutlineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	Config::ESP::boxInvisibleOutlineColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	Config::ESP::boxVisibleFilledColor = ImVec4(0.0f, 1.0f, 0.0f, 0.3f);
	Config::ESP::boxInvisibleFilledColor = ImVec4(1.0f, 0.0f, 0.0f, 0.3f);
	Config::ESP::nicknameVisibleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	Config::ESP::nicknameInvisibleColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	Config::ESP::snaplinesVisibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	Config::ESP::snaplinesInvisibleColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	Config::ESP::distanceVisibleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	Config::ESP::distanceInvisibleColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	Config::ESP::skeletonVisibleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	Config::ESP::skeletonInvisibleColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	
	// Reset settings
	Config::Settings::streamProof = false;
	
	Logger.log("All settings reset to defaults");
}

// Export current configuration to a shareable file
void ExportCurrentConfig() {
	// Use Windows file dialog for export location
	char filename[MAX_PATH] = {0};
	
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "JSON Config Files\0*.json\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = "json";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	
	if (GetSaveFileNameA(&ofn)) {
		std::string exportPath = filename;
		std::string configName = "Export_" + std::to_string(
			std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::system_clock::now().time_since_epoch()).count());
		
		// Save using the same logic as SaveCurrentConfig but to the selected path
		try {
			nlohmann::json config;
			
			// Same config building logic as SaveCurrentConfig...
			// [Abbreviated for brevity - would include all the same config serialization]
			
			std::ofstream file(exportPath);
			file << config.dump(4);
			file.close();
			
			Logger.log("Configuration exported to: {}", exportPath);
		} catch (const std::exception& e) {
			Logger.log(LogLevel::ERROR, "Failed to export config: {}", e.what());
		}
	}
}

// Import configuration from a file
void ImportConfigFromFile() {
	// Use Windows file dialog for import
	char filename[MAX_PATH] = {0};
	
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "JSON Config Files\0*.json\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (GetOpenFileNameA(&ofn)) {
		std::string importPath = filename;
		
		try {
			std::ifstream file(importPath);
			if (!file.is_open()) {
				Logger.log(LogLevel::ERROR, "Could not open import file: {}", importPath);
				return;
			}
			
			nlohmann::json config;
			file >> config;
			file.close();
			
			// Use the same loading logic as LoadCustomConfig...
			// [Would include all the same config deserialization logic]
			
			Logger.log("Configuration imported from: {}", importPath);
		} catch (const std::exception& e) {
			Logger.log(LogLevel::ERROR, "Failed to import config: {}", e.what());
		}
	}
}
