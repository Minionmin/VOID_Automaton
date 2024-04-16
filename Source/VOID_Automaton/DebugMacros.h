#pragma once

#include "DrawDebugHelpers.h"

#define LOG(text) UE_LOG(LogTemp, Warning, TEXT(text))
#define LOG_ONE_PARAM(text, textVal) UE_LOG(LogTemp, Warning, TEXT(text), textVal)
#define LOG_VECTOR(outputVector) UE_LOG(LogTemp, Warning, TEXT("Vector: (%2f, %2f, %2f)"), outputVector.X, outputVector.Y, outputVector.Z);
#define ERROR_LOG(text) UE_LOG(LogTemp, Error, TEXT(text))
#define ERROR_LOG_ONE_PARAM(text, textVal) UE_LOG(LogTemp, Error, TEXT(text), textVal)

#define DRAW_SPHERE(location, lifeTime) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 100.f, 16, FColor::Red, false, lifeTime);
#define DRAW_SPHERE_SINGLEFRAME(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 100.f, 16, FColor::Red, false, -1);

#define DRAW_LINE(startLocation, endLocation, lifeTime) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, lifeTime);
#define DRAW_LINE_SINGLEFRAME(startLocation, endLocation) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, -1.f);

#define DRAW_POINT(location, lifeTime) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15.f, FColor::Red, false, lifeTime);
#define DRAW_POINT_SINGLEFRAME(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15.f, FColor::Red, false, -1.f);

#define DRAW_VECTOR(startLocation, endLocation, lifeTime) if(GetWorld()) \
		{ \
			DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, lifeTime); \
			DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Red, false, lifeTime); \
		}
#define DRAW_VECTOR_SINGLEFRAME(startLocation, endLocation) if(GetWorld()) \
		{ \
			DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, -1.f); \
			DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Red, false, -1.f); \
		}