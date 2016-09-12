#ifndef DRONA_WORKSPACE_H
#define DRONA_WORKSPACE_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct WS_Dimension {
	int x;
	int y;
	int z;
} WS_Dimension;

typedef struct WS_LocationsInfo {
	int size;
	int* locations; //integers list representing the cells occupied by obstacles
} WS_LocationsInfo;

typedef struct WS_RobotInfo {
	int id;
	int start_position; //integer representing the starting cell.
	//other information
} WS_RobotInfo;

typedef struct WorkspaceInfo {
	WS_Dimension dimension;
	WS_LocationsInfo obstacles;
	WS_LocationsInfo starts;
	WS_LocationsInfo ends;
	int numOfRobots;
	WS_RobotInfo *robots; //list of all robot-info for the numOfRobots.
} WorkspaceInfo;

inline int CompactCoord(WS_Dimension coord, WS_Dimension dim)
{
	return coord.x * dim.y * dim.z + coord.y * dim.z + coord.z;
}

inline WS_Dimension ExtractCoord(int compacted, WS_Dimension dim)
{
	WS_Dimension ret;
	ret.x = compacted / (dim.x * dim.y);
	ret.y = (compacted % (dim.x * dim.y)) / dim.y;
	ret.z = compacted % dim.y;
	return ret;
}

#ifdef __cplusplus
}
#endif
#endif