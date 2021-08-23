// Fill out your copyright notice in the Description page of Project Settings.


#include "ChartLineSection.h"

void FLineSection::BuildLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs)
{
	switch (Type)
	{
	case EChartLineType::Solid:
		BuildSolidLine(OutVertices, OutUVs);
		break;
	case EChartLineType::Dashed:
		BuildDashedLine(OutVertices, OutUVs);
		break;
	case EChartLineType::Dotted:
		BuildDottedLine(OutVertices, OutUVs);
		break;
	}
}

void FLineSection::BuildSolidLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs) const
{
	OutVertices.Empty();
	OutUVs.Empty();

	if (Vertices.Num() > 1)
	{
		BuildOuterVertices(FVector(Vertices[0], ZDepth), FVector(Vertices[1], ZDepth), OutVertices, true);

		TArray<FVector> tmp;
		for (int i = 1; i < Vertices.Num() - 1; i++)
		{
			FVector2D Start = Vertices[i - 1], Middle = Vertices[i], End = Vertices[i + 1];
			tmp.Empty();
			if (FVector2D::Distance(Middle, End) < Width && i < Vertices.Num() - 2)
				continue;
			if (!BuildInnerVertices(FVector(Start, ZDepth), FVector(Middle, ZDepth), FVector(End, ZDepth), tmp))
				continue;
			OutVertices.Append(tmp);
		}

		BuildOuterVertices(FVector(Vertices[Vertices.Num() - 1], ZDepth), FVector(Vertices[Vertices.Num() - 2], ZDepth), tmp, false);
		OutVertices.Append(tmp);
	}
}

void FLineSection::BuildDashedLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs) const
{
	OutVertices.Empty();
	OutUVs.Empty();

	if (Vertices.Num() > 1)
	{
		float y1 = 0.5 + 1. / (2. * DashLength), y2 = 0.5 + 1. / (2. * DashLength), t1 = 0, t2 = 0, tmp0 = 0;

		BuildOuterVertices(FVector(Vertices[0], ZDepth), FVector(Vertices[1], ZDepth), OutVertices, true);

		TArray<FVector> tmp;
		for (int i = 1; i < Vertices.Num() - 1; i++)
		{
			FVector2D Start = Vertices[i - 1], Middle = Vertices[i], End = Vertices[i + 1];
			tmp.Empty();
			if (FVector2D::Distance(Middle, End) < Width && i < Vertices.Num() - 2)
				continue;
			if (!BuildInnerVertices(FVector(Start, ZDepth), FVector(Middle, ZDepth), FVector(End, ZDepth), tmp))
				continue;
			OutVertices.Append(tmp);

			float min, max;

			tmp0 = FMath::Sqrt(abs(FMath::Square(FVector::Distance(OutVertices[OutVertices.Num() - 8], OutVertices[OutVertices.Num() - 7])) - FMath::Square(Width * 2))) / (DashInterval + DashLength);
			if (OutVertices[OutVertices.Num() - 8] == FMath::ClosestPointOnSegment(OutVertices[OutVertices.Num() - 7], OutVertices[OutVertices.Num() - 8], OutVertices[OutVertices.Num() - 6]))
			{
				y1 = y2 + tmp0;
				min = y2;
			}
			else
			{
				y2 = y1 + tmp0;
				min = y1;
			}

			float y1_prev = y1, y2_prev = y2;

			y1 += FVector::Distance(OutVertices[OutVertices.Num() - 8], OutVertices[OutVertices.Num() - 6]) / (DashInterval + DashLength);
			y2 += FVector::Distance(OutVertices[OutVertices.Num() - 7], OutVertices[OutVertices.Num() - 5]) / (DashInterval + DashLength);

			if (OutVertices[OutVertices.Num() - 6] == FMath::ClosestPointOnSegment(OutVertices[OutVertices.Num() - 5], OutVertices[OutVertices.Num() - 8], OutVertices[OutVertices.Num() - 6]))
				max = y2;
			else max = y1;
			
			y1_prev = FMath::GetMappedRangeValueClamped(FVector2D(min, max), FVector2D(min, AttachToDecimal(max, 0.5 - DashInterval / (2 * (DashInterval + DashLength)))), y1_prev);
			y2_prev = FMath::GetMappedRangeValueClamped(FVector2D(min, max), FVector2D(min, AttachToDecimal(max, 0.5 - DashInterval / (2. * (DashInterval + DashLength)))), y2_prev);
			y1 = FMath::GetMappedRangeValueClamped(FVector2D(min, max), FVector2D(min, AttachToDecimal(max, 0.5 - DashInterval / (2. * (DashInterval + DashLength)))), y1);
			y2 = FMath::GetMappedRangeValueClamped(FVector2D(min, max), FVector2D(min, AttachToDecimal(max, 0.5 - DashInterval / (2. * (DashInterval + DashLength)))), y2);
			
			OutUVs.Emplace(y1_prev, 0);
			OutUVs.Emplace(y2_prev, 1);
			OutUVs.Emplace(y1, 0);
			OutUVs.Emplace(y2, 1);
			OutUVs.Emplace(y1, 0);
			OutUVs.Emplace(y2, 1);

			OutVertices.Emplace(tmp[tmp.Num() - 2]);
			OutVertices.Emplace(tmp[tmp.Num() - 1]);
			OutUVs.Emplace(y1, 0);
			OutUVs.Emplace(y2, 1);
		}

		BuildOuterVertices(FVector(Vertices[Vertices.Num() - 1], ZDepth), FVector(Vertices[Vertices.Num() - 2], ZDepth), tmp, false);
		OutVertices.Append(tmp);

		float min;
		tmp0 = FMath::Sqrt(abs(FMath::Square(FVector::Distance(OutVertices[OutVertices.Num() - 4], OutVertices[OutVertices.Num() - 3])) - FMath::Square(Width * 2))) / (DashInterval + DashLength);
		if (OutVertices[OutVertices.Num() - 4] == FMath::ClosestPointOnSegment(OutVertices[OutVertices.Num() - 3], OutVertices[OutVertices.Num() - 4], OutVertices[OutVertices.Num() - 2]))
		{
			y1 = y2 + tmp0;
			min = y2;
		}
		else
		{
			y2 = y1 + tmp0;
			min = y1;
		}
		float y1_prev = y1, y2_prev = y2;

		y1 += FVector::Distance(OutVertices[OutVertices.Num() - 4], OutVertices[OutVertices.Num() - 2]) / (DashInterval + DashLength);
		float ttt = y1;
		y1 = AttachToDecimal(y1, 0.5 - 1 / (2 * DashLength));
		y2 = y1;

		y1_prev = FMath::GetMappedRangeValueClamped(FVector2D(min, ttt), FVector2D(min, y1), y1_prev);
		y2_prev = FMath::GetMappedRangeValueClamped(FVector2D(min, ttt), FVector2D(min, y1), y2_prev);

		OutUVs.Emplace(y1_prev, 0);
		OutUVs.Emplace(y2_prev, 1);
		OutUVs.Emplace(y1, 0);
		OutUVs.Emplace(y2, 1);
	}
}

void FLineSection::BuildDottedLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs) const
{
	OutVertices.Empty();
	OutUVs.Empty();

	if (Vertices.Num() > 1)
	{
		for (int i = 0; i < Vertices.Num() - 1; i++)
		{
			OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[i] - Width, ZDepth)));
			OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[i].X - Width, Vertices[i].Y + Width, ZDepth)));
			OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[i].X + Width, Vertices[i].Y - Width, ZDepth)));
			OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[i] + Width, ZDepth)));
			OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[i + 1] - Width, ZDepth)));
			OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[i + 1].X - Width, Vertices[i + 1].Y + Width, ZDepth)));

			OutUVs.Emplace(0, 0);
			OutUVs.Emplace(0, 1);
			OutUVs.Emplace(1, 0);
			OutUVs.Emplace(1, 1);
			OutUVs.Emplace(1, 0);
			OutUVs.Emplace(1, 1);
		}

		OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[Vertices.Num() - 1] - Width, ZDepth)));
		OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[Vertices.Num() - 1].X - Width, Vertices[Vertices.Num() - 1].Y + Width, ZDepth)));
		OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[Vertices.Num() - 1].X + Width, Vertices[Vertices.Num() - 1].Y - Width, ZDepth)));
		OutVertices.Emplace(InvertAxisYZ(FVector(Vertices[Vertices.Num() - 1] + Width, ZDepth)));
		OutUVs.Emplace(0, 0);
		OutUVs.Emplace(0, 1);
		OutUVs.Emplace(1, 0);
		OutUVs.Emplace(1, 1);
	}
}
bool FLineSection::BuildInnerVertices(const FVector& Start, const FVector& Middle, const FVector& End, TArray<FVector>& OutVertices) const
{
	OutVertices.Empty();

	FVector UnitToStart = (Start - Middle).GetSafeNormal();
	FVector UnitToEnd = (End - Middle).GetSafeNormal();

	float CosAngle = UnitToStart.CosineAngle2D(UnitToEnd);

	// 180 angle, can be skipped
	if (FMath::IsNearlyEqual(CosAngle, -1.f, 0.01f)) return false;

	// Getting half the angle (degrees)
	float Angle = (90.f) / PI * FMath::Acos(CosAngle);
	// Getting sine
	float SinAngle = FMath::Sin(PI / (180.f) * Angle);

	FVector Unit = (UnitToStart + UnitToEnd) / (UnitToStart + UnitToEnd).Size();

	bool bSwap = false;
	if (!((0 <= UnitToStart.HeadingAngle() && UnitToStart.HeadingAngle() <= PI / 2. && -UnitToStart.HeadingAngle() < UnitToEnd.HeadingAngle() && UnitToEnd.HeadingAngle() < UnitToStart.HeadingAngle()) ||
		(0 >= UnitToStart.HeadingAngle() && UnitToStart.HeadingAngle() >= -PI / 2. && UnitToStart.HeadingAngle() < UnitToEnd.HeadingAngle() && UnitToEnd.HeadingAngle() < -UnitToStart.HeadingAngle()) ||
		(PI / 2. <= UnitToStart.HeadingAngle() && UnitToStart.HeadingAngle() <= PI && -UnitToStart.HeadingAngle() < UnitToEnd.HeadingAngle() && UnitToEnd.HeadingAngle() < UnitToStart.HeadingAngle()) ||
		(-PI / 2. >= UnitToStart.HeadingAngle() && UnitToStart.HeadingAngle() >= -PI && UnitToStart.HeadingAngle() < UnitToEnd.HeadingAngle() && UnitToEnd.HeadingAngle() < -UnitToStart.HeadingAngle())))
		bSwap = true;

	FVector BisectorLong, BisectorShort;
	BisectorLong = Unit * (Width / SinAngle);
	BisectorShort = -Unit * Width;

	FVector NormalLeft, NormalRight;

	if (Angle < 15)
	{
		NormalLeft = UnitToStart.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f)) * Width;
		NormalRight = UnitToEnd.RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f)) * Width;

		OutVertices.Emplace(InvertAxisYZ(Middle + NormalLeft));
		OutVertices.Emplace(InvertAxisYZ(Middle + NormalRight));
		OutVertices.Emplace(InvertAxisYZ(Middle + BisectorShort));
		OutVertices.Emplace(InvertAxisYZ(Middle + BisectorShort));
		OutVertices.Emplace(InvertAxisYZ(Middle + NormalRight));
		OutVertices.Emplace(InvertAxisYZ(Middle + NormalLeft));
	}
	else
	{
		if ((Unit.Y > 0 && !bSwap) || (Unit.Y < 0 && bSwap))
		{
			NormalLeft = UnitToStart.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f)) * Width;
			NormalRight = UnitToEnd.RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f)) * Width;

			OutVertices.Emplace(InvertAxisYZ(Middle + NormalLeft));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorLong));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorShort));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorLong));
			OutVertices.Emplace(InvertAxisYZ(Middle + NormalRight));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorLong));

		}
		else
		{
			NormalLeft = UnitToStart.RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f)) * Width;
			NormalRight = UnitToEnd.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f)) * Width;

			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorLong));
			OutVertices.Emplace(InvertAxisYZ(Middle + NormalLeft));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorLong));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorShort));
			OutVertices.Emplace(InvertAxisYZ(Middle + BisectorLong));
			OutVertices.Emplace(InvertAxisYZ(Middle + NormalRight));
		}
	}

	return true;
}

void FLineSection::BuildOuterVertices(const FVector& Start, const FVector& End, TArray<FVector>& OutVertices, bool bStart) const
{
	OutVertices.Empty();

	FVector Normal = (End - Start).GetSafeNormal().RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f)) * Width;

	if (!bStart) Normal = -Normal;

	OutVertices.Emplace(InvertAxisYZ(Start + Normal));
	OutVertices.Emplace(InvertAxisYZ(Start - Normal));
}

float FLineSection::AttachToDecimalRange(float Value, const FVector2D& Range) const
{
	float Decimal = Value - FMath::FloorToFloat(Value);

	if (Decimal > Range.Y)
		return AttachToDecimal(Value, Range.Y);
	else if (Decimal < Range.X)
		return AttachToDecimal(Value, Range.X);
	return Value;
}

float FLineSection::AttachToDecimal(float Value, float Decimal) const
{
	Decimal = Decimal - FMath::FloorToFloat(Decimal);

	float Var1 = FMath::FloorToFloat(Value) + Decimal;
	float Var2 = FMath::CeilToFloat(Value) + Decimal;

	if (Value - Var1 > Value - Var2) return Var2;
	return Var1;
}