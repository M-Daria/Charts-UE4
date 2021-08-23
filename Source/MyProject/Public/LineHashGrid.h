// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util/GridIndexing2.h"

struct FLine
{
	FVector2d LineStart;
	FVector2d LineEnd;
};

/*/struct FLine
{
	FVector2d LineStart;
	FVector2d LineEnd;
};*/

/*FORCEINLINE FArchive& operator<<(FArchive& Ar, FVector2i& Vector)
{
	Ar << Vector.X;
	Ar << Vector.Y;
	return Ar;
}*/

/**
 * 
 */
template<typename LineDataType>
class MYPROJECT_API TLineHashGrid
{
private:
	TMap<int64, int64> HashForFile;
	FString FileName;
	TMap<int64, TArray<LineDataType>> Hash;
	FScaleGridIndexer2d Indexer;
	FBox2D BoundingBox;
	int64 NumOfObj;

public:
	//~TLineHashGrid();

	//TLineHashGrid() {}
	TLineHashGrid() : Indexer(1.0), NumOfObj(0) {}
	TLineHashGrid(double CellSize, FBox2D BB) : Indexer(CellSize), BoundingBox(BB), NumOfObj(0) {}

	void SetFileInfo(const TMap<int64, int64>& FileHash, FString File)
	{
		HashForFile = FileHash;
		FileName = File;
	}

	void SetCellSize(double CellSize)
	{
		Indexer.CellSize = CellSize;
	}

	int64 GetCountObjects()
	{
		return NumOfObj;
	}

	double GetCellSize() const
	{
		return Indexer.CellSize;
	}

	void Clear()
	{
		NumOfObj = 0;
		Hash.Reset();
	}

	int64 SerializeCountsOnly(FArchive& Ar)
	{
		int64 counter = 0;
		Hash.KeySort(TLess<int64>());

		for (auto& Pair : Hash)
		{
			int64 tmp(Pair.Key);
			int64 tmp2(Pair.Value.Num());

			Ar << tmp << tmp2;

			counter++;
		}

		return counter;
	}

	void Serialize(FArchive& Ar)
	{
		int64 counter = 0;
		Hash.KeySort(TLess<int64>());

		for (auto& Pair : Hash)
		{
			int64 tmp(Pair.Key);
			TArray<LineDataType> tmp2(Pair.Value);

			Ar << tmp << tmp2;

			counter++;
		}
	}

	void InsertLine(const LineDataType& LineData, const FLine& Line)
	{
		FVector2i IndexStart = Indexer.ToGrid(Line.LineStart);
		FVector2i IndexEnd = Indexer.ToGrid(Line.LineEnd);
		if (IndexStart == IndexEnd) return;

		TArray<FVector2i> Indices;

		Bresenham(IndexStart, IndexEnd, Indices);

		for (auto& Idx : Indices)
		{
			int64 idx = Idx.X * (BoundingBox.GetSize().Y / Indexer.CellSize) + Idx.Y;
			Hash.FindOrAdd(idx).Emplace(LineData);
			NumOfObj++;
		}
	}

	/*bool RemoveLine(const LineDataType& LineData, const FLine& Line)
	{
		FVector2i IndexStart = Indexer.ToGrid(Line.LineStart);
		FVector2i IndexEnd = Indexer.ToGrid(Line.LineEnd);
		TArray<FVector2i> Indices;

		Bresenham(IndexStart, IndexEnd, Indices);

		int count = 0;
		for (auto& Idx : Indices)
			count += Hash.RemoveSingle(Idx, LineData);

		return count > 0;
	}

	void UpdateLine(const LineDataType& LineData, const FLine& OldLine, const FLine& NewLine) {}*/

	int FindInRangeForFile(const FVector2d& QueryRangeMin, const FVector2d& QueryRangeMax,
		TFunction<bool(const LineDataType&)> IntersecFunc,
		TSet<LineDataType>& Data) const
	{
		if (!HashForFile.Num())
			return 0;

		FVector2i MinIndex = Indexer.ToGrid(QueryRangeMin);
		FVector2i MaxIndex = Indexer.ToGrid(QueryRangeMax);

		TArray<LineDataType> Values;
		for (int yi = MinIndex.Y; yi <= MaxIndex.Y; yi++)
			for (int xi = MinIndex.X; xi <= MaxIndex.X; xi++)
			{
				FVector2i Index(xi, yi);
				Values.Reset();
				int64 idx = Index.X * (BoundingBox.GetSize().Y / Indexer.CellSize) + Index.Y;
				//ValuesIdx = HashForFile[ValuesIdx];
				const int64* ValuesIdx = HashForFile.Find(idx);

				TUniquePtr<FArchive> Reader(IFileManager::Get().CreateFileReader(*FileName));
				(*Reader.Get()).Seek(*ValuesIdx);
				int64 Size = 4 + 2048 * sizeof(FVector2D) * 2;
				TArray<uint8> Ch;
				FMemoryReader FromBinary = FMemoryReader(Ch, true);
				Ch.Reset(Size + 2);
				Ch.AddUninitialized(Size);
				(*Reader.Get()).Serialize(Ch.GetData(), Ch.Num());
				(*Reader.Get()).Close();
				FromBinary.Seek(0);
				FromBinary << Values;

				for (LineDataType Value : Values)
					if (IntersecFunc(Value))
						Data.Emplace(Value);

				Values.Reset();
			}

		return Data.Num();
	}

	int FindInRange(const FVector2d& QueryRangeMin, const FVector2d& QueryRangeMax,
					TFunction<bool(const LineDataType&)> IntersecFunc,
					TSet<LineDataType>& Data) const
	{
		if (!Hash.Num())
			return 0;

		Data.Reset();

		FVector2i MinIndex = Indexer.ToGrid(QueryRangeMin);
		FVector2i MaxIndex = Indexer.ToGrid(QueryRangeMax);

		TArray<LineDataType> Values;
		for (int yi = MinIndex.Y; yi <= MaxIndex.Y; yi++)
			for (int xi = MinIndex.X; xi <= MaxIndex.X; xi++)
			{
				FVector2i Index(xi, yi);
				Values.Reset();
				int64 idx = Index.X * (BoundingBox.GetSize().Y / Indexer.CellSize) + Index.Y;
				Values = Hash.Find(idx);

				for (LineDataType Value : Values)
					if (IntersecFunc(Value))
						Data.Emplace(Value);
			}

		return Data.Num();
	}

private:
	void Bresenham(const FVector2i& Start, const FVector2i& End, TArray<FVector2i>& Points) const
	{
		Points.Reset();

		FVector2i Shift, StartShifted, EndShifted;
		int min = 0.0;

		min = FMathd::Min(Start.X, End.X);
		if (min < 0) Shift.X = -min;
		min = FMathd::Min(Start.Y, End.Y);
		if (min < 0) Shift.Y = -min;
		StartShifted = Start + Shift;
		EndShifted = End + Shift;

		int Width = EndShifted.X - StartShifted.X;
		int Height = EndShifted.Y - StartShifted.Y;
		int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

		if (Width < 0) dx1 = -1; else if (Width > 0) dx1 = 1;
		if (Height < 0) dy1 = -1; else if (Height > 0) dy1 = 1;
		if (Width < 0) dx2 = -1; else if (Width > 0) dx2 = 1;

		int Longest = FMathd::Abs(Width);
		int Shortest = FMathd::Abs(Height);

		if (Longest <= Shortest)
		{
			Longest = FMathd::Abs(Height);
			Shortest = FMathd::Abs(Width);
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
	}
};
