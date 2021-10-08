#include "BuildAirRaceMapInfo.h"

std::vector<std::shared_ptr<BaseAirRaceMap>> BuildAirRaceMapInfo::BuildAll()
{
	std::vector<std::shared_ptr<BaseAirRaceMap>> allMaps;
	allMaps.emplace_back(BuildPanicsAirRace());
	allMaps.emplace_back(BuildPanicsAirRaceHard());
	return allMaps;
}

std::shared_ptr<BaseAirRaceMap> BuildAirRaceMapInfo::BuildPanicsAirRace()
{
    return std::make_shared<BaseAirRaceMap>(BaseAirRaceMap(
		"panicsairrace",
		"Panic's Air Race: Beach",
        {Vector(-5479.08f, -23993.7f, 107.9f), Rotator(4925, -29189, -1646)},
        {   
            {Vector(-14357.9f, -27018.7f, 55.46f), Rotator(-100, 128, 0)},
            {Vector(-12694.9f, -23032.8f, 55.46f), Rotator(-100, -8352, 0)},
            {Vector(-8700.45f, -21379.4f, 55.46f), Rotator(-100, -16480, 0)},
            {Vector(-4714.4f, -23024.1f, 55.46f), Rotator(-99, -24800, 0)},
            {Vector(-3050.12f, -27029.3f, 55.46f), Rotator(-100, -32640, 0)},
            {Vector(-4713.1f, -31015.1f, 55.46f), Rotator(-100, 24416, 0)},
            {Vector(-8707.55f, -32668.6f, 55.46f), Rotator(-100, 16288, 0)},
            {Vector(-12693.5f, -31023.7f, 55.46f), Rotator(-100, 7968, 0)} 
        },
        {
			{
				30,
				0,
				{Vector(11429.1f, -25965.0f, 20.73f), Rotator(-99, 3968, 0)}
			},
			{
				39,
				1,
				{Vector(7217.72f, -16505.1f, -50.87f), Rotator(-99, 12896, 0)}
			},
			{
				61,
				2,
				{Vector(1581.56f, 793.81f, -50.87f), Rotator(-99, -16640, 0)}
			},
			{
				70,
				3,
				{Vector(-4065.18f, -12098.1f, -18.87f), Rotator(-99, 19840, 0)}
			},
			{
				76,
				4,
				{Vector(-6041.8f, 3165.83f, -50.87f), Rotator(-99, 23648, 0)}
			},
			{
				84,
				5,
				{Vector(10414.9f, 5010.1f, -50.87f), Rotator(-99, -8064, 0)}
			},
			{
				91,
				6,
				{Vector(17088.6f, 9104.01f, 61.13f), Rotator(-99, 26048, 0)}
			},
			{
				118,
				7,
				{Vector(12386.2f, 15565.4f, 61.13f), Rotator(-99, -4128, 0)}
			},
			{
				126,
				8,
				{Vector(10192.1f, 21147.2f, -50.87f), Rotator(-99, 21472, 0)}
			},
			{
				155,
				9,
				{Vector(-868.91f, 13827.7f, -50.87f), Rotator(-99, 31648, 0)}
			},
			{
				172,
				10,
				{Vector(-13317.9f, 7834.04f, -50.87f), Rotator(-99, -21824, 0)}
			}
        }
    ));
}

std::shared_ptr<BaseAirRaceMap> BuildAirRaceMapInfo::BuildPanicsAirRaceHard()
{
	return std::make_shared<BaseAirRaceMap>(BaseAirRaceMap(
		"panicsairracehard",
		"Panic's Air Race: Beach [Hard]",
		{ Vector(-5479.08f, -23993.7f, 107.9f), Rotator(4925, -29189, -1646) },
		{
			{Vector(-14357.9f, -27018.7f, 55.46f), Rotator(-100, 128, 0)},
			{Vector(-12694.9f, -23032.8f, 55.46f), Rotator(-100, -8352, 0)},
			{Vector(-8700.45f, -21379.4f, 55.46f), Rotator(-100, -16480, 0)},
			{Vector(-4714.4f, -23024.1f, 55.46f), Rotator(-99, -24800, 0)},
			{Vector(-3050.12f, -27029.3f, 55.46f), Rotator(-100, -32640, 0)},
			{Vector(-4713.1f, -31015.1f, 55.46f), Rotator(-100, 24416, 0)},
			{Vector(-8707.55f, -32668.6f, 55.46f), Rotator(-100, 16288, 0)},
			{Vector(-12693.5f, -31023.7f, 55.46f), Rotator(-100, 7968, 0)}
		},
		{
			{
				30,
				0,
				{Vector(11429.1f, -25965.0f, 20.73f), Rotator(-99, 3968, 0)}
			},
			{
				67,
				1,
				{Vector(4324.74f, -16946.03f, -70.87f), Rotator(-99, -30112, 0)}
			},
			{
				95,
				2,
				{Vector(16881.35f, -8285.39f, -70.87f), Rotator(-99, 18176, 0)}
			},
			{
				138,
				3,
				{Vector(363.83f, -5871.45f, -38.87f), Rotator(-99, 16768, 0)}
			},
			{
				149,
				4,
				{Vector(12459.81f, 462.26f, -70.87f), Rotator(-99, 1120, 0)}
			},
			{
				169,
				5,
				{Vector(14731.38f, 8409.35f, -70.87f), Rotator(-99, -30592, 0)}
			},
			{
				178,
				6,
				{Vector(16251.58f, 22202.67f, 41.13f), Rotator(-99, 29120, 0)}
			},
			{
				204,
				7,
				{Vector(7714.85f, 16393.39f, 41.13f), Rotator(-99, -31776, 0)}
			},
			{
				236,
				8,
				{Vector(-12229.56f, 14613.95f, -70.91f), Rotator(-99, -13344, 0)}
			},
			{
				257,
				9,
				{Vector(-22073.93f, -18363.92f, -70.87f), Rotator(-99, 17312, 0)}
			},
			{
				285,
				10,
				{Vector(-25713.47f, -11347.24f, -70.87f), Rotator(-99, -20800, 0)}
			}
		}
		));
}