// Fill out your copyright notice in the Description page of Project Settings.

#include "TestQuadTree.h"
#include "GenericQuadTree.h"
#include "LineHashGrid.h"
#include "Serialization/BufferArchive.h"

struct FLineData
{
	FVector2D Start;
	FVector2D End;

	FLineData() {};

	FLineData(float x1, float y1, float x2, float y2)
	{
		Start.Set(x1, y1);
		End.Set(x2, y2);
	}

	FLineData(const FVector2D& InStart, const FVector2D& InEnd)
	{
		Start = InStart;
		End = InEnd;
	}

	bool operator==(const FLineData& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FLineData& Other) const
	{
		return Start.Equals(Other.Start) && End.Equals(Other.End);
	}

	friend FArchive& operator<<(FArchive& Ar, FLineData& LineData)
	{
		Ar << LineData.Start;
		Ar << LineData.End;
		return Ar;
	}
};

FORCEINLINE uint64 GetTypeHash(const FLineData& LineData)
{
	return FCrc::MemCrc32(&LineData, sizeof(FLineData));
}

// Sets default values
ATestQuadTree::ATestQuadTree()
{
	/*TUniquePtr<FArchive> Reader(IFileManager::Get().CreateFileReader(TEXT("D:/LAB/go/awesomeProject/100m_sort.csv")));

	int64 Size = 1024, counter = 0, MaxElementsInCell = 4096, MaxElementsInGrid = 600000, cnt = 0;
	FString test_prev;
	FBox2D BB;
	BB.Min = FVector2D(TNumericLimits<float>::Max());
	BB.Max = FVector2D(TNumericLimits<float>::Min());

	// Find BB, count
	for (int64 i = 0; i < (*Reader.Get()).TotalSize(); i += Size)
	{
		if ((*Reader.Get()).TotalSize() - i < Size) Size = (*Reader.Get()).TotalSize() - i;

		(*Reader.Get()).Seek(i);

		TArray<uint8> Ch;
		Ch.Reset(Size + 2);
		Ch.AddUninitialized(Size);

		(*Reader.Get()).Serialize(Ch.GetData(), Ch.Num());

		FString test;
		FFileHelper::BufferToString(test, Ch.GetData(), Ch.Num());
		TArray<FString> arr;

		test.ParseIntoArrayLines(arr, false);

		arr[0] = test_prev + arr[0];
		test_prev = arr.Last();
		if ((*Reader.Get()).TotalSize() - i > Ch.Num()) arr.RemoveAt(arr.Num() - 1);

		for (auto str : arr)
		{
			TArray<FString> tmp;

			str.ParseIntoArray(tmp, TEXT(";"));

			if (tmp.Num() > 1)
			{
				FVector2D tmp_vector;

				tmp_vector.X = FCString::Atof(*tmp[0]);
				tmp_vector.Y = FCString::Atof(*tmp[1]);

				if (tmp_vector.X > BB.Max.X) BB.Max.X = tmp_vector.X;
				if (tmp_vector.X < BB.Min.X) BB.Min.X = tmp_vector.X;

				if (tmp_vector.Y > BB.Max.Y) BB.Max.Y = tmp_vector.Y;
				if (tmp_vector.Y < BB.Min.Y) BB.Min.Y = tmp_vector.Y;

			}

			counter++;
		}
	}

	float StartStep = FMath::Sqrt(BB.GetArea() / counter * MaxElementsInCell);
	StartStep /= 6;

	typedef TLineHashGrid<FLineData> FLineHashGrid;
	TUniquePtr<FLineHashGrid> LineHashGrid = MakeUnique<FLineHashGrid>(StartStep, BB);

	// Build w start step
	Size = 1024, counter = 0, cnt = 0;
	for (int64 i = 0; i < (*Reader.Get()).TotalSize(); i += Size)
	{
		if ((*Reader.Get()).TotalSize() - i < Size) Size = (*Reader.Get()).TotalSize() - i;

		(*Reader.Get()).Seek(i);

		TArray<uint8> Ch;
		Ch.Reset(Size + 2);
		Ch.AddUninitialized(Size);

		(*Reader.Get()).Serialize(Ch.GetData(), Ch.Num());

		FString test;
		FFileHelper::BufferToString(test, Ch.GetData(), Ch.Num());

		TArray<FString> arr;

		test.ParseIntoArrayLines(arr, false);

		arr[0] = test_prev + arr[0];
		test_prev = arr.Last();
		if ((*Reader.Get()).TotalSize() - i > Ch.Num()) arr.RemoveAt(arr.Num() - 1);

		FVector2D tmp_vector_prev;
		for (auto str : arr)
		{
			TArray<FString> tmp;

			str.ParseIntoArray(tmp, TEXT(";"));

			if (tmp.Num() > 1)
			{
				FVector2D tmp_vector;

				tmp_vector.X = FCString::Atof(*tmp[0]);
				tmp_vector.Y = FCString::Atof(*tmp[1]);

				if (counter != 0)
				{
					FLine Line = { FVector2d(tmp_vector_prev.X, tmp_vector_prev.Y), FVector2d(tmp_vector.X, tmp_vector.Y) };
					LineHashGrid.Get()->InsertLine(FLineData(), Line);
				}

				tmp_vector_prev = tmp_vector;
				counter++;

				if (LineHashGrid.Get()->GetCountObjects() > 10000000)
				{
					counter = 0;

					FBufferArchive a;

					LineHashGrid.Get()->SerializeCountsOnly(a);
					if (a.Num() <= 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("Saving failed!"));
						break;
					}
					LineHashGrid.Get()->Clear();

					FString name = "D:/LAB/Unreal Projects/MyProject/test";
					FFileHelper::SaveArrayToFile(a, *(name + FString::FromInt(cnt)));
					a.FlushCache();
					a.Empty();

					cnt++;
				}
			}
		}
	}


	// Find max in nodes
	TArray<TUniquePtr<FArchive>> Sizes;
	FString name = "D:/LAB/Unreal Projects/MyProject/test";

	for (int64 i = 0; i < cnt; i++)
	{
		TUniquePtr<FArchive> tmpReader(IFileManager::Get().CreateFileReader(*(name + FString::FromInt(i))));
		(*tmpReader.Get()).Seek(0);
		Sizes.Add(MoveTemp(tmpReader));
	}

	TMap<int64, TPair<int64, TSet<int64>>> heap;
	int64 max = TNumericLimits<int64>::Min();
	int64 tmpCounter = 0;
	while (tmpCounter < cnt)
	{
		for (int64 i = 0; i < cnt; i++)
		{
			if (!IFileManager::Get().FileExists(*(name + FString::FromInt(i)))) continue;

			Size = 16;
			TArray<uint8> Ch;
			FMemoryReader FromBinary = FMemoryReader(Ch, true);
			Ch.Reset(Size + 2);
			Ch.AddUninitialized(Size);
			
			(*Sizes[i].Get()).Serialize(Ch.GetData(), Ch.Num());
			FromBinary.Seek(0);

			int64 tmp;
			int64 tmp2;
			FromBinary << tmp << tmp2;

			heap.FindOrAdd(tmp).Key += tmp2;
			heap.FindOrAdd(tmp).Value.Add(i);

			int64 t1 = (*Sizes[i].Get()).Tell();
			(*Sizes[i].Get()).Seek(t1 - 16);
		}
		TSet<int64> tmpind;

		int64 min = TNumericLimits<int64>::Max();
		for (auto& el : heap)
		{
			if (el.Value.Key > max)
			{
				max = el.Value.Key;
			}
			if (el.Key < min)
			{
				min = el.Key;
				tmpind = el.Value.Value;
			}
		}
		for (auto& el : tmpind)
		{
			int64 t1 = (*Sizes[el].Get()).Tell();
			int64 ts = (*Sizes[el].Get()).TotalSize();
			(*Sizes[el].Get()).Seek(t1 + 16);

			// Последний элемент?
			if (ts - t1 <= 16)
			{
				(*Sizes[el].Get()).Close();
				IFileManager::Get().Delete(*(name + FString::FromInt(el)));
				tmpCounter++;
			}
		}

		heap.Reset();
		//break;
	}
	Sizes.Reset();


	// Build w "optimal" step
	LineHashGrid.Get()->Clear();
	Size = 1024, counter = 0, cnt = 0;
	for (int64 i = 0; i < (*Reader.Get()).TotalSize(); i += Size)
	{
		if ((*Reader.Get()).TotalSize() - i < Size) Size = (*Reader.Get()).TotalSize() - i;

		(*Reader.Get()).Seek(i);

		TArray<uint8> Ch;
		Ch.Reset(Size + 2);
		Ch.AddUninitialized(Size);

		(*Reader.Get()).Serialize(Ch.GetData(), Ch.Num());

		FString test;
		FFileHelper::BufferToString(test, Ch.GetData(), Ch.Num());

		TArray<FString> arr;

		test.ParseIntoArrayLines(arr, false);

		arr[0] = test_prev + arr[0];
		test_prev = arr.Last();
		if ((*Reader.Get()).TotalSize() - i > Ch.Num()) arr.RemoveAt(arr.Num() - 1);

		FVector2D tmp_vector_prev;
		for (auto str : arr)
		{
			TArray<FString> tmp;

			str.ParseIntoArray(tmp, TEXT(";"));

			if (tmp.Num() > 1)
			{
				FVector2D tmp_vector;

				tmp_vector.X = FCString::Atof(*tmp[0]);
				tmp_vector.Y = FCString::Atof(*tmp[1]);

				if (counter != 0)
				{
					FLine Line = { FVector2d(tmp_vector_prev.X, tmp_vector_prev.Y), FVector2d(tmp_vector.X, tmp_vector.Y) };
					LineHashGrid.Get()->InsertLine(FLineData(tmp_vector_prev, tmp_vector), Line);
				}

				tmp_vector_prev = tmp_vector;
				counter++;

				if (LineHashGrid.Get()->GetCountObjects() > 10000000)
				{
 					counter = 0;

					FBufferArchive a;

					LineHashGrid.Get()->Serialize(a);
					if (a.Num() <= 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("Saving failed!"));
						break;
					}
					LineHashGrid.Get()->Clear();

					FFileHelper::SaveArrayToFile(a, *(name + FString::FromInt(cnt)));
					a.FlushCache();
					a.Empty();

					cnt++;
				}
			}
		}
	}
	LineHashGrid.Get()->Clear();
	(*Reader.Get()).Close();


	TMap<int64, int64> HashGridQueryMap;

	// Merge
	Sizes.Reset();
	for (int64 i = 0; i < cnt; i++)
	{
		TUniquePtr<FArchive> tmpReader(IFileManager::Get().CreateFileReader(*(name + FString::FromInt(i))));
		(*tmpReader.Get()).Seek(0);
		Sizes.Add(MoveTemp(tmpReader));
	}

	TMap<int64, TSet<int64>> heap2;
	tmpCounter = 0;
	while (tmpCounter < cnt)
	{
		for (int64 i = 0; i < cnt; i++)
		{
			if (!IFileManager::Get().FileExists(*(name + FString::FromInt(i)))) continue;

			int64 t1 = (*Sizes[i].Get()).Tell();

			Size = 8;
			TArray<uint8> Ch;
			FMemoryReader FromBinary = FMemoryReader(Ch, true);
			Ch.Reset(Size + 2);
			Ch.AddUninitialized(Size);

			(*Sizes[i].Get()).Serialize(Ch.GetData(), Ch.Num());
			FromBinary.Seek(0);

			//int64 t3 = (*Sizes[i].Get()).Tell();

			int64 tmp;
			//TArray<FLineData> tmp2;
			FromBinary << tmp;

			heap2.FindOrAdd(tmp).Emplace(i);

			(*Sizes[i].Get()).Seek(t1);
		}

		TSet<int64> tmpind;
		int64 min = TNumericLimits<int64>::Max();
		for (auto& el : heap2)
		{
			if (el.Key < min)
			{
				min = el.Key;
				tmpind = el.Value;
			}
		}
		int64 qq = 0;
		for (auto& el : tmpind)
		{
			int64 t1 = (*Sizes[el].Get()).Tell();

			Size = 8 + max * sizeof(FVector2D) * 2 + 2;
			TArray<uint8> Ch;
			FMemoryReader FromBinary = FMemoryReader(Ch, true);
			Ch.Reset(Size + 2);
			Ch.AddUninitialized(Size);

			(*Sizes[el].Get()).Serialize(Ch.GetData(), Ch.Num());
			FromBinary.Seek(0);

			int64 tmp;
			TArray<FLineData> tmp2;
			FromBinary << tmp << tmp2;
			(*Sizes[el].Get()).Seek(t1 + FromBinary.Tell());

			FBufferArchive a;
			if (qq == 0)
			{
				a << tmp;
				HashGridQueryMap.Add(tmp, a.Tell());
			}
			a << tmp2;
			FFileHelper::SaveArrayToFile(a, *(name), &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
			a.FlushCache();
			a.Empty();

			int64 ts = (*Sizes[el].Get()).TotalSize();
			int64 tt = (*Sizes[el].Get()).Tell();

			if (ts - tt <= Size)
			{
				(*Sizes[el].Get()).Close();
				IFileManager::Get().Delete(*(name + FString::FromInt(el)));
				tmpCounter++;
			}

			qq++;
		}

		heap2.Reset();
	}
	Sizes.Reset();


	// Query
	FBox QueryBox(FVector(0.0), FVector(200.0, 500.0, 0.0));
	LineHashGrid.Get()->SetFileInfo(HashGridQueryMap, name);
	HashGridQueryMap.Reset();

	TSet<FLineData> Res;
	LineHashGrid.Get()->FindInRangeForFile(FVector2d(QueryBox.Min.X, QueryBox.Min.Y), FVector2d(QueryBox.Max.X, QueryBox.Max.Y), [QueryBox](const FLineData& Data)
		{
			return FMath::LineBoxIntersection(QueryBox, FVector(Data.Start, 0.0), FVector(Data.End, 0.0), FVector(Data.End, 0.0) - FVector(Data.Start, 0.0));
		}, Res);

	UE_LOG(LogTemp, Warning, TEXT("Time ex all: %d.%d"));*/

	/*FDateTime endT = FDateTime::UtcNow();
	int64 endMs = endT.GetMillisecond();
	int64 unixEnd = endT.ToUnixTimestamp() * 1000 + endMs;
	UE_LOG(LogTemp, Warning, TEXT("Time ex all: %d.%d"), (unixEnd - unixStart)/1000, (unixEnd - unixStart) % 1000);*/
}

void ATestQuadTree::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e)
{
	
}

// Called when the game starts or when spawned
void ATestQuadTree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestQuadTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

