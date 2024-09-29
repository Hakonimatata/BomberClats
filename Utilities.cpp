#include "Utilities.h"
#include <math.h>

void FloatPoint::normalize()
{
    double magnitude = getSize();
    if (magnitude != 0) {
        x = x / magnitude;
        y = y / magnitude;
    }
}

float FloatPoint::getSize()
{
    return sqrt(pow(x,2) + pow(y,2));
}

float getDistanceBetween(FloatPoint a, FloatPoint b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

FloatPoint getMidPointBetween(FloatPoint a, FloatPoint b)
{
    FloatPoint midPoint;
    midPoint.x = (a.x + b.x) / 2;
    midPoint.y = (a.y + b.y) / 2;
    return midPoint;
}


// Funksjon for å sjekke om det er en tile mellom to punkter
// Denne fungerer nesten, men gjør at man er udødelig inntill en vegg
bool IsTileBetweenPoints(Vector2 start, Vector2 end, vector<vector<Tile>>& tileMap, int tileSize, float gridShiftX, float gridShiftY) 
{
    // Juster start- og sluttkoordinatene for grid shift før deling med tileSize
    int x0 = (int)((start.x - gridShiftX) / tileSize);
    int y0 = (int)((start.y - gridShiftY) / tileSize);
    int x1 = (int)((end.x - gridShiftX) / tileSize);
    int y1 = (int)((end.y - gridShiftY) / tileSize);

    // Sjekk at start- og sluttkoordinatene er innenfor gyldige grenser
    int mapHeight = tileMap.size();
    int mapWidth = tileMap[0].size();

    if (x0 < 0 || x0 >= mapWidth || y0 < 0 || y0 >= mapHeight || 
        x1 < 0 || x1 >= mapWidth || y1 < 0 || y1 >= mapHeight) 
    {
        return false; // Ugyldige koordinater
    }

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1; 
    int err = dx - dy;

    const int interpolationSteps = 5; // Antall sjekkpunkter mellom to tile-punkter

    while (true) 
    {
        // Sjekk tile flere ganger for mer nøyaktighet
        for (int i = 0; i <= interpolationSteps; ++i) 
        {
            float interpolateX = x0 + (sx * i * (dx ? 1.0f / interpolationSteps : 0));
            float interpolateY = y0 + (sy * i * (dy ? 1.0f / interpolationSteps : 0));

            // Gjør tilnærmingen til heltall
            int checkX = static_cast<int>(interpolateX);
            int checkY = static_cast<int>(interpolateY);

            // Sjekk om tile er innenfor gyldige grenser
            if (checkX >= 0 && checkX < mapWidth && checkY >= 0 && checkY < mapHeight) 
            {
                if (tileMap[checkY][checkX].id != -1) 
                {
                    return true; // Fant en tile på veien
                }
            }
        }

        // Hvis vi har nådd sluttpunktet, avslutt loopen
        if (x0 == x1 && y0 == y1) 
        {
            break;
        }

        // Beregn neste punkt på linjen
        int e2 = 2 * err;
        if (e2 > -dy) 
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) 
        {
            err += dx;
            y0 += sy;
        }
    }

    return false; // Ingen tiles funnet langs linjen
}


