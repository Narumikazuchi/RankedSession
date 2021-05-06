
#include <sstream>
#include "bakkesmod/wrappers/cvarmanagerwrapper.h"
#include "bakkesmod/wrappers/canvaswrapper.h"
#include "bakkesmod/wrappers/MMRWrapper.h"
#include "Renderer.h"

namespace RankedSession
{
    Renderer::Renderer()
    {
        this->posX = std::make_shared<int>(0);
        this->posY = std::make_shared<int>(0);
        this->colorBackground = std::make_shared<LinearColor>(LinearColor());
        this->colorTitle = std::make_shared<LinearColor>(LinearColor());
        this->colorLabel = std::make_shared<LinearColor>(LinearColor());
        this->colorPositive = std::make_shared<LinearColor>(LinearColor());
        this->colorNegative = std::make_shared<LinearColor>(LinearColor());
    }

    void Renderer::RenderSessionInfo(CanvasWrapper* canvas, StatTracker& tracker, RankedPlaylist playlist)
    {

    }

    void Renderer::RenderAfterGame(CanvasWrapper* canvas, GameResultViewer& viewer, RankedPlaylist playlist)
    {

    }

    void Renderer::SetColorByValue(CanvasWrapper* canvas, float value)
    {
        if (value > 0.0f)
        {
            canvas->SetColor(*this->colorPositive);
        }
        else if (value < 0.0f)
        {
            canvas->SetColor(*this->colorNegative);
        }
        else
        {
            canvas->SetColor(*this->colorLabel);
        }
    }
}