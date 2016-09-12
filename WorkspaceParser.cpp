#include "WorkspaceParser.h"
#include "tinyxml2.h"
#include <string.h>
#include <stdexcept>

using namespace tinyxml2;

const XMLNode* ParseXMLChildElement(const XMLNode* node, const char* optionName)
{
	const XMLNode *ret = node->FirstChildElement(optionName);
	if(!ret) {
		throw std::runtime_error(std::string("Failed to parse, ") + std::string(optionName) + std::string(" is required"));
	}
	return ret;
}

WS_Dimension ParseXMLCoord(const XMLNode* node)
{
	WS_Dimension dim;
	const XMLElement *e = node->ToElement();
	if(!e)
		throw std::runtime_error("Not a valid coordinate element");
	dim.x = e->IntAttribute("x");
	dim.y = e->IntAttribute("y");
	dim.z = e->IntAttribute("z");
	return dim;
}

int CountXMLChild(const XMLNode* node, const char* name) 
{
	int count = 0;
	for(const XMLElement* nodeIter = node->FirstChildElement(name); 
		nodeIter != NULL; 
		nodeIter = nodeIter->NextSiblingElement(name))
	{
		count++;
	}
	return count;
}

WS_LocationsInfo ParseListOfCoordinates(const XMLNode* node, WS_Dimension dim)
{
	WS_LocationsInfo locations;
	locations.size = CountXMLChild(node, "coord");
	locations.locations = (int*)malloc(sizeof(int) * locations.size);
	int count = 0;
	for(const XMLElement* nodeIter = node->FirstChildElement("coord"); 
		nodeIter != NULL; 
		nodeIter = nodeIter->NextSiblingElement("coord"))
	{
		locations.locations[count++] = CompactCoord(ParseXMLCoord(nodeIter), dim);
	}
	return locations;
}

WorkspaceInfo* ParseConfiguration(const char* configurationFile)
{
	WorkspaceInfo *workspace_info = (WorkspaceInfo*)malloc(sizeof(WorkspaceInfo));

	XMLDocument configDoc;
    configDoc.LoadFile(configurationFile);

    const XMLNode* configuration = ParseXMLChildElement(&configDoc, "configuration");
	const XMLNode* tmp;
	if(tmp = ParseXMLChildElement(configuration, "dimension")) {
		workspace_info->dimension = ParseXMLCoord(tmp);
	}
	if(tmp = ParseXMLChildElement(configuration, "robots")) {
		workspace_info->numOfRobots = CountXMLChild(tmp, "robot");
		workspace_info->robots = (WS_RobotInfo*)malloc(sizeof(WS_RobotInfo) * workspace_info->numOfRobots);
		int count = 0;
		for(const XMLElement* nodeIter = tmp->FirstChildElement("robot"); 
			nodeIter != NULL; 
			nodeIter = nodeIter->NextSiblingElement("robot"))
		{
			WS_RobotInfo& robot_info = workspace_info->robots[count]; 
			robot_info.id = count;
			robot_info.start_position = CompactCoord(ParseXMLCoord(ParseXMLChildElement(nodeIter, "start")), workspace_info->dimension);
			count++;
		}
	}
	if(tmp = ParseXMLChildElement(configuration, "obstacles")) {
		workspace_info->obstacles = ParseListOfCoordinates(tmp, workspace_info->dimension);
	}
	if(tmp = ParseXMLChildElement(configuration, "starts")) {
		workspace_info->starts = ParseListOfCoordinates(tmp, workspace_info->dimension);
	}
	if(tmp = ParseXMLChildElement(configuration, "ends")) {
		workspace_info->ends = ParseListOfCoordinates(tmp, workspace_info->dimension);
	}
	return workspace_info;
}

void FreeWorkspaceInfo(WorkspaceInfo* info)
{
	free(info->obstacles.locations);
	free(info->robots);
	free(info);
}

#ifdef TEST_WS_PARSER
int main(int argc, char const *argv[])
{
	WorkspaceInfo* ws_info = ParseConfiguration(argv[1]);
	printf("Successfully parsed %s\n", argv[1]);
	FreeWorkspaceInfo(ws_info);
	return 0;
}
#endif