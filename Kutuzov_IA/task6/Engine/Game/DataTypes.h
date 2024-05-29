#pragma once
#include <map>
#include <string>

struct TIntVector2D;

struct TVector2D
{
	float X, Y;

	TVector2D(float X_ = 0.f, float Y_ = 0.f);
	TVector2D(TIntVector2D& InVector);

	float Length() const;

	bool operator==(const TVector2D& InVector) const;
	bool operator!=(const TVector2D& InVector) const;
	bool operator< (const TVector2D& InVector) const;
	bool operator<=(const TVector2D& InVector) const;
	bool operator> (const TVector2D& InVector) const;
	bool operator>=(const TVector2D& InVector) const;

	TVector2D operator+(const TVector2D& InVector) const;
	TVector2D& operator+=(const TVector2D& InVector);

	TVector2D operator-(const TVector2D& InVector) const;
	TVector2D& operator-=(const TVector2D& InVector);

	TVector2D operator*(const float Scale) const;
	TVector2D& operator*=(const float Scale);

	TVector2D operator*(const TVector2D& InVector) const;
	TVector2D& operator*=(const TVector2D& InVector);

	TVector2D operator/(const float Scale) const;
	TVector2D& operator/=(const float Scale);

	TVector2D operator/(const TVector2D& InVector) const;
	TVector2D& operator/=(const TVector2D& InVector);

	TVector2D Normalize() const;
	float DotProduct(TVector2D& InVector) const;

};

struct TIntVector2D
{
	int X, Y;

	TIntVector2D(int X_ = 0, int Y_ = 0);
	TIntVector2D(TVector2D& InVector);

	float Length() const;

	bool operator==(const TIntVector2D& InVector) const;
	bool operator!=(const TIntVector2D& InVector) const;
	bool operator< (const TIntVector2D& InVector) const;
	bool operator<=(const TIntVector2D& InVector) const;
	bool operator> (const TIntVector2D& InVector) const;
	bool operator>=(const TIntVector2D& InVector) const;

	TIntVector2D operator+(const TIntVector2D& InVector) const;
	TIntVector2D& operator+=(const TIntVector2D& InVector);

	TIntVector2D operator-(const TIntVector2D& InVector) const;
	TIntVector2D& operator-=(const TIntVector2D& InVector);

	TIntVector2D operator*(const int Scale) const;
	TIntVector2D& operator*=(const int Scale);

	TIntVector2D operator*(const TIntVector2D& InVector) const;
	TIntVector2D& operator*=(const TIntVector2D& InVector);

	TIntVector2D operator/(const int Scale) const;
	TIntVector2D& operator/=(const int Scale);

	TIntVector2D operator/(const TIntVector2D& InVector) const;
	TIntVector2D& operator/=(const TIntVector2D& InVector);

	TIntVector2D Normalize() const;
	float DotProduct(TIntVector2D& InVector) const;

};

struct TSprite
{
	std::string SpriteID;
	TVector2D RelPosition;
	TVector2D TargetSize;
	float Scale;

	TSprite(std::string InSpriteID, TVector2D InRelPosition = TVector2D(), float InScale = 1.f, TVector2D InTargetSize = TVector2D(1.f, 1.f));
};

struct TCamera
{
	TVector2D Position = TVector2D(0.f, 0.f);

	TVector2D Resolution = TVector2D(1280.f, 720.f);
	float UnitsPerPixel = 1.f;

	TVector2D GetScreenSizeInUnits();
};

struct TRenderLayerLocation
{
	int RenderLayer = 0;
	int Index = 0;
};

