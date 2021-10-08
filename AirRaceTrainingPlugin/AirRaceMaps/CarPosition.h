#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

struct CarPosition {
	Vector Location;
	Rotator Rotation;
};

struct Checkpoint {
	int RingCount;
	int CPCount;
	CarPosition SpawnLocation;
};