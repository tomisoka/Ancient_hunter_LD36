#include "unlock.h"

vector<int> weaponsUnlocked;
vector<int> devicesUnlocked; 
vector<int> statsUnlocked; 


char unlock_file[] = "unlocks.xml";
void setup_unlock(){
	uint32_t i;
	weaponsUnlocked.push_back(1);
	weaponsUnlocked.push_back(0);
	weaponsUnlocked.push_back(0);
	weaponsUnlocked.push_back(0);
	weaponsUnlocked.push_back(0);
	weaponsUnlocked.push_back(0);

	devicesUnlocked.push_back(0);
	devicesUnlocked.push_back(0);

	statsUnlocked.push_back(10);
	statsUnlocked.push_back(1000);

	xml_document doc;
	if(!doc.load_file(unlock_file)){
		return;
	}



	xml_node weaponsN = doc.child("weapons");
	if(weaponsN)for(xml_node weapN: weaponsN.children("weap")){
		xml_attribute IDN = weapN.attribute("ID");
		int ID = IDN.as_int();
		weaponsUnlocked[ID] = 1;
	}

	xml_node devicesN = doc.child("devices");
	i=0;
	if(devicesN)for(xml_node devN: devicesN.children("dev")){
		xml_attribute numN = devN.attribute("num");
		int num = numN.as_int();
		devicesUnlocked[i] = num;
		i++;
	}


	xml_node statsN = doc.child("stats");
	i=0;
	if(statsN)for(xml_node statN: statsN.children("stat")){
		xml_attribute numN = statN.attribute("num");
		int num = numN.as_int();
		statsUnlocked[i] = num;
		i++;
	}

}

void cleanup_unlock(){
	uint32_t i;
	xml_document doc;

	xml_node weaponsN = doc.append_child("weapons");
	for(i=0;i<weaponsUnlocked.size();++i){
		if(weaponsUnlocked[i]){
			xml_node weaponN = weaponsN.append_child("weap");
			xml_attribute IDN = weaponN.append_attribute("ID");

			IDN.set_value(i);
		}
	}


	xml_node devicesN = doc.append_child("devices");
	for(i=0;i<devicesUnlocked.size();++i){
		xml_node devN = devicesN.append_child("dev");
		xml_attribute numN = devN.append_attribute("num");

		numN.set_value(devicesUnlocked[i]);
	}

	xml_node statsN = doc.append_child("stats");
	for(i=0;i<statsUnlocked.size();++i){
		xml_node statN = statsN.append_child("stat");
		xml_attribute numN = statN.append_attribute("num");

		numN.set_value(statsUnlocked[i]);
	}



	doc.save_file(unlock_file);
}
