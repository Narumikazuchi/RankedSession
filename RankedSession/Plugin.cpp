#include "Logger.h"
#include "Plugin.h"

#define CVAR_NAME_COLOR_BACKGROUND "cl_rankedsession_plugin_background_color"
#define CVAR_NAME_COLOR_TITLE "cl_rankedsession_plugin_title_color"
#define CVAR_NAME_COLOR_LABEL "cl_rankedsession_plugin_label_color"
#define CVAR_NAME_COLOR_POSITIVE "cl_rankedsession_plugin_positive_color"
#define CVAR_NAME_COLOR_NEGATIVE "cl_rankedsession_plugin_negative_color"
#define CVAR_NAME_RESET_COLORS "cl_rankedsession_plugin_reset_colors"

#define HOOK_COUNTDOWN_BEGINSTATE "Function GameEvent_TA.Countdown.BeginState"
#define HOOK_ON_WINNER_SET "Function TAGame.GameEvent_Soccar_TA.EventMatchWinnerSet"
#define HOOK_DESTROYED "Function TAGame.GameEvent_Soccar_TA.Destroyed"

BAKKESMOD_PLUGIN(RankedSession::Plugin, "Rank Session Tracker", "1.0", 0)

namespace RankedSession
{
	Plugin::Plugin()
	{
		this->renderer = nullptr;
		this->resultViewer = nullptr;
		this->statTracker = nullptr;
		this->drawStats = false;
		this->drawResults = false;
	}

	void Plugin::ResetColors()
	{
		this->renderer->colorBackground->R = 0.f;
		this->renderer->colorBackground->G = 0.f;
		this->renderer->colorBackground->B = 0.f;
		this->renderer->colorBackground->A = 160.f;
		this->cvarManager->getCvar(CVAR_NAME_COLOR_BACKGROUND).setValue(*this->renderer->colorBackground);

		this->renderer->colorTitle->R = 255.f;
		this->renderer->colorTitle->G = 255.f;
		this->renderer->colorTitle->B = 255.f;
		this->renderer->colorTitle->A = 224.f;
		this->cvarManager->getCvar(CVAR_NAME_COLOR_TITLE).setValue(*this->renderer->colorTitle);

		this->renderer->colorLabel->R = 255.f;
		this->renderer->colorLabel->G = 255.f;
		this->renderer->colorLabel->B = 255.f;
		this->renderer->colorLabel->A = 224.f;
		this->cvarManager->getCvar(CVAR_NAME_COLOR_LABEL).setValue(*this->renderer->colorLabel);

		this->renderer->colorPositive->R = 95.f;
		this->renderer->colorPositive->G = 232.f;
		this->renderer->colorPositive->B = 95.f;
		this->renderer->colorPositive->A = 224.f;
		this->cvarManager->getCvar(CVAR_NAME_COLOR_POSITIVE).setValue(*this->renderer->colorPositive);

		this->renderer->colorNegative->R = 232.f;
		this->renderer->colorNegative->G = 95.f;
		this->renderer->colorNegative->B = 95.f;
		this->renderer->colorNegative->A = 224.f;
		this->cvarManager->getCvar(CVAR_NAME_COLOR_NEGATIVE).setValue(*this->renderer->colorNegative);
	}

	void Plugin::onLoad()
	{
		RankedSession::manager = this->cvarManager;

		this->renderer = std::make_shared<Renderer>();
		this->renderer->PreLoadImages(this->gameWrapper.get());

		this->cvarManager->registerCvar(CVAR_NAME_COLOR_BACKGROUND, "(0, 0, 0, 160)", "Background color", false, false, 0, false, 255, true).bindTo(this->renderer->colorBackground);
		this->cvarManager->registerCvar(CVAR_NAME_COLOR_TITLE, "(255, 255, 255, 224)", "Title color", false, false, 0, false, 255, true).bindTo(this->renderer->colorTitle);
		this->cvarManager->registerCvar(CVAR_NAME_COLOR_LABEL, "(255, 255, 255, 224)", "Label color", false, false, 0, false, 255, true).bindTo(this->renderer->colorLabel);
		this->cvarManager->registerCvar(CVAR_NAME_COLOR_POSITIVE, "(95, 232, 95, 224)", "Positive color", false, false, 0, false, 255, true).bindTo(this->renderer->colorPositive);
		this->cvarManager->registerCvar(CVAR_NAME_COLOR_NEGATIVE, "(232, 95, 95, 224)", "Negative color", false, false, 0, false, 255, true).bindTo(this->renderer->colorNegative);

		this->cvarManager->registerNotifier(CVAR_NAME_RESET_COLORS, [this](std::vector<std::string> params)
		{
			ResetColors();
		}, "Reset to default colors", PERMISSION_ALL);


		this->gameWrapper->HookEventPost(HOOK_COUNTDOWN_BEGINSTATE, std::bind(&Plugin::Event_GameBegin, this, std::placeholders::_1));
		this->gameWrapper->HookEventPost(HOOK_ON_WINNER_SET, std::bind(&Plugin::Event_GameConcluded, this, std::placeholders::_1));
		this->gameWrapper->HookEventPost(HOOK_DESTROYED, std::bind(&Plugin::Event_GameLeft, this, std::placeholders::_1));

		this->gameWrapper->RegisterDrawable(std::bind(&Plugin::Render, this, std::placeholders::_1));

		this->resultViewer = std::make_shared<GameResultViewer>(this->gameWrapper);
		this->statTracker = std::make_shared<StatTracker>(this->gameWrapper);
		this->SyncTracker(15);
	}

	void Plugin::onUnload()
	{
		RankedSession::manager = nullptr;

		this->gameWrapper->UnregisterDrawables();
		this->gameWrapper->UnhookEventPost(HOOK_COUNTDOWN_BEGINSTATE);
		this->gameWrapper->UnhookEventPost(HOOK_ON_WINNER_SET);
		this->gameWrapper->UnhookEventPost(HOOK_DESTROYED);

		delete this->renderer.get();
		delete this->resultViewer.get();
		delete this->statTracker.get();

		this->renderer = nullptr;
		this->resultViewer = nullptr;
		this->statTracker = nullptr;
		this->drawStats = false;
		this->drawResults = false;
	}

	void Plugin::Render(CanvasWrapper canvas)
	{
		MMRWrapper mmr = this->gameWrapper->GetMMRWrapper();
		RankedPlaylist playlist = (RankedPlaylist)mmr.GetCurrentPlaylist();
		if (this->drawStats)
		{
			this->renderer->RenderSessionInfo(&canvas, this->statTracker.get());
		}
		if (this->drawResults)
		{
			this->renderer->RenderAfterGame(&canvas, this->resultViewer.get(), playlist);
		}
	}

	void Plugin::Event_GameBegin(std::string eventName)
	{
		this->drawStats = false;
		this->drawResults = false;
	}

	void Plugin::Event_GameConcluded(std::string eventName)
	{
		if (!this->resultViewer->isInitialized)
		{
			this->resultViewer = std::make_shared<GameResultViewer>(this->gameWrapper);
		}
		if (!this->statTracker->isInitialized)
		{
			this->statTracker = std::make_shared<StatTracker>(this->gameWrapper);
			this->SyncTracker(15);
		}

		MMRWrapper mmr = this->gameWrapper->GetMMRWrapper();
		RankedPlaylist playlist = (RankedPlaylist)mmr.GetCurrentPlaylist();
		this->UpdateTrackers(5, playlist);
	}

	void Plugin::Event_GameLeft(std::string eventName)
	{
		this->drawResults = false;
	}

	//
	// Private
	//

	void Plugin::SyncTracker(const int retries)
	{
		this->gameWrapper->SetTimeout([retries, this](GameWrapper* gameWrapper)
		{
			RatingUpdateResult statResult = this->statTracker->Sync();
			if (statResult == RatingUpdateResult::NOT_SYNCED)
			{
				gameWrapper->SetTimeout([retries, this](GameWrapper* wrapper)
				{
					this->SyncTracker(retries - 1);
				}, 0.5f);
			}
		}, 3);
	}

	void Plugin::UpdateTrackers(const int retries, const RankedPlaylist playlist)
	{
		this->gameWrapper->SetTimeout([retries, playlist, this](GameWrapper* gameWrapper)
		{
			RatingUpdateResult gameResult = this->resultViewer->Update(playlist);
			RatingUpdateResult statResult = this->statTracker->Update(playlist);
			if (gameResult == RatingUpdateResult::NOT_SYNCED ||
				statResult == RatingUpdateResult::NOT_SYNCED)
			{
				gameWrapper->SetTimeout([retries, playlist, this](GameWrapper* wrapper)
				{
					this->UpdateTrackers(retries - 1, playlist);
				}, 0.5f);
				return;
			}
			this->drawResults = true;
			this->drawStats = true;
		}, 3);
	}
}