#include "AttackInfo.h"
#include "CsvReader.h"

#include "DxLib.h"


using namespace std;


/*
* 1マス分の攻撃情報
*/
AttackElement::AttackElement(int damage, int dx, int dy) {
	m_damage = damage;
	m_dx = dx;
	m_dy = dy;
}


/*
* 攻撃情報
*/
AttackInfo::AttackInfo() {

}


AttackInfo::~AttackInfo() {
	for (unsigned int i = 0; i < m_elements.size(); i++) {
		delete m_elements[i];
	}
}


/*
* csvファイルから攻撃情報を読み込む
*/
AttackInfoCreator::AttackInfoCreator() {
	// ファイルポインタ
	int fp;

	// バッファ
	const int size = 512;
	char buff[size];
	fp = FileRead_open("data/csv/attackInfo.csv");

	// ファイルの終端までループ
	string attackName = "";
	while (FileRead_eof(fp) == 0) {

		// 一行分のテキストをデータにしてVectorに変換
		FileRead_gets(buff, size, fp);
		vector<string> oneDataVector;
		oneDataVector = csv2vector(buff);

		if (oneDataVector[0] == "name") {
			attackName = oneDataVector[1];
		}
		else {
			m_data[attackName].push_back(oneDataVector);
		}
	}
	FileRead_close(fp);
}


AttackInfo* AttackInfoCreator::createAttackInfo(std::string attackName, int baseDamage) {
	AttackInfo* attackInfo = new AttackInfo();

	// m_data[i][columnName] == valueとなるiを調べる
	map<string, vector<vector<string> > >::iterator ite = m_data.find(attackName);

	if (ite == m_data.end()) {
		ErrorLogAdd((attackName + "がattackInfo.csvにありません。").c_str());
		delete attackInfo;
		return createAttackInfo("アカツキ", 0);
	}
	
	vector<vector<string> > sources = ite->second;
	int baseY = 0, baseX = 0; // 基準となる座標
	for (unsigned int y = 0; y < sources.size(); y++) {
		for (unsigned int x = 0; x < sources[y].size(); x++) {
			if (sources[y][x] == "x") {
				baseY = y;
				baseX = x;
				break;
			}
		}
	}

	for (unsigned int y = 0; y < sources.size(); y++) {
		for (unsigned int x = 0; x < sources[y].size(); x++) {
			if (sources[y][x] != "x" && !sources[y][x].empty()) {
				attackInfo->pushAttackElement(new AttackElement(stoi(sources[y][x]), x - baseX, y - baseY));
			}
		}
	}

	if (baseDamage != 0) {
		attackInfo->pushAttackElement(new AttackElement(baseDamage, 0, 0));
	}

	return attackInfo;
}
