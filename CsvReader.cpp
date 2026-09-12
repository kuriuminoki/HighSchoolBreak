#include "CsvReader.h"
#include "DxLib.h"


using namespace std;


// 一行分のテキストから一行分のデータに変換
vector<string> csv2vector(string buff) {
	vector<string> data;
	string oneCell = "";

	// 1文字ずつ見ていく
	for (int i = 0; buff[i] != '\0'; i++) {
		// CSVファイルなのでカンマで区切ってoneDataにpush_back
		if (buff[i] == ',') {
			data.push_back(oneCell);
			oneCell = "";
		}
		else { // カンマ以外の文字なら合体
			oneCell += buff[i];
		}
	}
	// 最後のセルにはカンマがない
	data.push_back(oneCell);

	return data;
}


// ファイル名を指定してCSVファイルを読み込む
CsvReader::CsvReader(const char* fileName) {

	// ファイルポインタ
	int fp;

	// バッファ
	const int size = 512;
	char buff[size];
	fp = FileRead_open(fileName);

	// バッファに一行目（カラム名）のテキストを入れる
	FileRead_gets(buff, size, fp);
	m_columnNames = csv2vector(buff);

	// ファイルの終端までループ
	while (FileRead_eof(fp) == 0) {

		// 一行分のテキストをデータにしてVectorに変換
		FileRead_gets(buff, size, fp);
		vector<string> oneDataVector;
		oneDataVector = csv2vector(buff);

		// vectorとカラム名を使ってmapを生成
		map<string, string> oneData;
		for (int i = 0; i < oneDataVector.size(); i++) {
			oneData[m_columnNames[i]] = oneDataVector[i];
		}

		// 一行分のmapデータをpush_back
		m_data.push_back(oneData);
	}
	FileRead_close(fp);
}


/*
* カラム名がvalueのデータを取得
* 例：findOne("name", "キャラ名");
* 見つからなかったら空のデータを返す
*/
map<string, string> CsvReader::findOne(const char* columnName, const char* value) {

	// m_data[i][columnName] == valueとなるiを調べる
	map<string, string>::iterator ite;
	for (int i = 0; i < m_data.size(); i++) {
		ite = m_data[i].find(columnName);

		if (ite == m_data[i].end()) { continue; }

		if (ite->second == value) {
			return m_data[i];
		}
	}

	// 見つからなかった場合は空のデータを返す
	map<string, string> res;
	return res;
}


/*
* 全データを返す
*/
vector<map<string, string>> CsvReader::getData() const {
	return m_data;
}
