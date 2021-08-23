// Fill out your copyright notice in the Description page of Project Settings.


#include "LineHashGrid.h"

/*template <typename LineDataType>
TLineHashGrid<LineDataType>::TLineHashGrid()
{
}

template <typename LineDataType>
TLineHashGrid<LineDataType>::~TLineHashGrid()
{
}

template <typename LineDataType>
void TLineHashGrid<LineDataType>::Bresenham(const FVector2i& Start, const FVector2i& End, TArray<FVector2i>& Points) const
{
    Points.Reset();

    FVector2i Shift, StartShifted, EndShifted;
    int min = 0.0;

    if (min = FMath::Min(Start.X, End.X) < 0) Shift.X = -min;
    if (min = FMath::Min(Start.Y, End.Y) < 0) Shift.Y = -min;
    StartShifted = Start + Shift;
    EndShifted = End + Shift;

    int Width = EndShifted.X - StartShifted.X;
    int Height = EndShifted.Y - StartShifted.Y;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

    if (Width < 0) dx1 = -1; else if (Width > 0) dx1 = 1;
    if (Height < 0) dy1 = -1; else if (Height > 0) dy1 = 1;
    if (Width < 0) dx2 = -1; else if (Width > 0) dx2 = 1;

    int Longest = FMath::Abs(Width);
    int Shortest = FMath::Abs(Height);

    if (Longest <= Shortest)
    {
        Longest = FMath::Abs(Height);
        Shortest = FMath::Abs(Width);
        if (Height < 0) dy2 = -1; else if (Height > 0) dy2 = 1;
        dx2 = 0;
    }

    int numerator = Longest >> 1;
    for (int i = 0, CurX = StartShifted.X, CurY = StartShifted.Y; i <= Longest; i++)
    {
        Points.Emplace(FVector2i(CurX, CurY) - Shift);

        numerator += Shortest;

        if (numerator >= Longest)
        {
            numerator -= Longest;
            CurX += dx1;
            CurY += dy1;
        }
        else
        {
            CurX += dx2;
            CurY += dy2;
        }
    }
}*/