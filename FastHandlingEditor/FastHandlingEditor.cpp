#include <boost/program_options.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../tinyxml2/tinyxml2.h"

namespace po = boost::program_options;

void handle_eptr(std::exception_ptr eptr) // passing by value is ok
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\n";
	}
}

void changeParamsCars(tinyxml2::XMLElement *elem) {
	float fInitialDragCoeff;
	float fDriveInertia;
	float fInitialDriveMaxFlatVel;
	float fLowSpeedTractionLossMult;
	float fTractionBiasFront;
	float fSuspensionForce;
	float fInitialDriveForce;

	elem->FirstChildElement("fInitialDragCoeff")->QueryAttribute("value", &fInitialDragCoeff);
	elem->FirstChildElement("fDriveInertia")->QueryAttribute("value", &fDriveInertia);
	elem->FirstChildElement("fInitialDriveMaxFlatVel")->QueryAttribute("value", &fInitialDriveMaxFlatVel);
	elem->FirstChildElement("fLowSpeedTractionLossMult")->QueryAttribute("value", &fLowSpeedTractionLossMult);
	elem->FirstChildElement("fTractionBiasFront")->QueryAttribute("value", &fTractionBiasFront);
	elem->FirstChildElement("fSuspensionForce")->QueryAttribute("value", &fSuspensionForce);
	elem->FirstChildElement("fInitialDriveForce")->QueryAttribute("value", &fInitialDriveForce);

	fInitialDragCoeff = (fInitialDragCoeff * 1.3) + (fInitialDriveForce * 15)		;
	fDriveInertia = (fDriveInertia * 0.2) + 0.8										;
	fInitialDriveMaxFlatVel = (fInitialDriveMaxFlatVel * 0.60) + 48					;
	fLowSpeedTractionLossMult = fLowSpeedTractionLossMult * 1.4						;
	fTractionBiasFront = fTractionBiasFront + ((0.5 - fTractionBiasFront) * 0.014)	;
	fSuspensionForce = fSuspensionForce * 0.93										;
	fInitialDriveForce = fInitialDriveForce + ((0.65 - fInitialDriveForce) * 0.028)	;

	elem->FirstChildElement("fInitialDragCoeff")->SetAttribute("value", fInitialDragCoeff);
	elem->FirstChildElement("fDriveInertia")->SetAttribute("value", fDriveInertia);
	elem->FirstChildElement("fInitialDriveMaxFlatVel")->SetAttribute("value", fInitialDriveMaxFlatVel);
	elem->FirstChildElement("fLowSpeedTractionLossMult")->SetAttribute("value", fLowSpeedTractionLossMult);
	elem->FirstChildElement("fTractionBiasFront")->SetAttribute("value", fTractionBiasFront);
	elem->FirstChildElement("fSuspensionForce")->SetAttribute("value", fSuspensionForce);
	elem->FirstChildElement("fInitialDriveForce")->SetAttribute("value", fInitialDriveForce);
}

void changeParamsBike(tinyxml2::XMLElement *elem) {
	float fInitialDragCoeff;
	float fInitialDriveMaxFlatVel;
	float fBrakeBiasFront;
	float fInitialDriveForce;

	elem->FirstChildElement("fInitialDragCoeff")->QueryAttribute("value", &fInitialDragCoeff);
	elem->FirstChildElement("fInitialDriveMaxFlatVel")->QueryAttribute("value", &fInitialDriveMaxFlatVel);
	elem->FirstChildElement("fBrakeBiasFront")->QueryAttribute("value", &fBrakeBiasFront);
	elem->FirstChildElement("fInitialDriveForce")->QueryAttribute("value", &fInitialDriveForce);

	fInitialDragCoeff = (fInitialDragCoeff * 0.9) + (fInitialDriveForce * 8) - 0.9 ; 
	fInitialDriveMaxFlatVel = (fInitialDriveMaxFlatVel * 0.60) + 53				   ; 
	fBrakeBiasFront = fBrakeBiasFront - 0.02									   ; 
	fInitialDriveForce = fInitialDriveForce * 0.89								   ; 

	elem->FirstChildElement("fInitialDragCoeff")->SetAttribute("value", fInitialDragCoeff);
	elem->FirstChildElement("fInitialDriveMaxFlatVel")->SetAttribute("value", fInitialDriveMaxFlatVel);
	elem->FirstChildElement("fBrakeBiasFront")->SetAttribute("value", fBrakeBiasFront);
	elem->FirstChildElement("fInitialDriveForce")->SetAttribute("value", fInitialDriveForce);
}

int main(int argc, char *argv[])
{
	std::string vehiclesFilePath;
	std::vector<std::string> cars;
	std::vector<std::string> bikes;

	std::vector<std::string> processedCars;
	std::vector<std::string> processedBikes;

	std::string handlingFilePath;
	tinyxml2::XMLDocument doc;

	std::exception_ptr eptr;
	po::options_description desc("handling.meta parameter util");
	desc.add_options()
		("help", "Show this help")
		("handling,h", po::value<std::string>(), "handling.meta file")
		("list,l", po::value<std::string>(), "Vehicle list");
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help") || argc == 1) {
			std::cout << desc << "\n";
			return 0;
		}

		if (vm.count("handling")) {
			handlingFilePath = vm["handling"].as<std::string>();
		} else {
			std::cout << "No handling.meta specified\n";
			return 1;
		}

		if (vm.count("list")) {
			vehiclesFilePath = vm["list"].as<std::string>();
		} else {
			std::cout << "No vehicle list specified\n";
			return 1;
		}

	}
	catch (...) {
		eptr = std::current_exception();
		handle_eptr(eptr);
	}

	// TODO: Maybe refactor vehicle list?????????
	std::ifstream vehiclesFile(vehiclesFilePath);
	if (vehiclesFile.is_open()) {
		std::string str;
		while (std::getline(vehiclesFile, str)) {
			if (str != "") {
				auto pos = str.find("B_");
				if (pos != std::string::npos) {
					bikes.push_back(str.substr(pos+2, std::string::npos));
					//std::cout << str.substr(0, pos);
				} else {
					cars.push_back(str);
				}
			}
		}
	}

	// TODO: Refactor XML parsing
	tinyxml2::XMLError err = doc.LoadFile(handlingFilePath.c_str());
	int err2 = 0;

	if (err != tinyxml2::XML_SUCCESS) {
		std::cout << "Can't load " << handlingFilePath << "\n";
	}

	tinyxml2::XMLNode * pRoot = doc.FirstChildElement("CHandlingDataMgr")->FirstChildElement("HandlingData");
	if (!pRoot) {
		std::cout << handlingFilePath << " read error\n";
		return 1;
	}

	for (auto e = pRoot->FirstChildElement("Item"); e != nullptr; e = e->NextSiblingElement("Item")) {
		std::string currName = e->FirstChildElement("handlingName")->GetText();
		//std::cout << e->FirstChildElement("handlingName")->GetText() << "\n";
		for (auto v : cars) {
			if (boost::iequals(v, currName)) {
				processedCars.push_back(v);
				changeParamsCars(e);
			}
		}
		for (auto b : bikes) {
			if (boost::iequals(b, currName)) {
				processedBikes.push_back(b);
				changeParamsBike(e);
			}
		}
	}

	if (processedCars.size() != cars.size()) {
		std::sort(cars.begin(), cars.end());
		std::sort(processedCars.begin(), processedCars.end());
		std::vector<std::string> missing;
		std::set_difference(
			cars.begin(), cars.end(), 
			processedCars.begin(), processedCars.end(), std::back_inserter(missing));
		
		std::cout << "\nFollowing handling line(s) not found: (cars)\n";// << "\n"
		for (auto m : missing) {
			std::cout << m << "\n";
		}
	}

	if (processedBikes.size() != bikes.size()) {
		std::sort(bikes.begin(), bikes.end());
		std::sort(processedBikes.begin(), processedBikes.end());
		std::vector<std::string> missing;
		std::set_difference(
			bikes.begin(), bikes.end(),
			processedBikes.begin(), processedBikes.end(), std::back_inserter(missing));

		std::cout << "\nFollowing handling line(s) not found: (bikes)\n";// << "\n"
		for (auto m : missing) {
			std::cout << m << "\n";
		}
	}

	doc.SaveFile("result.meta");
	std::cout << "Saved changes to result.meta\n";
    return 0;
}

