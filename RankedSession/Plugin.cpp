#include "Plugin.h"

#define CVAR_NAME_COLOR_BACKGROUND "cl_session_plugin_background_color"
#define CVAR_NAME_COLOR_TITLE "cl_session_plugin_title_color"
#define CVAR_NAME_COLOR_LABEL "cl_session_plugin_label_color"
#define CVAR_NAME_COLOR_POSITIVE "cl_session_plugin_positive_color"
#define CVAR_NAME_COLOR_NEGATIVE "cl_session_plugin_negative_color"
#define CVAR_NAME_RESET_COLORS "cl_session_plugin_reset_colors"

#define HOOK_COUNTDOWN_BEGINSTATE "Function GameEvent_TA.Countdown.BeginState"
#define HOOK_ON_WINNER_SET "Function TAGame.GameEvent_Soccar_TA.EventMatchWinnerSet"
#define HOOK_DESTROYED "Function TAGame.GameEvent_Soccar_TA.Destroyed"

namespace RankedSession
{
	Plugin::Plugin()
	{
		GameResultViewer temp_g = GameResultViewer(this->gameWrapper.get());
		StatTracker temp_s = StatTracker(this->gameWrapper.get());

		this->renderer = Renderer();
		this->resultViewer = &temp_g;
		this->statTracker = &temp_s;
		this->drawStats = false;
		this->drawResults = false;
	}

	void Plugin::ResetColors()
	{

		renderer.colorBackground->R = 0.f;
		renderer.colorBackground->G = 0.f;
		renderer.colorBackground->B = 0.f;
		renderer.colorBackground->A = 75.f;
		cvarManager->getCvar(CVAR_NAME_COLOR_BACKGROUND).setValue(*renderer.colorBackground);

		renderer.colorTitle->R = 255.f;
		renderer.colorTitle->G = 255.f;
		renderer.colorTitle->B = 255.f;
		renderer.colorTitle->A = 127.f;
		cvarManager->getCvar(CVAR_NAME_COLOR_TITLE).setValue(*renderer.colorTitle);

		renderer.colorLabel->R = 255.f;
		renderer.colorLabel->G = 255.f;
		renderer.colorLabel->B = 255.f;
		renderer.colorLabel->A = 150.f;
		cvarManager->getCvar(CVAR_NAME_COLOR_LABEL).setValue(*renderer.colorLabel);

		renderer.colorPositive->R = 95.f;
		renderer.colorPositive->G = 232.f;
		renderer.colorPositive->B = 95.f;
		renderer.colorPositive->A = 127.f;
		cvarManager->getCvar(CVAR_NAME_COLOR_POSITIVE).setValue(*renderer.colorPositive);

		renderer.colorNegative->R = 232.f;
		renderer.colorNegative->G = 95.f;
		renderer.colorNegative->B = 95.f;
		renderer.colorNegative->A = 127.f;
		cvarManager->getCvar(CVAR_NAME_COLOR_NEGATIVE).setValue(*renderer.colorNegative);
	}

	void Plugin::onLoad()
	{
		cvarManager->registerCvar(CVAR_NAME_COLOR_BACKGROUND, "(0, 0, 0, 75)", "Background color", false, false, 0, false, 255, true).bindTo(renderer.colorBackground);
		cvarManager->registerCvar(CVAR_NAME_COLOR_TITLE, "(255, 255, 255, 127)", "Title color", false, false, 0, false, 255, true).bindTo(renderer.colorTitle);
		cvarManager->registerCvar(CVAR_NAME_COLOR_LABEL, "(255, 255, 255, 150)", "Label color", false, false, 0, false, 255, true).bindTo(renderer.colorLabel);
		cvarManager->registerCvar(CVAR_NAME_COLOR_POSITIVE, "(95, 232, 95, 127)", "Positive color", false, false, 0, false, 255, true).bindTo(renderer.colorPositive);
		cvarManager->registerCvar(CVAR_NAME_COLOR_NEGATIVE, "(232, 95, 95, 127)", "Negative color", false, false, 0, false, 255, true).bindTo(renderer.colorNegative);

		cvarManager->registerNotifier(CVAR_NAME_RESET_COLORS, [this](std::vector<std::string> params) 
		{
			ResetColors();
		}, "Reset to default colors", PERMISSION_ALL);

		gameWrapper->HookEventPost(HOOK_COUNTDOWN_BEGINSTATE, std::bind(&Plugin::Event_GameBegin, this, std::placeholders::_1));
		gameWrapper->HookEventPost(HOOK_ON_WINNER_SET, std::bind(&Plugin::Event_GameConcluded, this, std::placeholders::_1));
		gameWrapper->HookEventPost(HOOK_DESTROYED, std::bind(&Plugin::Event_GameLeft, this, std::placeholders::_1));

		gameWrapper->RegisterDrawable(std::bind(&Plugin::Render, this, std::placeholders::_1));
	}

	void Plugin::onUnload()
	{
		gameWrapper->UnregisterDrawables();
		gameWrapper->UnhookEventPost(HOOK_COUNTDOWN_BEGINSTATE);
		gameWrapper->UnhookEventPost(HOOK_ON_WINNER_SET);
		gameWrapper->UnhookEventPost(HOOK_DESTROYED);
	}

	void Plugin::Render(CanvasWrapper canvas)
	{
		MMRWrapper mmr = this->gameWrapper->GetMMRWrapper();
		RankedPlaylist playlist = (RankedPlaylist)mmr.GetCurrentPlaylist();
		if (this->drawStats)
		{
			this->renderer.RenderSessionInfo(&canvas, this->statTracker);
		}
		if (this->drawResults)
		{
			this->renderer.RenderAfterGame(&canvas, this->resultViewer, playlist);
		}
	}

	void Plugin::Event_GameBegin(std::string eventName)
	{
		this->drawStats = false;
	}

	void Plugin::Event_GameConcluded(std::string eventName)
	{
		MMRWrapper mmr = this->gameWrapper->GetMMRWrapper();
		RankedPlaylist playlist = (RankedPlaylist)mmr.GetCurrentPlaylist();
		this->resultViewer->Update(playlist);
		this->drawResults = true;
		this->statTracker->Update(this->gameWrapper.get(), playlist);
		this->drawStats = true;
	}

	void Plugin::Event_GameLeft(std::string eventName)
	{
		this->drawResults = false;
	}
}