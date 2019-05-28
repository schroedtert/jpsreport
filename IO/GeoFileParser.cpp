/**
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 **/
//
// Created by laemmel on 30.03.16.
//

#include <filesystem>
#include "GeoFileParser.h"
#include "../tinyxml/tinyxml.h"
#include "../geometry/SubRoom.h"
#include "../geometry/WaitingArea.h"
namespace fs = std::filesystem;



GeoFileParser::GeoFileParser(Configuration* configuration)
          :_configuration(configuration)
{

}

void GeoFileParser::LoadBuilding(Building* building)
{
     //todo: what happens if any of these  methods failed (return false)? throw exception ?
     if (!LoadGeometry(building)) {
          Log->Write("ERROR:\t could not load the geometry!");
          exit(EXIT_FAILURE);
     }

     if (!LoadRoutingInfo(building)) {
          Log->Write("ERROR:\t could not load extra routing information!");
          exit(EXIT_FAILURE);
     }
     if (!LoadTrafficInfo(building)) {
          Log->Write("ERROR:\t could not load extra traffic information!");
          exit(EXIT_FAILURE);
     }
     if(!LoadTrainInfo(building))
     {
          Log->Write("ERROR:\t could not load train information!");
          exit(EXIT_FAILURE);
     }
}

bool GeoFileParser::LoadGeometry(Building* building)
{

     std::string geoFilenameWithPath = _configuration->GetProjectRootDir()+_configuration->GetGeometryFile();
     std::cout << "\nLoadGeometry: file: " << geoFilenameWithPath << "\n";

     TiXmlDocument docGeo(geoFilenameWithPath);
     if (!docGeo.LoadFile()) {
          Log->Write("ERROR: \t%s", docGeo.ErrorDesc());
          Log->Write("\t could not parse the geometry file");
          return false;
     }

     TiXmlElement* xRootNode = docGeo.RootElement();
     if (!xRootNode) {
          Log->Write("ERROR:\tRoot element does not exist");
          return false;
     }

     if (xRootNode->ValueStr()!="geometry") {
          Log->Write("ERROR:\tRoot element value is not 'geometry'.");
          return false;
     }
     if (xRootNode->Attribute("unit")) if (std::string(xRootNode->Attribute("unit"))!="m") {
          Log->Write("ERROR:\tOnly the unit m (meters) is supported. \n\tYou supplied [%s]",
                    xRootNode->Attribute("unit"));
          return false;
     }

     double version = xmltof(xRootNode->Attribute("version"), -1);

     if (version<std::stod(JPS_OLD_VERSION)) {
          Log->Write(" \tWrong geometry version!");
          Log->Write(" \tOnly version >= %s supported", JPS_OLD_VERSION);
          Log->Write(" \tPlease update the version of your geometry file to %s", JPS_OLD_VERSION);
          return false;
     }

     building->SetCaption(xmltoa(xRootNode->Attribute("caption"), "virtual building"));

     //processing the rooms node
     TiXmlNode* xRoomsNode = xRootNode->FirstChild("rooms");
     if (!xRoomsNode) {
          Log->Write("ERROR: \tThe geometry should have at least one room and one subroom");
          return false;
     }

     for (TiXmlElement* xRoom = xRoomsNode->FirstChildElement("room"); xRoom;
          xRoom = xRoom->NextSiblingElement("room")) {

          Room* room = new Room();
          //make_unique<Song>

          std::string room_id = xmltoa(xRoom->Attribute("id"), "-1");
          room->SetID(xmltoi(room_id.c_str(), -1));

          std::string caption = "room "+room_id;
          room->SetCaption(
                    xmltoa(xRoom->Attribute("caption"), caption.c_str()));

          double position = xmltof(xRoom->Attribute("zpos"), 0.0);

          //if(position>6.0) position+=50;
          room->SetZPos(position);

          //parsing the subrooms
          //processing the rooms node
          //TiXmlNode*  xSubroomsNode = xRoom->FirstChild("subroom");

          for (TiXmlElement* xSubRoom = xRoom->FirstChildElement("subroom"); xSubRoom;
               xSubRoom = xSubRoom->NextSiblingElement("subroom")) {

//               std::string subroom_id = xmltoa(xSubRoom->Attribute("id"), "-1");
               int subroom_id = xmltoi(xSubRoom->Attribute("id"), -1);

               std::string SubroomClosed = xmltoa(xSubRoom->Attribute("closed"), "0");
               std::string type = xmltoa(xSubRoom->Attribute("class"), "subroom");

               //get the equation of the plane if any
               double A_x = xmltof(xSubRoom->Attribute("A_x"), 0.0);
               double B_y = xmltof(xSubRoom->Attribute("B_y"), 0.0);

               // assume either the old "C_z" or the new "C"
               double C_z = xmltof(xSubRoom->Attribute("C_z"), 0.0);
               C_z = xmltof(xSubRoom->Attribute("C"), C_z);

               SubRoom* subroom = nullptr;

               if (type=="stair" || type=="escalator" || type=="idle_escalator") {
                    if (xSubRoom->FirstChildElement("up") == nullptr) {
                         Log->Write("ERROR:\t the attribute <up> and <down> are missing for the " + type);
                         Log->Write("ERROR:\t check your geometry file");
                         return false;
                    }
                    double up_x = xmltof(xSubRoom->FirstChildElement("up")->Attribute("px"), 0.0);
                    double up_y = xmltof(xSubRoom->FirstChildElement("up")->Attribute("py"), 0.0);
                    double down_x = xmltof(xSubRoom->FirstChildElement("down")->Attribute("py"), 0.0);
                    double down_y = xmltof(xSubRoom->FirstChildElement("down")->Attribute("py"), 0.0);
                    subroom = new Stair();
                    ((Stair *) subroom)->SetUp(Point(up_x, up_y));
                    ((Stair *) subroom)->SetDown(Point(down_x, down_y));
               } else if (type =="escalator_up") {
                    if (xSubRoom->FirstChildElement("up") == nullptr) {
                         Log->Write("ERROR:\t the attribute <up> and <down> are missing for the " + type);
                         Log->Write("ERROR:\t check your geometry file");
                         return false;
                    }
                    double up_x = xmltof(xSubRoom->FirstChildElement("up")->Attribute("px"), 0.0);
                    double up_y = xmltof(xSubRoom->FirstChildElement("up")->Attribute("py"), 0.0);
                    double down_x = xmltof(xSubRoom->FirstChildElement("down")->Attribute("py"), 0.0);
                    double down_y = xmltof(xSubRoom->FirstChildElement("down")->Attribute("py"), 0.0);
                    subroom = new Escalator();
                    ((Escalator *) subroom)->SetUp(Point(up_x, up_y));
                    ((Escalator *) subroom)->SetDown(Point(down_x, down_y));
                    ((Escalator*) subroom)->SetEscalatorUp();
                    _configuration->set_has_directional_escalators(true);
               } else if (type == "escalator_down") {
                    if (xSubRoom->FirstChildElement("up") ==nullptr) {
                         Log->Write("ERROR:\t the attribute <up> and <down> are missing for the " + type);
                         Log->Write("ERROR:\t check your geometry file");
                         return false;
                    }
                    double up_x = xmltof(xSubRoom->FirstChildElement("up")->Attribute("px"), 0.0);
                    double up_y = xmltof(xSubRoom->FirstChildElement("up")->Attribute("py"), 0.0);
                    double down_x = xmltof(xSubRoom->FirstChildElement("down")->Attribute("py"), 0.0);
                    double down_y = xmltof(xSubRoom->FirstChildElement("down")->Attribute("py"), 0.0);
                    subroom = new Escalator();
                    ((Escalator *) subroom)->SetUp(Point(up_x, up_y));
                    ((Escalator *) subroom)->SetDown(Point(down_x, down_y));
                    ((Escalator*) subroom)->SetEscalatorDown();
                    _configuration->set_has_directional_escalators(true);
               } else {
                    //normal subroom or corridor
                    subroom = new NormalSubRoom();
               }

               subroom->SetType(type);
               subroom->SetPlanEquation(A_x, B_y, C_z);
               subroom->SetRoomID(room->GetID());
               subroom->SetSubRoomID(subroom_id);

               //static int p_id=1;
               //cout<<endl<<"wall polygon: "<< p_id++<<endl;
               //looking for polygons (walls)
               for (TiXmlElement* xPolyVertices = xSubRoom->FirstChildElement("polygon"); xPolyVertices;
                    xPolyVertices = xPolyVertices->NextSiblingElement("polygon")) {

                    std::string wall_type = xmltoa(xPolyVertices->Attribute("type"), "wall");
                    for (TiXmlElement* xVertex = xPolyVertices->FirstChildElement(
                              "vertex");
                         xVertex && xVertex!=xPolyVertices->LastChild("vertex");
                         xVertex = xVertex->NextSiblingElement("vertex")) {

                         double x1 = xmltof(xVertex->Attribute("px"));
                         double y1 = xmltof(xVertex->Attribute("py"));
                         double x2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("px"));
                         double y2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("py"));
                         subroom->AddWall(Wall(Point(x1, y1), Point(x2, y2), wall_type));
                         //printf("%0.2f %0.2f %0.2f %0.2f\n",x1,y1,x2,y2);
                    }

               }

               //looking for obstacles
               for (TiXmlElement* xObstacle = xSubRoom->FirstChildElement("obstacle"); xObstacle;
                    xObstacle = xObstacle->NextSiblingElement("obstacle")) {

                    int id = xmltoi(xObstacle->Attribute("id"), -1);
                    double height = xmltof(xObstacle->Attribute("height"), 0);
                    //double ObstClosed = xmltof(xObstacle->Attribute("closed"), 0);
                    std::string ObstCaption = xmltoa(xObstacle->Attribute("caption"), "-1");

                    Obstacle* obstacle = new Obstacle();
                    obstacle->SetId(id);
                    obstacle->SetCaption(ObstCaption);
                    //obstacle->SetClosed(ObstClosed);
                    obstacle->SetHeight(height);

                    //looking for polygons (walls)
                    for (TiXmlElement* xPolyVertices = xObstacle->FirstChildElement("polygon"); xPolyVertices;
                         xPolyVertices = xPolyVertices->NextSiblingElement("polygon")) {

                         for (TiXmlElement* xVertex = xPolyVertices->FirstChildElement(
                                   "vertex");
                              xVertex && xVertex!=xPolyVertices->LastChild("vertex");
                              xVertex = xVertex->NextSiblingElement("vertex")) {

                              double x1 = xmltof(xVertex->Attribute("px"));
                              double y1 = xmltof(xVertex->Attribute("py"));
                              double x2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("px"));
                              double y2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("py"));
                              obstacle->AddWall(Wall(Point(x1, y1), Point(x2, y2)));
                         }
                    }
                    subroom->AddObstacle(obstacle);
               }
               room->AddSubRoom(subroom);

          }
          //parsing the crossings
          TiXmlNode* xCrossingsNode = xRoom->FirstChild("crossings");
          if (xCrossingsNode)
               for (TiXmlElement* xCrossing = xCrossingsNode->FirstChildElement("crossing"); xCrossing;
                    xCrossing = xCrossing->NextSiblingElement("crossing")) {

                    int id = xmltoi(xCrossing->Attribute("id"), -1);
                    int sub1_id = xmltoi(xCrossing->Attribute("subroom1_id"), -1);
                    int sub2_id = xmltoi(xCrossing->Attribute("subroom2_id"), -1);

                    double x1 = xmltof(xCrossing->FirstChildElement("vertex")->Attribute("px"));
                    double y1 = xmltof(xCrossing->FirstChildElement("vertex")->Attribute("py"));
                    double x2 = xmltof(xCrossing->LastChild("vertex")->ToElement()->Attribute("px"));
                    double y2 = xmltof(xCrossing->LastChild("vertex")->ToElement()->Attribute("py"));

                    Crossing* c = new Crossing();
                    c->SetID(id);
                    c->SetPoint1(Point(x1, y1));
                    c->SetPoint2(Point(x2, y2));

                    c->SetSubRoom1(room->GetSubRoom(sub1_id));
                    c->SetSubRoom2(room->GetSubRoom(sub2_id));
                    c->SetRoom1(room);
                    building->AddCrossing(c);

                    room->GetSubRoom(sub1_id)->AddCrossing(c);
                    room->GetSubRoom(sub2_id)->AddCrossing(c);
               }

          building->AddRoom(room);
     }

     // all rooms are read, now proceed with transitions
     TiXmlNode* xTransNode = xRootNode->FirstChild("transitions");
     if (xTransNode)
     {
          for (TiXmlElement* xTrans = xTransNode->FirstChildElement("transition"); xTrans;
               xTrans = xTrans->NextSiblingElement("transition")) {
               Transition * t = parseTransitionNode(xTrans, building);
               building->AddTransition(t);
          }
          TiXmlNode * xNodeFile = xTransNode->FirstChild("file");
          if(xNodeFile)
          {
               fs::path p(_configuration->GetProjectRootDir());
               std::string transFilename = xNodeFile->FirstChild()->ValueStr();
               p /= transFilename;
               transFilename = p.string();

               Log->Write("INFO:\tParsing transition from file <%s>", transFilename.c_str());
               TiXmlDocument docTrans(transFilename);
               if (!docTrans.LoadFile()) {
                    Log->Write("ERROR: \t%s", docTrans.ErrorDesc());
                    Log->Write("ERROR: \t could not parse the transitions file.");
                    return false;
               }
               TiXmlElement* xRootNodeTrans = docTrans.RootElement();
               if (!xRootNodeTrans) {
                    Log->Write("ERROR:\tRoot element does not exist in transitions file.");
                    return false;
               }
               if (xRootNodeTrans->ValueStr() != "JPScore") {
                    Log->Write("ERROR:\tParsing transitions file. Root element value is not 'JPScore'.");
                    return false;
               }
               TiXmlNode* xTransNodeFile = xRootNodeTrans->FirstChild("transitions");
               if (!xTransNodeFile) {
                    Log->Write("ERROR:\tNo Transitions in file found.");
                    return false;
               }
               for (TiXmlElement* xTrans = xTransNodeFile->FirstChildElement("transition"); xTrans;
               xTrans = xTrans->NextSiblingElement("transition")) {
                    Transition * t = parseTransitionNode(xTrans, building);
                    building->AddTransition(t);
               }
          }
          else{
               Log->Write("INFO:\tNot parsing transition from file %s");
          }
          Log->Write("INFO:\tGot %d transitions", building-> GetAllTransitions().size());

     }// xTransNode
     Log->Write("INFO: \tLoading building file successful!!!\n");

     //everything went fine
     return true;
}

bool GeoFileParser::LoadRoutingInfo(Building* building)
{
     //TODO read schedule

     TiXmlDocument docRouting(_configuration->GetProjectFile());
     if (!docRouting.LoadFile()) {
          Log->Write("ERROR: \t%s", docRouting.ErrorDesc());
          Log->Write("ERROR: \t could not parse the routing file");
          return false;
     }

     TiXmlElement* xRootNode = docRouting.RootElement();
     if (!xRootNode) {
          Log->Write("ERROR:\tRoot element does not exist");
          return false;
     }

     if (!xRootNode->FirstChild("routing")) {
          return true; // no extra routing information
     }


     //load goals and routes
     TiXmlNode* xGoalsNode = xRootNode->FirstChild("routing")->FirstChild("goals");
     if (xGoalsNode) {
          // ---- parse goals from inifile
          for (TiXmlElement* e = xGoalsNode->FirstChildElement("goal"); e;
               e = e->NextSiblingElement("goal")) {
               Goal * goal = parseGoalNode(e);
               if (goal) {
                    building->AddGoal(goal);
                    _configuration->GetRoutingEngine()->AddFinalDestinationID(goal->GetId());
               }

          }

          // ---- parse waiting areas from inifile
          for (TiXmlElement* e = xGoalsNode->FirstChildElement("waiting_area"); e;
               e = e->NextSiblingElement("waiting_area")) {
               Goal * goal = parseWaitingAreaNode(e);
               if (goal) {
                    building->AddGoal(goal);
                    _configuration->GetRoutingEngine()->AddFinalDestinationID(goal->GetId());
               }

          }
          // ---- parse goals/waiting areas from external file
          TiXmlNode* xGoalsNodeFile = xGoalsNode->FirstChild("file");
          if(xGoalsNodeFile)
          {
               fs::path p(_configuration->GetProjectRootDir());
               std::string goalFilename = xGoalsNodeFile->FirstChild()->ValueStr();
               p /= goalFilename;
               goalFilename = p.string();
               Log->Write("INFO:\tGoal file <%s> will be parsed", goalFilename.c_str());
               TiXmlDocument docGoal(goalFilename);
               if (!docGoal.LoadFile()) {
                    Log->Write("ERROR: \t%s", docGoal.ErrorDesc());
                    Log->Write("ERROR: \t could not parse the goal file.");
                    return false;
               }
               TiXmlElement* xRootNodeGoal = docGoal.RootElement();
               if (!xRootNodeGoal) {
                    Log->Write("ERROR:\tRoot element does not exist in goal file.");
                    return false;
               }
               if (xRootNodeGoal->ValueStr() != "JPScore") {
                    Log->Write("ERROR:\tParsing goal file. Root element value is not 'JPScore'.");
                    return false;
               }
               TiXmlNode* xGoal = xRootNodeGoal->FirstChild("goals");
               if (!xGoal) {
                    Log->Write("ERROR:\tNo Goals in file found.");
                    return false;
               }
               for (TiXmlElement* e = xGoal->FirstChildElement("goal"); e;
                    e = e->NextSiblingElement("goal")) {
                    Goal * goal = parseGoalNode(e);
                    if (goal) {
                         building->AddGoal(goal);
                         _configuration->GetRoutingEngine()->AddFinalDestinationID(goal->GetId());
                    }
               }

               for (TiXmlElement* e = xGoal->FirstChildElement("waiting_area"); e;
                    e = e->NextSiblingElement("waiting_area")) {
                    Goal * goal = parseWaitingAreaNode(e);
                    if (goal) {
                         building->AddGoal(goal);
                         _configuration->GetRoutingEngine()->AddFinalDestinationID(goal->GetId());
                    }
               }


          }
          else
               Log->Write("INFO:\tGoal file not parsed");
     } //xgoalsNode

     Log->Write("INFO:\tdone with loading extra routing information");
     return true;
}

bool GeoFileParser::parseDoorNode(TiXmlElement * xDoor, int id, Building* building)
{
     bool result = false;// this return value is not needed.
                         // maybe in the future it might be...
     std::string str("INFO:\tParsed Door: \n");
     char tmp[100];
     sprintf(tmp, "\t>> ID: %d\n", id);
     str.append(tmp);
     //------------------ state
     std::string stateStr = xmltoa(xDoor->Attribute("state"), "open");
     DoorState state = StringToDoorState(stateStr);
     //store transition in a map and call getTransition/getCrossing
     switch (state) {
     case DoorState::OPEN:
          building->GetTransition(id)->Open();
          break;
     case DoorState::CLOSE:
          building->GetTransition(id)->Close();
          break;
     case DoorState::TEMP_CLOSE:
          building->GetTransition(id)->TempClose();
          break;
     case DoorState::Error:
          Log->Write("WARNING:\t Unknown door state: <%s>. open or close. Default: open",
                    stateStr.c_str());
          building->GetTransition(id)->Open();
          break;
     }

     sprintf(tmp, "\t>> state: %s\n", stateStr.c_str());
     str.append(tmp);
     //------------------ outflow
     double outflow = xmltof(xDoor->Attribute("outflow"), -1.0);
     if(outflow >= 0)
     {
          building->GetTransition(id)->SetOutflowRate(outflow);
          sprintf(tmp, "\t>> ouflow: %.2f\n", outflow);
          str.append(tmp);
     }
     //----------------- dt
     double DT = xmltof(xDoor->Attribute("dt"), -1.0);
     if(DT >= 0)
     {
          building->GetTransition(id)->SetDT(DT);
     }
     //----------------- dn
     int DN = xmltof(xDoor->Attribute("dn"), -1.0);
     if(DN >= 0)
     {
          building->GetTransition(id)->SetDN(DN);
          sprintf(tmp, "\t>> dn: %d\n", DN);
          str.append(tmp);
     }

     //------------------ max door usage
     int mdu = xmltof(xDoor->Attribute("max_agents"), -1);
     if(mdu >= 0)
     {
          building->GetTransition(id)->SetMaxDoorUsage(mdu);
          sprintf(tmp, "\t>> max_agents: %d\n", mdu);
          str.append(tmp);
     }
     //-----------------
     Log->Write(str);
     result = true;
     return result;
}

bool GeoFileParser::LoadTrafficInfo(Building* building)
{

     Log->Write("INFO:\tLoading the traffic info");

     TiXmlDocument doc(_configuration->GetProjectFile());
     if (!doc.LoadFile()) {
          Log->Write("ERROR: \t%s", doc.ErrorDesc());
          Log->Write("ERROR: \t could not parse the project file");
          return false;
     }

     TiXmlNode* xRootNode = doc.RootElement()->FirstChild("traffic_constraints");
     if (!xRootNode) {
          Log->Write("WARNING:\tcould not find any traffic information");
          return true;
     }

     //processing the rooms node
     TiXmlNode* xRoomsNode = xRootNode->FirstChild("rooms");
     if (xRoomsNode)
          for (TiXmlElement* xRoom = xRoomsNode->FirstChildElement("room"); xRoom;
               xRoom = xRoom->NextSiblingElement("room")) {

               double id = xmltof(xRoom->Attribute("room_id"), -1);
               std::string state = xmltoa(xRoom->Attribute("state"), "good");
               RoomState status = (state=="good") ? ROOM_CLEAN : ROOM_SMOKED;
               building->GetRoom(id)->SetState(status);
          }
     else
          Log->Write("Info:\t  no room info found in inifile");

     //processing the doors node
     TiXmlNode* xDoorsNode = xRootNode->FirstChild("doors");
     if(xDoorsNode)
     {
          bool res_parseDoor;
          for (TiXmlElement* xDoor = xDoorsNode->FirstChildElement("door"); xDoor;
               xDoor = xDoor->NextSiblingElement("door")) {
               int id = xmltoi(xDoor->Attribute("trans_id"), -1);
               if (id!=-1 && building->GetTransition(id)) {
                    res_parseDoor = parseDoorNode(xDoor, id, building);
                    if(!res_parseDoor)
                         return false;
               }
          }//for xDoor
     }
     else
          Log->Write("Info:\t  no door info found in inifile");
     // processing file node
     TiXmlNode* xFileNode = xRootNode->FirstChild("file");
     if(xFileNode)
     {
          fs::path p(_configuration->GetProjectRootDir());
          std::string trafficFilename = xFileNode->FirstChild()->ValueStr();
          p /= trafficFilename;
          trafficFilename = p.string();
          Log->Write("Info:\t  traffic file found <%s>", trafficFilename.c_str());
          TiXmlDocument docTraffic(trafficFilename);
          if (!docTraffic.LoadFile()) {
               Log->Write("ERROR: \t%s", docTraffic.ErrorDesc());
               Log->Write("ERROR: \t could not parse the traffic file.");
               return false;
          }
          TiXmlElement* xRootNodeTraffic = docTraffic.RootElement();
          if (!xRootNodeTraffic) {
               Log->Write("ERROR:\tRoot element does not exist.");
               return false;
          }

          if (xRootNodeTraffic->ValueStr() != "JPScore") {
               Log->Write("ERROR:\tRoot element value is not 'JPScore'.");
               return false;
          }
          TiXmlNode* xTraffic = xRootNodeTraffic->FirstChild("traffic_constraints");
          if (!xTraffic) {
               Log->Write("ERROR:\tNo traffic constraints in file found.");
               return false;
          }
          TiXmlNode* xDoorsNodeF = xTraffic->FirstChild("doors");
          if(xDoorsNodeF)
          {
               bool res_parseDoor;
               for (TiXmlElement* xDoor = xDoorsNodeF->FirstChildElement("door"); xDoor;
                    xDoor = xDoor->NextSiblingElement("door")) {
                    int id = xmltoi(xDoor->Attribute("trans_id"), -1);
                    if (id!=-1 && building->GetTransition(id)) {
                         res_parseDoor = parseDoorNode(xDoor, id, building);
                         if(!res_parseDoor)
                              return false;
                    }
               }//for xDoor
          }
          else
               Log->Write("Info:\t  no door info found in traffic file");
     }
     else
          Log->Write("Info:\t  no traffic file found.");

     Log->Write("INFO:\tDone with loading traffic info file");
     return true;
}

Transition* GeoFileParser::parseTransitionNode(TiXmlElement * xTrans, Building * building)
{
     int id = xmltoi(xTrans->Attribute("id"), -1);
     // string caption = "door " + id;
     std::string caption = "door ";
     caption += std::to_string(id);
     caption = xmltoa(xTrans->Attribute("caption"), caption.c_str());
     int room1_id = xmltoi(xTrans->Attribute("room1_id"), -1);
     int room2_id = xmltoi(xTrans->Attribute("room2_id"), -1);
     int subroom1_id = xmltoi(xTrans->Attribute("subroom1_id"), -1);
     int subroom2_id = xmltoi(xTrans->Attribute("subroom2_id"), -1);
     std::string type = xmltoa(xTrans->Attribute("type"), "normal");

     double x1 = xmltof(xTrans->FirstChildElement("vertex")->Attribute("px"));
     double y1 = xmltof(xTrans->FirstChildElement("vertex")->Attribute("py"));

     double x2 = xmltof(xTrans->LastChild("vertex")->ToElement()->Attribute("px"));
     double y2 = xmltof(xTrans->LastChild("vertex")->ToElement()->Attribute("py"));

     Transition* t = new Transition();
     t->SetID(id);
     t->SetCaption(caption);
     t->SetPoint1(Point(x1, y1));
     t->SetPoint2(Point(x2, y2));
     t->SetType(type);
     //--- danger area
     if (room1_id!=-1 && subroom1_id!=-1) {
          //Room* room = _rooms[room1_id];
          Room* room = building->GetRoom(room1_id);
          SubRoom* subroom = room->GetSubRoom(subroom1_id);

          //subroom->AddGoalID(t->GetUniqueID());
          //MPI
          room->AddTransitionID(t->GetUniqueID());
          t->SetRoom1(room);
          t->SetSubRoom1(subroom);

          //new implementation
          subroom->AddTransition(t);
     }
     if (room2_id!=-1 && subroom2_id!=-1) {
          Room* room = building->GetRoom(room2_id);
          SubRoom* subroom = room->GetSubRoom(subroom2_id);
          //subroom->AddGoalID(t->GetUniqueID());
          //MPI
          room->AddTransitionID(t->GetUniqueID());
          t->SetRoom2(room);
          t->SetSubRoom2(subroom);

          //new implementation
          subroom->AddTransition(t);
     }
     return t;
}

Goal* GeoFileParser::parseGoalNode(TiXmlElement * e)
{
     Log->Write("INFO:\tLoading goal");
     int id = xmltoi(e->Attribute("id"), -1);
     int isFinal = std::string(e->Attribute("final"))=="true" ? true : false;
     std::string caption = xmltoa(e->Attribute("caption"), "-1");
     int room_id = xmltoi(e->Attribute("room_id"), -1);
     int subroom_id = xmltoi(e->Attribute("subroom_id"), -1);
     Log->Write("INFO:\t  Goal id: %d", id);
     Log->Write("INFO:\t  Goal caption: %s", caption.c_str());
     Log->Write("INFO:\t  Goal room_id: %d", room_id);
     Log->Write("INFO:\t  Goal subroom_id: %d", subroom_id);
     Goal* goal = new Goal();
     goal->SetId(id);
     goal->SetCaption(caption);
     goal->SetIsFinalGoal(isFinal);
     goal->SetRoomID(room_id);
     goal->SetSubRoomID(subroom_id);

     //looking for polygons (walls)
     for (TiXmlElement* xPolyVertices = e->FirstChildElement("polygon"); xPolyVertices;
          xPolyVertices = xPolyVertices->NextSiblingElement("polygon")) {

          for (TiXmlElement* xVertex = xPolyVertices->FirstChildElement(
                    "vertex");
               xVertex && xVertex!=xPolyVertices->LastChild("vertex");
               xVertex = xVertex->NextSiblingElement("vertex")) {

               double x1 = xmltof(xVertex->Attribute("px"));
               double y1 = xmltof(xVertex->Attribute("py"));
               double x2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("px"));
               double y2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("py"));
               goal->AddWall(Wall(Point(x1, y1), Point(x2, y2)));
          }
     }

     if (!goal->ConvertLineToPoly())
     {
          Log->Write("ERROR:\t parsing polygon of goal %d", id);
          return nullptr;
     }
     Log->Write("INFO:\t  finished parsing goal %d", id);
     return goal;
}

Goal* GeoFileParser::parseWaitingAreaNode(TiXmlElement * e)
{
     Log->Write("INFO:\tLoading Waiting Area");
     int id = xmltoi(e->Attribute("id"), -1);
     int min_peds = xmltoi(e->Attribute("min_peds"), -1);
     int max_peds = xmltoi(e->Attribute("max_peds"), -1);
     int waiting_time = xmltoi(e->Attribute("waiting_time"), -1);
     int transition_id = xmltoi(e->Attribute("transition_id"), -1);
     int room_id = xmltoi(e->Attribute("room_id"), -1);
     int subroom_id = xmltoi(e->Attribute("subroom_id"), -1);
     bool open = strcmp(xmltoa(e->Attribute("is_open"), "false"), "true") == 0;
     bool global_timer = strcmp(xmltoa(e->Attribute("global_timer"), "false"), "true") == 0;

     std::string caption = xmltoa(e->Attribute("caption"), "-1");

     Log->Write("INFO:\t  Goal id: %d", id);
     Log->Write("INFO:\t  Goal caption: %s", caption.c_str());
     Log->Write("INFO:\t  Goal room_id: %d", room_id);
     Log->Write("INFO:\t  Goal subroom_id: %d", subroom_id);

     Goal* wa = new WaitingArea();
     WaitingArea* waitingArea = static_cast<WaitingArea*>(wa);
     waitingArea->SetIsFinalGoal(0);
     waitingArea->SetId(id);
     waitingArea->SetCaption(caption);
     waitingArea->setOpen(open);
     waitingArea->setGlobalTimer(global_timer);
     waitingArea->setMinNumPed(min_peds);
     waitingArea->setMaxNumPed(max_peds);
     waitingArea->setWaitingTime(waiting_time);
     waitingArea->setTransitionID(transition_id);
     waitingArea->SetRoomID(room_id);
     waitingArea->SetSubRoomID(subroom_id);

     std::map<int, double> nextGoals;


     //looking for next_wa
     for (TiXmlElement* nextWa = e->FirstChildElement("next_wa"); nextWa;
          nextWa = nextWa->NextSiblingElement("next_wa")) {
          int nextWaId = xmltoi(nextWa->Attribute("id"));
          double nextWaP = xmltof(nextWa->Attribute("p"));

          nextGoals.insert(std::pair<int, double>(nextWaId, nextWaP));
     }

     if (!waitingArea->setNextGoals(nextGoals)){
          Log->Write("ERROR:\t  check probabilties of next goal of WA  %d", id);
          return nullptr;
     };


     //looking for polygons (walls)
     for (TiXmlElement* xPolyVertices = e->FirstChildElement("polygon"); xPolyVertices;
          xPolyVertices = xPolyVertices->NextSiblingElement("polygon")) {

          for (TiXmlElement* xVertex = xPolyVertices->FirstChildElement(
                    "vertex");
               xVertex && xVertex!=xPolyVertices->LastChild("vertex");
               xVertex = xVertex->NextSiblingElement("vertex")) {

               double x1 = xmltof(xVertex->Attribute("px"));
               double y1 = xmltof(xVertex->Attribute("py"));
               double x2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("px"));
               double y2 = xmltof(xVertex->NextSiblingElement("vertex")->Attribute("py"));
               wa->AddWall(Wall(Point(x1, y1), Point(x2, y2)));
          }
     }

     if (!waitingArea->ConvertLineToPoly()){
          Log->Write("ERROR:\t   parsing polygon of waiting area %d", id);
          return nullptr;
     }

     Log->Write("INFO:\t  finished parsing waiting area %d", id);

     return wa;
}

bool GeoFileParser::LoadTrainInfo(Building* building)
{
     Log->Write("--------\nINFO:\tLoading the train info");

     TiXmlDocument doc(_configuration->GetProjectFile());
     if (!doc.LoadFile()) {
          Log->Write("ERROR: \t%s", doc.ErrorDesc());
          Log->Write("ERROR: \t could not parse the project file");
          return false;
     }
     TiXmlElement* xRootNode = doc.RootElement();
     if (!xRootNode) {
          Log->Write("ERROR:\tRoot element does not exist");
          return false;
     }
     if (!xRootNode->FirstChild("train_constraints")) {
          Log->Write("WARNING:\tNo train constraints were found. Continue.");
     }
     bool resTTT = true;
     bool resType = true;
     if(xRootNode->FirstChild("train_constraints"))
     {
          resTTT = LoadTrainTimetable(building, xRootNode);
          resType = LoadTrainType(building, xRootNode);
     }
     return (resTTT && resType);
}
bool GeoFileParser::LoadTrainTimetable(Building* building, TiXmlElement * xRootNode)
{
     TiXmlNode* xTTTNode = xRootNode->FirstChild("train_constraints")->FirstChild("train_time_table");
     std::string TTTFilename;
     if(xTTTNode)
     {
          fs::path p(_configuration->GetProjectRootDir());
          TTTFilename = xTTTNode->FirstChild()->ValueStr();
          p /= TTTFilename;
          TTTFilename = p.string();
          Log->Write("INFO:\tTrain Timetable file <%s> will be parsed", TTTFilename.c_str());
     }
     else return true;


     TiXmlDocument docTTT(TTTFilename);
     if (!docTTT.LoadFile()) {
          Log->Write("ERROR: \t%s", docTTT.ErrorDesc());
          Log->Write("ERROR: \t could not parse the train timetable file.");
          return false;
     }
     TiXmlElement* xTTT = docTTT.RootElement();
     if (!xTTT) {
          Log->Write("ERROR:\tRoot element does not exist in TTT file.");
          return false;
     }
     if (xTTT->ValueStr() != "train_time_table") {
          Log->Write("ERROR:\tParsing train timetable file. Root element value is not 'train_time_table'.");
          return false;
     }
     for (TiXmlElement* e = xTTT->FirstChildElement("train"); e;
                    e = e->NextSiblingElement("train")) {
          std::shared_ptr<TrainTimeTable> TTT = parseTrainTimeTableNode(e);

          if (TTT) { // todo: maybe get pointer to train
               building->AddTrainTimeTable(TTT);
          }
     }
     return true;
}
bool GeoFileParser::LoadTrainType(Building* building, TiXmlElement * xRootNode)
{
     TiXmlNode* xTTNode = xRootNode->FirstChild("train_constraints")->FirstChild("train_types");
     std::string TTFilename;
     if(xTTNode)
     {
          fs::path p(_configuration->GetProjectRootDir());
          TTFilename = xTTNode->FirstChild()->ValueStr();
          p /= TTFilename;
          TTFilename = p.string();
          Log->Write("INFO:\tTrain Type file <%s> will be parsed", TTFilename.c_str());
     }
     else return true;


     TiXmlDocument docTT(TTFilename);
     if (!docTT.LoadFile()) {
          Log->Write("ERROR: \t%s", docTT.ErrorDesc());
          Log->Write("ERROR: \t could not parse the train type file.");
          return false;
     }
     TiXmlElement* xTT = docTT.RootElement();
     if (!xTT) {
          Log->Write("ERROR:\tRoot element does not exist in TT file.");
          return false;
     }
     if (xTT->ValueStr() != "train_type") {
          Log->Write("ERROR:\tParsing train type file. Root element value is not 'train_type'.");
          return false;
     }
     for (TiXmlElement* e = xTT->FirstChildElement("train"); e;
                    e = e->NextSiblingElement("train")) {
          std::shared_ptr<TrainType> TT = parseTrainTypeNode(e);
          if (TT) {
               building->AddTrainType(TT);
          }
     }
     return true;

}

std::shared_ptr<TrainTimeTable> GeoFileParser::parseTrainTimeTableNode(TiXmlElement * e)
{
     Log->Write("INFO:\tLoading train time table NODE");
     std::string caption = xmltoa(e->Attribute("caption"), "-1");
     int id = xmltoi(e->Attribute("id"), -1);
     std::string type = xmltoa(e->Attribute("type"), "-1");
     int room_id = xmltoi(e->Attribute("room_id"), -1);
     int subroom_id = xmltoi(e->Attribute("subroom_id"), -1);
     int platform_id = xmltoi(e->Attribute("platform_id"), -1);
     float track_start_x = xmltof(e->Attribute("track_start_x"), -1);
     float track_start_y = xmltof(e->Attribute("track_start_y"), -1);
     float track_end_x = xmltof(e->Attribute("track_end_x"), -1);
     float track_end_y = xmltof(e->Attribute("track_end_y"), -1);

     float train_start_x = xmltof(e->Attribute("train_start_x"),-1);
     float train_start_y = xmltof(e->Attribute("train_start_y"), -1);
     float train_end_x = xmltof(e->Attribute("train_end_x"), -1);
     float train_end_y = xmltof(e->Attribute("train_end_y"), -1);

     float arrival_time = xmltof(e->Attribute("arrival_time"), -1);
     float departure_time = xmltof(e->Attribute("departure_time"), -1);
     Log->Write("INFO:\tTrain time table:");
     Log->Write("INFO:\t   id: %d", id);
     Log->Write("INFO:\t   type: %s", type.c_str());
     Log->Write("INFO:\t   room_id: %d", room_id);
     Log->Write("INFO:\t   subroom_id: %d", subroom_id);
     Log->Write("INFO:\t   platform_id: %d", platform_id);
     Log->Write("INFO:\t   track_start: [%.2f, %.2f]", track_start_x, track_start_y);
     Log->Write("INFO:\t   track_end: [%.2f, %.2f]", track_end_x, track_end_y);
     Log->Write("INFO:\t   arrival_time: %.2f", arrival_time);
     Log->Write("INFO:\t   departure_time: %.2f", departure_time);
     Point track_start(track_start_x, track_start_y);
     Point track_end(track_end_x, track_end_y);
     Point train_start(train_start_x, train_start_y);
     Point train_end(train_end_x, train_end_y);
     std::shared_ptr<TrainTimeTable> trainTimeTab = std::make_shared<TrainTimeTable>(
          TrainTimeTable{
                    id,
                    type,
                    room_id,
                    subroom_id,
                    arrival_time,
                    departure_time,
                    track_start,
                    track_end,
                    train_start,
                    train_end,
                    platform_id,
                    });

     return trainTimeTab;
}

std::shared_ptr<TrainType> GeoFileParser::parseTrainTypeNode(TiXmlElement * e)
{
     Log->Write("INFO:\tLoading train type");
     // int T_id = xmltoi(e->Attribute("id"), -1);
     std::string type = xmltoa(e->Attribute("type"), "-1");
     int agents_max = xmltoi(e->Attribute("agents_max"), -1);
     float length = xmltof(e->Attribute("length"), -1);
     // std::shared_ptr<Transition> t = new Transition();
     // std::shared_ptr<Transition> doors;
     Transition t;
     std::vector<Transition> doors;

     for (TiXmlElement* xDoor = e->FirstChildElement("door"); xDoor;
          xDoor = xDoor->NextSiblingElement("door")) {
          int D_id = xmltoi(xDoor->Attribute("id"), -1);
          float x1 = xmltof(xDoor->FirstChildElement("vertex")->Attribute("px"), -1);
          float y1 = xmltof(xDoor->FirstChildElement("vertex")->Attribute("py"), -1);
          float x2 = xmltof(xDoor->LastChild("vertex")->ToElement()->Attribute("px"), -1);
          float y2 = xmltof(xDoor->LastChild("vertex")->ToElement()->Attribute("py"), -1);
          Point start(x1, y1);
          Point end(x2, y2);
          float outflow = xmltof(xDoor->Attribute("outflow"), -1);
          float dn = xmltoi(xDoor->Attribute("dn"), -1);
          t.SetID(D_id);
          t.SetCaption(type + std::to_string(D_id));
          t.SetPoint1(start);
          t.SetPoint2(end);
          t.SetOutflowRate(outflow);
          t.SetDN(dn);
          doors.push_back(t);
     }
     Log->Write("INFO:\t   type: %s", type.c_str());
     Log->Write("INFO:\t   capacity: %d", agents_max);
     Log->Write("INFO:\t   number of doors: %d", doors.size());
     for(auto d: doors)
     {
          Log->Write("INFO\t      door (%d): %s | %s", d.GetID(), d.GetPoint1().toString().c_str(), d.GetPoint2().toString().c_str());
     }

     std::shared_ptr<TrainType> Type = std::make_shared<TrainType>(
          TrainType{
                    type,
                    agents_max,
                    length,
                    doors,
                    });
   return Type;

}

GeoFileParser::~GeoFileParser()
{

}
