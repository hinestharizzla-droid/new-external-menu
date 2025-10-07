#pragma once
#include <Windows.h>

namespace Menu {
	// Main rendering function - call this in your main loop
	void RenderMenu();
	
	// Initialize menu (optional - for any startup configuration)
	void Initialize();
	
	// Cleanup (optional - for any cleanup needed)
	void Cleanup();
}
