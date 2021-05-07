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

    void Renderer::RenderSessionInfo(CanvasWrapper* canvas, StatTracker* tracker)
    {
        if (canvas == nullptr ||
            tracker == nullptr)
        {
            return;
        }

        Vector2 position =
        {
            *posX,
            *posY
        };
        int count = 0;
        for (const RankedPlaylist playlist : AvailablePlaylists)
        {
            Stats* stats = tracker->stats[playlist];
            if (stats->wins == 0 &&
                stats->losses == 0)
            {
                continue;
            }
            this->RenderBox(canvas, stats, position, GetPlaylistName(playlist));
            if (count++ % 2 == 0)
            {
                position.X += 240;
            }
            else
            {
                position.X = *posX;
                position.Y += 128;
            }
        }
    }

    void Renderer::RenderAfterGame(CanvasWrapper* canvas, GameResultViewer* viewer, const RankedPlaylist playlist)
    {
        if (canvas == nullptr ||
            viewer == nullptr)
        {
            return;
        }

        Vector2 screen = canvas->GetSize();
        float fontSize = (float)screen.X / (float)1920;

        if (!viewer->IsPlacement(playlist))
        {
            // 1-1
            canvas->SetColor(233, 238, 240, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .2) , int(screen.Y * .7) });
            canvas->DrawString("Next: ", 2 * fontSize, 2 * fontSize);

            // 1-2
            canvas->SetColor(viewer->colorNext->R, viewer->colorNext->G, viewer->colorNext->B, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .24) , int(screen.Y * .7) });
            canvas->DrawString(viewer->tierNext + ": ", 2 * fontSize, 2 * fontSize);

            // 1-3
            canvas->SetColor(233, 238, 240, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .37) , int(screen.Y * .7) });
            canvas->DrawString(std::to_string(viewer->ratingNext) + viewer->differenceNext, 2 * fontSize, 2 * fontSize);
        }
        else
        {
            // Placement Warning
            canvas->SetColor(viewer->colorCurrent->R, viewer->colorCurrent->G, viewer->colorCurrent->B, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .18) , int(screen.Y * .7) });
            canvas->DrawString("Finish placements for full functionality!", 2 * fontSize, 2 * fontSize);
        }

        // 2-1
        canvas->SetColor(233, 238, 240, 255);
        canvas->SetPosition(Vector2{ int(screen.X * .18) , int(screen.Y * .735) });
        canvas->DrawString("Current: ", 2 * fontSize, 2 * fontSize);

        // 2-2
        canvas->SetColor(viewer->colorCurrent->R, viewer->colorCurrent->G, viewer->colorCurrent->B, 255);
        canvas->SetPosition(Vector2{ int(screen.X * .24) , int(screen.Y * .735) });
        canvas->DrawString(viewer->tierCurrent + ": ", 2 * fontSize, 2 * fontSize);

        // 2-3
        canvas->SetColor(233, 238, 240, 255);
        canvas->SetPosition(Vector2{ int(screen.X * .37) , int(screen.Y * .735) });
        canvas->DrawString(std::to_string(viewer->ratingCurrent), 2 * fontSize, 2 * fontSize);

        if (!viewer->IsPlacement(playlist))
        {
            // 3-1
            canvas->SetColor(233, 238, 240, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .175) , int(screen.Y * .77) });
            canvas->DrawString("Previous: ", 2 * fontSize, 2 * fontSize);

            // 3-2
            canvas->SetColor(viewer->colorPrevious->R, viewer->colorPrevious->G, viewer->colorPrevious->B, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .24) , int(screen.Y * .77) });
            canvas->DrawString(viewer->tierPrevious + ": ", 2 * fontSize, 2 * fontSize);

            // 3-3
            canvas->SetColor(233, 238, 240, 255);
            canvas->SetPosition(Vector2{ int(screen.X * .37) , int(screen.Y * .77) });
            canvas->DrawString(std::to_string(viewer->ratingPrevious) + viewer->differencePrevious, 2 * fontSize, 2 * fontSize);
        }
    }

    //
    // Private
    //

    void Renderer::SetColorByValue(CanvasWrapper* canvas, const float value)
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

    void Renderer::RenderBox(CanvasWrapper* canvas, Stats* stats, const Vector2 position, const std::string playlist)
    {
        Vector2 boxSize = { 240, 128 };

        // DRAW BOX
        canvas->SetColor(*this->colorBackground);
        canvas->SetPosition(position);
        canvas->FillBox(boxSize);

        // DRAW CURRENT PLAYLIST
        std::stringstream stringStream;

        canvas->SetColor(*this->colorTitle);
        canvas->SetPosition(Vector2{ position.X + 42, position.Y + 5 });

        stringStream << playlist;
        canvas->DrawString(stringStream.str());

        // DRAW LAST GAME MMR GAIN
        canvas->SetColor(*this->colorLabel);
        canvas->SetPosition(Vector2{ position.X + 10, position.Y + 21 });
        canvas->DrawString("Current MMR:");

        float mmrGain = stats->rating->GetLastGameDifference();
        SetColorByValue(canvas, mmrGain);
        canvas->SetPosition(Vector2{ position.X + 104, position.Y + 21 });

        stringStream.str("");
        stats->FormatStream(stringStream, StreamFormatStyle::LastGameDifference);
        canvas->DrawString(stringStream.str());
        // DRAW SESSION MMR GAIN
        canvas->SetColor(*this->colorLabel);
        canvas->SetPosition(Vector2{ position.X + 10, position.Y + 37 });
        canvas->DrawString("Start MMR:");

        mmrGain = stats->rating->GetSessionDifference();
        SetColorByValue(canvas, mmrGain);
        canvas->SetPosition(Vector2{ position.X + 104, position.Y + 37 });

        stringStream.str("");
        stats->FormatStream(stringStream, StreamFormatStyle::InitialDifference);
        canvas->DrawString(stringStream.str());

        // DRAW WINS
        canvas->SetColor(*this->colorLabel);
        canvas->SetPosition(Vector2{ position.X + 10, position.Y + 53 });
        canvas->DrawString("Wins:");

        canvas->SetColor(*this->colorPositive);
        canvas->SetPosition(Vector2{ position.X + 104, position.Y + 53 });
        canvas->DrawString(std::to_string(stats->wins));

        // DRAW LOSSES
        canvas->SetColor(*this->colorLabel);
        canvas->SetPosition(Vector2{ position.X + 10, position.Y + 69 });
        canvas->DrawString("Losses:");

        canvas->SetColor(*this->colorNegative);
        canvas->SetPosition(Vector2{ position.X + 104, position.Y + 69 });
        canvas->DrawString(std::to_string(stats->losses));

        // DRAW STREAK
        canvas->SetColor(*this->colorLabel);
        canvas->SetPosition(Vector2{ position.X + 10, position.Y + 85 });
        canvas->DrawString("Streak:");

        int streak = stats->streak;
        SetColorByValue(canvas, static_cast<float>(streak));
        canvas->SetPosition(Vector2{ position.X + 104, position.Y + 85 });
        stringStream.str("");
        stringStream << abs(streak);
        canvas->DrawString(stringStream.str());
    }
}